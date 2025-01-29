#include "chart_projections.h"
#include "ui_chart_projections.h"
#include <QDir>
#include <iostream>
using namespace std;

DerivativesChart::DerivativesChart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DerivativesChart)
{
    ui->setupUi(this);
}// ~DerivativesChart

DerivativesChart::~DerivativesChart()
{
    delete ui;
}//~DerivativesChart

// The values of  Projection
void DerivativesChart::setData_scanline(const vector<double> values)
{
        derivativesSet = new QBarSet("barset");
        derivativesSet->setColor("#5b5b5b"); // the colour in general
        derivativesSet->setPen(QPen(QColor("#5b5b5b"), 1, Qt::SolidLine));

        int n = values.size();
        for (int i = 0; i < n; i++)
        {
            *derivativesSet << values[i];
        }


        hbseries = new QBarSeries();
        hbseries->append(derivativesSet);
        hbseries->setLabelsVisible(false);

        lseries = new QLineSeries();
        lseries->setName("line series");
        lseries->setColor("#5b5b5b");

        lseries->setPointLabelsVisible(false);
        // Apply point labels to each series of data
        for (int i = 0; i < n; i++)
        {
            lseries->append(i,values[i]);
        }

        chart = new QChart();
        chart->setTitle("Calculated values of the Projection Chart");

        chart->addSeries(hbseries);
        chart->addSeries(lseries);
        chart->createDefaultAxes();

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::HorizontalRubberBand); /*      If the left mouse button is pressed and the rubber band is enabled,
                                                                             ... the rubber band is displayed on the screen.
                                                                            This enables the user to select the zoom area.
                                                                            How to come back to the main by default area?
                                                                            By clicking multiple times with rigth button of the mouse.
                                                                    */
        chartView->setParent(ui->values_of_scanline);
        //Resizing dynamically the window
        layout = new QVBoxLayout(ui->values_of_scanline);
        layout->addWidget(chartView,2);


        qreal scaleFactor = chartView->devicePixelRatio();
        // Scale the QPixmap based on the scale factor
        QPixmap scaledPixmap = chartView->grab().scaled(chartView->size() * scaleFactor, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
} // ~DerivativesChart


// The Gaussian KERNEL graphic  (the actual code for Gaussian filter is given below as a comment)
//void DerivativesChart::setData_gaussian1dfilter(const vector<double> kernel)
//{
//    int halfKernelSize = kernel.size() / 2;

//    // Create a QScatterSeries to hold the data points
//    QScatterSeries *scatterSeries = new QScatterSeries();
//    scatterSeries->setMarkerSize(8); // Set the size of the markers



//    // Set custom color using hex code
//    QColor customColor("#333332");
//    scatterSeries->setColor(customColor); // Set the color of the markers


//    // Populate the QScatterSeries with Gaussian kernel values
//    for (int i = 0; i < kernel.size(); ++i) {
//        double x = i - halfKernelSize; // Center the x-values around zero
//        double y = kernel[i];
//        scatterSeries->append(x, y);
//    }

//    // Create a QChart and add the scatter series
//    QChart *chart = new QChart();
//    chart->addSeries(scatterSeries);
//    chart->setTitle("Shape of the Gaussian Function G(x) (σ =  20)");


//    // Set up the x and y axes
//    QValueAxis *axisX = new QValueAxis;
//    axisX->setRange(-halfKernelSize, halfKernelSize);
//    axisX->setTitleText("Position x");
//    axisX->setTickCount(11); // Set to 11 for 10 intervals
//    axisX->setLabelFormat("%.1f"); // Format for displaying ticks (optional)

//    QValueAxis *axisY = new QValueAxis;
//    axisY->setRange(0, *std::max_element(kernel.begin(), kernel.end()) * 1.1);
//    axisY->setTitleText("Gaussian Value G(x)");
//    axisY->setTickCount(8); // Set to 11 for 10 intervals
//    axisY->setLabelFormat("%.4f"); // Format for displaying ticks (optional)


