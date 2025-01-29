#ifndef CARTEDUCIEL_H
#define CARTEDUCIEL_H

// Qt libraries
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QImageReader>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QValueAxis>
#include <QLineSeries>
#include <opencv2/opencv.hpp>

// C++ libraries
#include <iostream>
#include <cmath> //round() in
#include <vector>

// PostgreSQL
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtSql>
using namespace std;

// another included header files
#include "chart_histogram.h"
#include "chart_projections.h"
#include "coordinatesinspector.h"
#include "custom_view.h"
#include "otsualgorithm.h"
#include "derivatives.h"
#include "gridlinesdetector.h"
#include "gaussian1dfilter.h"
#include "projectionscalculator.h"
#include "sobelfilter.h"
#include "asterismsdetector.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class CarteDuCiel;
}
QT_END_NAMESPACE

class CarteDuCiel : public QMainWindow
{
    Q_OBJECT

    public:
        CarteDuCiel(QWidget *parent = nullptr);
        ~CarteDuCiel();

    private slots:
        //Sources: Doxygen-QT https://www.youtube.com/watch?v=KPN1y_vstjY&list=PLzHdTn7Pdxs7H5w9L_tzdDU1sZ74eb0CS&index=5
        void on_pushB_uploadImg_clicked();
        void on_pushB_deleteImg_clicked();
        void on_pushB_saveImg_clicked();
        void on_pushB_applyAlgorithms_clicked();
        void on_showChart_histo_clicked();
        void on_showChart_vr_projections_clicked();
        void on_showChart_hr_projections_clicked();
        void on_pushB_showCoord_clicked();


private:
            Ui::CarteDuCiel *ui;

            QString filename;

            QVector <QRgb> r_vect;
            QVector <QRgb> g_vect;
            QVector <QRgb> b_vect;
            QVector <QRgb> histo_red, histo_green, histo_blue;


        // Otsu's algorithm
            double histo[INTENS_MAX + 1];
            vector<double> histo_prob, histo_prob0;
            int th;
            vector<int> copy_histo;
            vector<double> copy_prob_histo;

        // Projections
            vector <double> horizontal_projection;
            vector <double> horizontal_projection2;
            vector <double> horizontal_projection0;

            vector <double> vertical_projection;
            vector <double> vertical_projection2;
            vector <double> vertical_projection0;
            vector<double> gaussiankernel1;
            vector<double> gaussiankernel2;

        public:
        // Gaussian 1D filter
            double sigma_h = 20.0;
            double sigma_v = 20.0;

       private:
            vector<double> filtered_hp;
            vector<double> filtered_hp_pos;
            vector<double> filtered_hp_peaks;

            vector<double> filtered_vp;
            vector<double> filtered_vp_pos;
            vector<double> filtered_vp_peaks;

        // Reseau lines - identify maxima
            vector<double> middle_peaks_hp;
            vector<double> middle_pos_hp;

            vector<double> middle_peaks_vp;
            vector<double> middle_pos_vp;

            vector<double> indices_to_colour_hp;
            vector<double> indices_to_colour_vp;

            vector<double> filteredNumbers_hp;
            vector<double> filteredNumbers_vp;


        // Charts
            vector <double> cumulative_sum, cumulative_sum_copy, between_class_variance; // Histogram Chart
            vector<double> copy_first_derivative_v, copy_second_derivative_v, copy_gaussian_v; // Vertical projection Chart
            vector<double> copy_first_derivative_h, copy_second_derivative_h, copy_gaussian_h; // Horizontal projection Chart



    public:

        /* these variables are public in order to be used in secondwindow.cpp */

        QImage sourceImage0; // the variable of the original image
        QImage sourceImage; //the variable of the to-be-changed image
        QImage sourceImage1;
        QImage sobelImage;
        QImage rawImage;

        QGraphicsItem *item, *item0;
        QGraphicsScene *scene, *scene0;

        /*
         *  The objects of the two classes that represent the charts
        */
        DerivativesChart *horchart,*verchart;
        HistogramChart *hchart;
        CoordinatesInspector* imageViewer;
};
#endif // CARTEDUCIEL_H
