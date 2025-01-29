#include "gaussian1dfilter.h"

Gaussian1DFilter::Gaussian1DFilter() {}// Gaussian1DFilter

double  Gaussian1DFilter::gaussianWithoutNormalization(int x, double sigma) {
    // Gaussian function without normalization factor
    return exp(-0.5 * pow(x / sigma, 2));
}// gaussianWithoutNormalization

double  Gaussian1DFilter::gaussianWithNormalization(int x, double sigma) {
    // Gaussian function with normalization factor (full Gaussian)
    return exp(-0.5 * pow(x / sigma, 2)) / (sqrt(2 * M_PI) * sigma);
}// gaussianWithNormalization

vector<double> Gaussian1DFilter::createGaussianKernel(double sigma)
{
    int kernel_size = 6 * sigma + 1; // choose kernel size based on sigma
    int half_kernel_size = kernel_size / 2; // get half kernel size

    vector<double> kernel(kernel_size);
    // Compute kernel values
    double sum = 0.0;
    for (int i = -half_kernel_size; i <= half_kernel_size; i++)
    {
        kernel[i + half_kernel_size] = gaussianWithoutNormalization(i, sigma);
        sum += kernel[i + half_kernel_size];
    }

    // Normalize kernel here
    for (int i = 0; i < kernel_size; i++)
    {
        // Print out each kernel values
         kernel[i] /= sum;
    }
    return kernel;
}// createGaussianKernel

vector<double> Gaussian1DFilter::applyConvolutionWithGaussian1Dfilter(const vector<double>& data, const vector<double>& kernel) {
    // create output vector
    int data_size = data.size(); // signal data size
    int kernelSize = kernel.size(); // kernel size
    int half_kernel_size = kernelSize / 2;
    // A vector to store the filtered signal data result
    vector<double> filtered_data(data_size);

    // Apply filter
    for (int i = 0; i < data_size; i++)
    {
        double value = 0.0;
        for (int j = -half_kernel_size; j <= half_kernel_size; j++)
        {
            int index = i + j;
            if (index >= 0 && index < data_size)
            {
                value += kernel[j + half_kernel_size] * data[index];
            }
        }
        filtered_data[i] = value;
    }

    return filtered_data;
} // applyConvolutionWithGaussian1Dfilter

void Gaussian1DFilter::print(vector<double> vect)
{
    for(int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << ", ";
    }
    cout << endl;
}// print