//    // Add the axes to the chart and attach them to the scatter series
//    chart->addAxis(axisX, Qt::AlignBottom);
//    chart->addAxis(axisY, Qt::AlignLeft);
//    scatterSeries->attachAxis(axisX);
//    scatterSeries->attachAxis(axisY);

//    // Create the chart view
//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);
//    chartView->setRubberBand(QChartView::HorizontalRubberBand);
//    chartView->setParent(ui->derivativesPlot3);

//    // Remove any existing layout and add the new chart view
//    if (ui->derivativesPlot3->layout() != nullptr) {
//        QLayoutItem *item;
//        while ((item = ui->derivativesPlot3->layout()->takeAt(0)) != nullptr) {
//            delete item->widget();
//            delete item;
//        }
//        delete ui->derivativesPlot3->layout();
//    }

//    QVBoxLayout *layout = new QVBoxLayout(ui->derivativesPlot3);
//    layout->addWidget(chartView, 2);

//    qreal scaleFactor = chartView->devicePixelRatio();


//    // Capture the entire QChartView
//    QPixmap pixmap = chartView->grab(); // This captures the current visible area
//    QPixmap scaledPixmap = pixmap.scaled(chartView->size() * scaleFactor, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


//}

// The ACTUAL Gaussian 1D filter values applied on Horizontal or Vertical Projection's values
void DerivativesChart::setData_gaussian1dfilter(const vector<double> f_values)
{

        derivativesSet = new QBarSet("barset");
        derivativesSet->setColor("#58b4c8"); // the colour in general
        derivativesSet->setPen(QPen(QColor("#58b4c8"), 1, Qt::SolidLine));

        int n = f_values.size();
        for (int i = 0; i < n; i++) {
            *derivativesSet << f_values[i];
        }

        bseries = new QBarSeries();
        bseries->append(derivativesSet);
        bseries->setLabelsVisible(false);

        lseries = new QLineSeries();
        lseries->setName("line series");
        lseries->setColor("#58b4c8");

        for (int i = 0; i < n; i++) {
            lseries->append(i,f_values[i]);
        }

        chart = new QChart();
        chart->setTitle("Gaussian 1D filter values applied on Projection values");
        chart->addSeries(bseries);
        chart->addSeries(lseries);
        chart->createDefaultAxes();

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::HorizontalRubberBand);
        chartView->setParent(ui->derivativesPlot3);
        layout = new QVBoxLayout(ui->derivativesPlot3);
        layout->addWidget(chartView,2);



        qreal scaleFactor = chartView->devicePixelRatio();
        // Scale the QPixmap based on the scale factor
        QPixmap scaledPixmap = chartView->grab().scaled(chartView->size() * scaleFactor, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

} // setData_gaussian1dfilter



// The difference between the Vertical Projection's values and the Gaussian 1D filter values applied on Vertical Projection's values
void DerivativesChart::setData_difference(const vector<double> values, const vector<double> f_values)
{
        derivativesSet = new QBarSet("barset");
        derivativesSet->setColor("#5b5b5b");
        derivativesSet->setPen(QPen(QColor("#5b5b5b"), 1, Qt::SolidLine));

        int n = values.size();
        for (int i = 0; i < n; i++) {
            *derivativesSet << values[i];
        }

        bseries = new QBarSeries();
        bseries->append(derivativesSet);

        lseries = new QLineSeries();
        lseries->setName("Noisy Signal (Original projection values)");
        lseries->setColor("#5b5b5b");

        for (int i = 0; i < n; i++) {
            lseries->append(i,values[i]);
        }

        derivativesSet2 = new QBarSet("barset2");
        derivativesSet2->setColor("#58b4c8");
        derivativesSet2->setPen(QPen(QColor("#58b4c8"), 1, Qt::SolidLine));

        for (int i = 0; i < n; i++) {
            *derivativesSet2 << f_values[i];
        }
        bseries2 = new QBarSeries();
        bseries2->append(derivativesSet2);


        lseries2 = new QLineSeries();
        lseries2->setName("Signal after noise removal(Gaussian 1D Filter on Projection values)");
        lseries2->setColor("#58b4c8");

        for (int i = 0; i < n; i++) {
            lseries2->append(i,f_values[i]);
        }

        chart = new QChart();
        chart->setTitle("The signal after and before noise removal Chart");
        chart->addSeries(bseries);
        chart->addSeries(bseries2);

        chart->createDefaultAxes();
        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::HorizontalRubberBand);
        chartView->setParent(ui->derivativesPlot4);

        layout = new QVBoxLayout(ui->derivativesPlot4);
        layout->addWidget(chartView,2);


        qreal scaleFactor = chartView->devicePixelRatio();
        // Scale the QPixmap based on the scale factor
        QPixmap scaledPixmap = chartView->grab().scaled(chartView->size() * scaleFactor, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
} // setData_difference


