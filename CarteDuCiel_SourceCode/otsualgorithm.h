#ifndef OTSUALGORITHM_H
#define OTSUALGORITHM_H

// Qt libraries
#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QTextStream>
#include <QPainter>
#include <QWidget>
#include <QString>

// C++ libraries
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int INTENS_MIN = 0;    //black - objects
const int INTENS_MAX = 255; //white - background

const double EPS = 1.0e-14;

// Segmentation using Thresholding - Otsu’s method that accepts only grayscale images!
class OtsuAlgorithm
{
    public:
        //! Otsu's algorithm
        /*!
            Otsu's algorithm
        */
        OtsuAlgorithm();

        //! Compute both histogram
        /*!
           Compute histogram  of each intensity level/value
        */
        void calcHisto(QImage &image, double histo[]);

        //! Print the histogram
        /*!
           Print out how many times each intensity level/value occurs
        */
        void printHisto(const double histo[]);

        //! Calculate the probability
        /*!
           Calculate the probability of the generated histogram's intensity levels/values
        */
        void calcProb(QImage &image, double histo[]);

        //! Apply Otsu algorithm
        /*!
            Compute the threshold
        */
        int otsu(const double histo[], std::vector <double> &cumulative_sums, std::vector <double> &between_class_variance);


        /*
            Using the calculated threshold,
            now we have to find the objects of the provided image.
            Having the intensity values we compare them to the threshold.
            If a given intensity value of the input image is less than or equal to threshold,
            we set it to INTENS_MIN = 0 i.e. black (object),
            else, we set it to INTENS_MAX = 255 i.e. white (background).

            The process of thresholding involves, comparing each pixel value of the image (pixel intensity) to a specified threshold.
            This divides all the pixels of the input image into 2 groups: Pixels having intensity value lower than threshold.
            Pixels having intensity value greater than threshold.
        */
        void threshold(QImage &image, int thr);
};

#endif // OTSUALGORITHM_H
