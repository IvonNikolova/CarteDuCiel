#ifndef GRIDLINESDETECTOR_H
#define GRIDLINESDETECTOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class GridlinesDetector
{
    public:
        GridlinesDetector();
        /*
            Define a threshold.

            The threshold allows you to ignore small peaks and valleys.
            It is set as an absolute magnitude to which your input signal is compared.
            For each peak detected, the signal must cross the threshold once on the way up and once on the way down.
            The highest point in between those two points is the peak.
        */
        void threshold(double max_h_frame1, double max_h_frame2, double min_value, double &thr);
        /*
            Find the peaks.
            The peaks are the most met black pixels of a given point, whose pixels belong to the coordinate system/lines.
            Apply the peak detector on the horizontal projection values.
        */
        void findPeaks(vector<double> A, vector<double> &pos, vector<double> &peaks, const double thr);
        /*
            Find a index of a given element.

            Having a value of an element,
            we should find out its index.
        */

        double findIndex(vector<double> const &A, double const &index);


        /*
           Find the values of given elements.

           Having the indices of multiple elements,
           we should find out the actual values of these elements.
        */

        vector<double> findValues(vector<double> const &A, vector<double> const &vals);
        /*
            Find multiple indices of given elements.

            Having the values of the elements,
            we should find out their indices.
        */

        vector<double> findIndices(vector<double> const &A, vector<double> const &indices);

        void print(vector<double> vect);


        /*
            Remove unnecessary detected lines.

            The step before extracting the data.
            We take care of the number of the peaks,
            if the vector of horizontal projection peaks i.e. coordinate lines
            or the vector of  vertical projection ones,
            each containes more than 27 values,
            is it possible to detect the lines next to the whole grid
            i.e. the lines having values: 60,55,50,45,40....
            So, we will remove these fake peaks,
            because they are not part of the grid i.e. coordinate system lines.
         */
        vector<double> excludeNeighbours(const vector<double> numbers);

};

#endif // GRIDLINESDETECTOR_H
