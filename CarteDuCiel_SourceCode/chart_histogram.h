#ifndef GRAPH_HISTOGRAM_H
#define GRAPH_HISTOGRAM_H

// Qt libraries
#include <QPixmap>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLogValueAxis>
#include <QMessageBox>
#include <QErrorMessage>
#include <QPen>
#include <QGridLayout>
#include <QVector>
#include <QDialog>
#include <QCategoryAxis>

// C++ libraries
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// QT DESIGNER FORM CLASS + QDialog without buttons
namespace Ui {
class HistogramChart;
}

class HistogramChart : public QDialog
{
    Q_OBJECT

    public:
        explicit HistogramChart(QWidget *parent = nullptr);
        ~HistogramChart();

        void setData(const std::vector<int> data, std::vector<double> cumulative_sum, int th,std::vector<double>  between_class_variance);
        void setData2(const std::vector<int> data, std::vector<double> cumulative_sum, int th,std::vector<double> between_class_variance);

private:
        Ui::HistogramChart *ui;

        QBarSet *histogramSet; // histogram values
        QBarSeries *bseries;
        QLineSeries *slseries; //cdf

        QValueAxis *axisX, *axisY;//main axis

        QValueAxis *axis_x;
        QValueAxis *axis_y;
        QLineSeries *thresholdLine;

        QValueAxis *axisYprob, *axisY2, *axis_y_variance; //probability and cdf axis

        QChart *chart;
        QChartView *chartView;
        QVBoxLayout *layout;

};

#endif // GRAPH_HISTOGRAM_H
