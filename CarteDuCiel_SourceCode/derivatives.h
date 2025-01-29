#ifndef DERIVATIVES_H
#define DERIVATIVES_H

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

class Derivatives
{
    public:
        Derivatives();
        //! Apply 1st. and 2nd. derivatives formulas
        /*!
           Calculate first and second derivatives using the data of a given projection
        */
        void first_derivative(vector<double>& f);
        void second_derivative(vector<double>& f_2);

        //! Print out the results
        /*!
           Print out the result values of first - and second derivatives using the data of the given projection.
        */
        void printDerivativeValues(vector<double> vect);
};

#endif // DERIVATIVES_H
