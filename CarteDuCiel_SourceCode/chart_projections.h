#ifndef GRAPH_PROJECTIONS_AND_DERIVATIVES_H
#define GRAPH_PROJECTIONS_AND_DERIVATIVES_H

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
#include <QHorizontalBarSeries>

#include <vector>

using namespace std;

namespace Ui {
class DerivativesChart;
}

// QT DESIGNER FORM CLASS + QDialog without buttons
class DerivativesChart : public QDialog
{
    Q_OBJECT

    public:
        explicit DerivativesChart(QWidget *parent = nullptr);
        ~DerivativesChart();

        /*
            Original calculated values of horizontal projection.
        */
         void setData_scanline(const vector<double> values);
         // The smoothed version of the projection data
          void setData_gaussian1dfilter(const vector<double> f_values);
         // Gaussian 1d kernel (bell-curve) diagram which is applied on the projections data in order to smooth it by convolution
         void setData_gaussianKernel(const vector<double> values);
         // First and Second derivatives
         void setData_firstDerivative(const vector<double> frst_derivatives_vect);
         void setData_secondDerivative(const vector<double> scnd_derivatives_vect);

         /*
            The difference between second derivative (noisy signal)
            and
            the result of gaussian filter applied on the second derivative (signal after noise removal).
         */
         void setData_difference(const vector<double> values, const vector<double> f_values);

protected:
        Ui::DerivativesChart *ui;
        QFrame* centralFrame;

        QBarSeries *hbseries;

        QBarSet *derivativesSet,*derivativesSet2;
        QBarSeries *bseries,*bseries2;
        QLineSeries *lseries,*lseries2;
        QChart *chart;
        QValueAxis *axisX,*axisY;
        QChartView *chartView;
        QVBoxLayout *layout;
};

#endif // GRAPH_PROJECTIONS_AND_DERIVATIVES_H
