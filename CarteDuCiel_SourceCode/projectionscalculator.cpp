#include "projectionscalculator.h"

#include <QPainter>
#include <QWidget>
#include <vector>

using namespace std;

ProjectionsCalculator::ProjectionsCalculator() {}// ProjectionsCalculator

//---------------------------------------------------------------------HORIZONTAL & VERTICAL PROJECTIONS------------------------------------------------------------------------------------------------------------------------------------------
void ProjectionsCalculator::horizontalPrjctn(QImage &image, int thr, vector<double>& hp, int rows, int cols)
{
    // Horizontal projection involves summing pixel intensities along each row of an image, across all columns.
    // row --------->  value
    // row --------->  value
    // row --------->  value
    // .............
    // Output: Y coordinates results within the astrigraphic map image

    rows = image.height(),
    cols = image.width();

      vector <double> horizontal_projection(image.height(),0);

      // The index i represents the current row being processed.
      // Loop through each row of the image
      for (int i = 0; i < rows; i++) // i is the row index
      {
          // Loop over the columns within the current row.
          // The index j represents the current column being processed in that row.
          for (int j = 0; j < cols; j++) // j is the column index within the current row.
          {
              // Pointer to the pixel at row 'i' and column 'j'
              quint8* pixel = (quint8*)(image.bits() + i * image.bytesPerLine() + j);
              /*
               * Check if the pixel value at (i, j) is less than or equal to a given threshold:
               */
              if (*pixel <= thr) //0 i.e. black
              {
                  // Increment the count of black pixels in the current row
                  // This is used to keep track of the number of black pixels found in the i-th row.
                  horizontal_projection[i] += 1; // count the black pixels on the selected row
              }
          }
      }
      hp.assign(std::begin(horizontal_projection), std::end(horizontal_projection));

}// horizontalPrjctn


vector<double> ProjectionsCalculator::verticalPrjctn(QImage &image, int thr)
{

    /**
     *  Vertical projection involves summing pixel intensities of the objects (black pixels)
     *  in each column across all rows.
     *  So, for each column, we add up the pixel values from top to bottom (across all rows).
     *
     *  This is similar to what we're doing for rows,
     *  but instead of iterating through rows and then columns,
     *  we need to iterate through columns and then rows.
     *
     *  The result will be a 1D array or vector where each element represents the total pixel intensity in a specific column of the image.
    */
    //col col col col col col col .............
    // |   |   |   |   |   |   |
    // |   |   |   |   |   |   |
    // v   v   v   v   v   v   v
    // Output: X coordinates results within the astrigraphic map image

    int rows = image.height();
    int cols = image.width();

    //Create a vector of size image.width() with all values as 0
    std::vector<double> vertical_projection(image.width(), 0); //i.e. we will have 'image.width()' number of elements assign to 0 (for now)

    // The index j represents the current column being processed.
    // Loop through each column of the image
    for (int j = 0; j < cols; j++)
    {
        // This loop iterates over the rows within the current column.
        // The index i represents the current row being processed in that column.
        for (int i = 0; i < rows; i++) // Loop through each row in the current column
        {
             // Pointer to the pixel at row 'i' and column 'j'
             quint8* pixel = (quint8*)(image.bits() + i * image.bytesPerLine() + j);

             if (*pixel <= thr) //0 i.e. black
             {
                // Add the pixel value to the sum for the current column, i.e.,
                // summing pixel intensities for the projection
                vertical_projection[j] += 1; // vertical_projection is an array where each index corresponds to the column
             }
         }
      }
      return vertical_projection;
}// verticalPrjctn


void ProjectionsCalculator::printProjections(vector<double> vect)
{
    // Print projection values
    for (int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << ", ";
        // QTextStream(stdout) << i << ":" << vect[i] << ", ";
    }
    cout << endl;
}// print_Proj



