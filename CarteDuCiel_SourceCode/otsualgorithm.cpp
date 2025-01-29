#include "otsualgorithm.h"

OtsuAlgorithm::OtsuAlgorithm(){} // OtsuAlgorithm

/*
 Unnormalized histogram is the function:
    h(rk) = nk, k=0,1,...,L−1, (7)
    where nk is the number of pixels of f with intensity rk.

    The subdivisions of the intensity scales are called histogram bins.

Normalized histogram is the function:
    p(rk) = h(rk)/MN = nk/MN,
    where M and N are the number of rows and columns of f respectively.

    The following two member functions calcHisto(...) and calcProb(...)
    calculate the unnormalized and normalized histograms h(rk) and p(rk) of an input gray scale image.

    'SUMk p(rk) = 1' and components of p(rk) are estimates of the probabilities of intensity levels occurring in the image.
*/
void OtsuAlgorithm::calcHisto(QImage &image, double histo[]) // Unnormalized histogram h(rk) of an input gray scale image.
{
        // Initialize all intensity values/grey levels to 0
        for (int i = 0; i <= INTENS_MAX; i++)
        {
            histo[i] = 0; //set the image histogram bins for [0] and [255] greylevels to 0 respectively
        }

        // Calculate the number of pixels for each intensity values
        for (int indx_row = 0; indx_row < image.height(); indx_row++)
        {
            quint8* ptr_row = (quint8*)(image.bits() + indx_row * image.bytesPerLine());
            for (int indx_col = 0; indx_col < image.width(); indx_col++)
            {           
                histo[ptr_row[indx_col]]++;
            }
        }
}// calcHisto

void OtsuAlgorithm::printHisto(const double histo[])
{
        for (int i = 0; i <= INTENS_MAX; i++)
        {
             cout << i << ": " << histo[i] << " "; // << endl;
        }
        cout << endl;
}/* printHisto */

void OtsuAlgorithm::calcProb(QImage &image, double histo[]) // Normalized histogram p(rk) of an input gray scale image.
{
        // Calculate the probability distribution of each gray level in the image
        int numb_pix = image.height() * image.width();
        for (int i = 0; i <= INTENS_MAX; i++)
        {
            histo[i] /= numb_pix;
        }
}// calcHisto


int OtsuAlgorithm::otsu(const double histo[], std::vector <double> &cumulative_sums, std::vector <double> &between_class_variance)
{
    // compute cumulative sums P(k)
      double p[INTENS_MAX + 1] = {0};
      p[0] = histo[0];
      for (int i = 1; i <= INTENS_MAX; i++){
          p[i] = p[i - 1] + histo[i];
      }


      // Store the P(k) data in cumulative_sums
      for (int i = 0; i <= INTENS_MAX; i++){
             cumulative_sums.assign(std::begin(p), std::end(p));
      }

      // cumulative mean - mu
      double m[INTENS_MAX + 1] = {0};
      for (int i = 1; i <= INTENS_MAX; i++){
          m[i] = m[i - 1] + i * histo[i];
      }

      // global mean
      double m_g = m[INTENS_MAX];

      // between-class variance
      double b_c[INTENS_MAX + 1] = {0};
      for (int i = 1; i <= INTENS_MAX; i++){
          double div = (p[i] * (1 - p[i]));
          b_c[i] =
              fabs(div < EPS) ? 0 :
              ((m_g * p[i] - m[i]) * (m_g * p[i] - m[i])) / div;
      }

      // Update the threshold by finding max
      double max_variance = 0;
      int max_i = 0; // index i.e. potential optimal threshold
      // Iterate the array
      for (int i = 0; i <= INTENS_MAX; i++)
      {
           // Store the between class variance data
           between_class_variance.assign(std::begin(b_c), std::end(b_c));
           // If current value is greater than max variance then assign it to:
          if (b_c[i] > max_variance) {
            max_variance = b_c[i]; // The current max variance is updated
            max_i = i; // Get that index refering to the potential optimal threshold
          }
      }
      //QTextStream(stdout) << "\n\nThreshold: " << max_i << '\n';
      return max_i;
}// otsu

void OtsuAlgorithm::threshold(QImage &image, int thr)
{
    for (int indx_row = 0; indx_row < image.height(); indx_row++)
    {
        quint8* ptr_row = (quint8*)(image.bits() + indx_row * image.bytesPerLine());
        for (int indx_col = 0; indx_col < image.width(); indx_col++)
        {
           /**
            *  For black objects on white background, we have the following:
            *  if the current pixel ptr_row[indx_col] is <= thr, then, we assign 0 i.e. INTENS_MIN (black), it is an object pixel.
            *  else if (ptr_row[indx_col] > thr), thus, 255 i.e. INTENS_MAX (white), it is background pixel.
           */
            ptr_row[indx_col] =
                (ptr_row[indx_col] <= thr) ? INTENS_MIN : INTENS_MAX;
        }
     }
}// threshold

