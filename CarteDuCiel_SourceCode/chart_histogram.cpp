#include "chart_histogram.h"
#include "ui_chart_histogram.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QDir>
#include <QApplication>
#include <QMainWindow>
#include <limits>
#include <algorithm>

HistogramChart::HistogramChart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramChart)
{
    ui->setupUi(this);
}// HistogramChart

HistogramChart::~HistogramChart()
{
    delete ui;
}// ~HistogramChart



// More detailed graph of the histogram and otsu in general
void HistogramChart::setData2(const std::vector<int> data, std::vector<double> cumulative_sum, int th, std::vector<double>  between_class_variance)
{
    // Create Histogram
        histogramSet = new QBarSet("Histogram");
        histogramSet->setColor("#939393");
        histogramSet->setPen(QPen(QColor("#939393"), 1, Qt::SolidLine));

        int maxDataValue = 0;
        for (int value : data)
        {
            *histogramSet << value;
            maxDataValue = std::max(maxDataValue, value); // Track the maximum value
        }

        bseries = new QBarSeries();
        bseries->append(histogramSet);

        // Create Between-Class Variance Plot
        slseries = new QLineSeries();
        slseries->setPen(QPen(QColor("#8ac259"), 2, Qt::SolidLine));

        for (int i = 0; i < between_class_variance.size(); ++i)
        {
            slseries->append(i, between_class_variance[i]);
            //QTextStream(stdout) << between_class_variance[i] << ",";
        }

        // Create Chart
        chart = new QChart();
        chart->setTitle("Otsu Method Visualization");
        chart->addSeries(bseries);
        chart->addSeries(slseries);

        // Create and Setup Axis
        axis_x = new QValueAxis();
        axis_x->setLabelFormat("%d");
        axis_x->setTitleText("Pixel Intensity"); // + threshold
        axis_x->setRange(0, data.size() - 1); // X-axis covers all bins
        chart->addAxis(axis_x, Qt::AlignBottom);
        bseries->attachAxis(axis_x);
        slseries->attachAxis(axis_x);

        axis_y = new QValueAxis();
        axis_y->setTitleText("Pixel Count"); // + variance

        chart->addAxis(axis_y, Qt::AlignLeft);
        bseries->attachAxis(axis_y); // shows the big numbers related to histogram

        // Create secondary Y-Axis for Between-Class Variance
        axis_y_variance = new QValueAxis();
        axis_y_variance->setTitleText("Between-Class Variance"); // For variance
        double maxVarianceValue = (between_class_variance.empty()) ? 0.0 : *std::max_element(between_class_variance.begin(), between_class_variance.end());
        axis_y_variance->setRange(0, maxVarianceValue); // Set range based on variance data
        chart->addAxis(axis_y_variance, Qt::AlignRight); // Place this axis on the right side
        slseries->attachAxis(axis_y_variance); // Attach variance plot to secondary Y-axis

        // Highlight Optimal Threshold
        thresholdLine = new QLineSeries();
        thresholdLine->setPen(QPen(QColor("#080402"), 2, Qt::DashLine)); // Set color and style for the threshold line
        thresholdLine->append(th, 0);
        thresholdLine->append(th, axis_y->max()); // Use axis_y->max() to ensure line goes to the top of the axis

        // Add the threshold line to the chart
        chart->addSeries(thresholdLine);
        thresholdLine->attachAxis(axis_x);
        thresholdLine->attachAxis(axis_y);

        // Display Chart
        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::HorizontalRubberBand);

        // Ensure layout is correctly set up
        if (ui->histoPlot2->layout()) {
            delete ui->histoPlot2->layout(); // Clean up previous layout if it exists
        }
        layout = new QVBoxLayout(ui->histoPlot2);
        layout->addWidget(chartView);
        ui->histoPlot2->setLayout(layout);

        qreal scaleFactor = chartView->devicePixelRatio();
        // Scale the QPixmap based on the scale factor
        QPixmap scaledPixmap = chartView->grab().scaled(chartView->size() * scaleFactor, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}//setData2

// Simpler histogram
void HistogramChart::setData(const std::vector<int> data, std::vector<double> cumulative_sum, int th, std::vector<double>  between_class_variance)
{
    histogramSet = new QBarSet("barset");
    histogramSet->setColor("#939393"); // the colour in general
    histogramSet->setPen(QPen(QColor("#939393"), 1, Qt::SolidLine));

    int size = data.size();
    for (int i = 0; i < size; ++i)
    {
        *histogramSet << data[i];
    }

    bseries = new QBarSeries();
    bseries->append(histogramSet);

    chart = new QChart();
    chart->setTitle("Chart of Image Histogram (Intensity histogram)"); //From Greylevel to then Binary histogram
    chart->addSeries(bseries);
    chart->createDefaultAxes();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::HorizontalRubberBand);
    chartView->setParent(ui->histoPlot);

    //Resizing dynamically the window
     layout = new QVBoxLayout(ui->histoPlot);
     layout->addWidget(chartView,2);
}//setData

