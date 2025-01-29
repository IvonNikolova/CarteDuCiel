#ifndef SOBELFILTER_H
#define SOBELFILTER_H
#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QString>
#include <cmath>
#include <QImageReader>

#include <iostream>
#include <vector>
#include <fstream>
#include <vector>
using namespace std;


class SobelFilter
{
public:
    SobelFilter();
    // Calculate the gradient magnitude only
    void applySobelEgeDetection(const QImage& image, QImage& filteredImage);
};

#endif // SOBELFILTER_H
