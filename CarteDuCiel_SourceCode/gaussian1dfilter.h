#ifndef GAUSSIAN1DFILTER_H
#define GAUSSIAN1DFILTER_H

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;


class Gaussian1DFilter
{
    public:
         // Gaussian 1-d smoothing filter used for smoothing the obaserved projections calculations.
        Gaussian1DFilter();
        double  gaussianWithoutNormalization(int x, double sigma);
        double  gaussianWithNormalization(int x, double sigma);
        vector<double> createGaussianKernel(double sigma);
        vector<double> applyConvolutionWithGaussian1Dfilter(const vector<double>& data, const vector<double>& kernel);
        void print(vector<double> vect);
};
#endif
