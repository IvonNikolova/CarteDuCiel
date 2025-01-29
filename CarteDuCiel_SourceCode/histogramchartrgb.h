#ifndef HISTOGRAMCHARTRGB_H
#define HISTOGRAMCHARTRGB_H

#include <QDialog>

namespace Ui {
class HistogramChartRGB;
}

class HistogramChartRGB : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramChartRGB(QWidget *parent = nullptr);
    ~HistogramChartRGB();

private:
    Ui::HistogramChartRGB *ui;
    /*QBarSet  *histogramSet_r, *histogramSet_g, *histogramSet_b;
    QBarSeries *bseries_r, *bseries_g, *bseries_b;
    QChart  *chart_rgb;
    QValueAxis *axisX,*axisY, *axisX_rgb, *axisY_rgb;
    QChartView *chartView, *chartView_rgb;*/
};

#endif // HISTOGRAMCHARTRGB_H
