#ifndef PROJECTIONSCALCULATOR_H
#define PROJECTIONSCALCULATOR_H

//Qt libraries
#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>

//C++ libraries
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

class ProjectionsCalculator
{
public:
    ProjectionsCalculator();

    //! Calculate the horizontal - and vertical projections
    /*!
       Calculate black pixels using horizontal and vertical projections of a given image
    */
    void horizontalPrjctn(QImage &image, int thr, vector<double>& hp, int rows, int cols);
    vector<double> verticalPrjctn(QImage &image, int thr);

    // Print out the values of the projections.
    void printProjections(vector<double> vect);
};

#endif // PROJECTIONSCALCULATOR_H
