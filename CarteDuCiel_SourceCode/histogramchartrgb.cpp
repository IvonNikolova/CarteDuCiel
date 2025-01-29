#include "histogramchartrgb.h"
#include "ui_histogramchartrgb.h"

HistogramChartRGB::HistogramChartRGB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramChartRGB)
{
    ui->setupUi(this);
}

HistogramChartRGB::~HistogramChartRGB()
{
    delete ui;
}