// The First Derivative values of the vertical projection
void DerivativesChart::setData_firstDerivative(const vector<double> frst_derivatives_vect)
{
        derivativesSet = new QBarSet("barset");
        derivativesSet->setColor("#5b5b5b"); // the colour in general
        derivativesSet->setPen(QPen(QColor("#5b5b5b"), 0.5, Qt::SolidLine));

        int n = frst_derivatives_vect.size();
        for (int i = 0; i < n; i++)
        {
            *derivativesSet << frst_derivatives_vect[i];
        }

        bseries = new QBarSeries();
        bseries->append(derivativesSet);

        lseries = new QLineSeries();
        lseries->setName("line series");
        lseries->setColor("#5b5b5b");

        for (int i = 0; i < n; i++) {
            lseries->append(i, frst_derivatives_vect[i]);
        }

        chart = new QChart();
        chart->setTitle("First Derivative Chart on Projection values");
        chart->addSeries(bseries);
        chart->addSeries(lseries);
        chart->createDefaultAxes();

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::HorizontalRubberBand); /*      If the left mouse button is pressed and the rubber band is enabled,
                                                                             ... the rubber band is displayed on the screen.
                                                                            This enables the user to select the zoom area.
                                                                            How to come back to the main by default area?
                                                                            By clicking multiple times with rigth button of the mouse.
                                                                    */
        chartView->setParent(ui->derivativesPlot1);
        //Resizing dynamically the window
        layout = new QVBoxLayout(ui->derivativesPlot1);
        layout->addWidget(chartView,2);


        qreal scaleFactor = chartView->devicePixelRatio();
        // Scale the QPixmap based on the scale factor
        QPixmap scaledPixmap = chartView->grab().scaled(chartView->size() * scaleFactor, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
} // setData_firstDerivative

// The Second Derivative values of the vertical projection
void DerivativesChart::setData_secondDerivative(const vector<double> scnd_derivatives_vect)
{

        derivativesSet = new QBarSet("barset");
        derivativesSet->setColor("#58b4c8"); // the colour in general
        derivativesSet->setPen(QPen(QColor("#58b4c8"), 1, Qt::SolidLine));

        int n = scnd_derivatives_vect.size();
        for (int i = 0; i < n; i++) {
            *derivativesSet << scnd_derivatives_vect[i];
        }

        bseries = new QBarSeries();
        //bseries->barSets();
        bseries->append(derivativesSet);

        lseries = new QLineSeries();
        lseries->setName("line series");
        lseries->setColor("#58b4c8");

        for (int i = 0; i < n; i++) {
            lseries->append(i,scnd_derivatives_vect[i]);
        }

        chart = new QChart();
        chart->setTitle("Second derivative Chart on Projection values");
        chart->addSeries(bseries);
        chart->addSeries(lseries);
        chart->createDefaultAxes();

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::HorizontalRubberBand);
        chartView->setParent(ui->derivativesPlot2);
        layout = new QVBoxLayout(ui->derivativesPlot2);
        layout->addWidget(chartView,2);


        qreal scaleFactor = chartView->devicePixelRatio();
        // Scale the QPixmap based on the scale factor
        QPixmap scaledPixmap = chartView->grab().scaled(chartView->size() * scaleFactor, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
} // setData_secondDerivative




