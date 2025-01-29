#include "sobelfilter.h"

SobelFilter::SobelFilter(){}//SobelFilter

void SobelFilter::applySobelEgeDetection(const QImage& image, QImage& filteredImage) // 'filteredImage' represents the gradient magnitudes computed using the Sobel operator.
{
    int width = image.width();
    int height = image.height();

    filteredImage = QImage(width, height, QImage::Format_Grayscale8);

    // Apply the Sobel operators (kernels)
    const QVector<int> sobelX = { -1, 0, 1, -2, 0, 2, -1, 0, 1 }; // Horizontal Sobel Kernel
    const QVector<int> sobelY = { -1, -2, -1, 0, 0, 0, 1, 2, 1 }; // Vertical Sobel kernel

    /** 
     * Iterate through each pixel of the image and apply convolution using the Sobel kernels
     * along both x-axis and y-axis utulizing the kernels
     */ 
    for (int indx_row = 0; indx_row < height; indx_row++)
    {
        // Modifying filteredImage directly via pointers
        quint8* filteredPtr_row = reinterpret_cast<quint8*>(filteredImage.scanLine(indx_row));

        for (int indx_col = 0; indx_col < width; indx_col++)
        {
            // Initialize the gradients
            int sumX = 0;
            int sumY = 0;

            for (int ky = -1; ky <= 1; ky++) // 3x3 Sobel kernel values
            {
                /**
                 * Handle row boundaries by clamping.
                 * qBound() to ensure that the pixel indices don't go out of bounds when accessing neighboring pixels 
                 * (i.e., the kernel is applied in a sliding window manner).
                */
                int neighborRow = qBound(0, indx_row + ky, height - 1);

                const uchar* neighborScanLine = image.constScanLine(neighborRow);

                for (int kx = -1; kx <= 1; kx++)
                {
                    //Handle column boundaries by clamping.
                    int neighborCol = qBound(0, indx_col + kx, width - 1);
                    // Calculate the index of the kernel
                    int kernelIndex = (ky + 1) * 3 + (kx + 1);
                    // Get the pixel value at the current neighbor
                    int grayValue = neighborScanLine[neighborCol];
                    // Generate horizontal gradient by performing a convolution
                    sumX += grayValue * sobelX[kernelIndex];
                    // Generate vertical gradient by performing a convolution
                    sumY += grayValue * sobelY[kernelIndex];
                }
            }

            // Compute the gradient magnitude
            int magnitude = static_cast<int>(std::sqrt(sumX * sumX + sumY * sumY));
            // Clamp the magnitude to [0,255] to avoid overflow
            magnitude = qBound(0, magnitude, 255);

            // Set the pixel value in the filtered image
            /**
             * The computed gradient magnitude is directly written into the filtered image's memory buffer.
             * filteredPtr_row is a pointer to the current row of filteredImage, provided by scanLine(),
             * so, we change the pixel value directly in memory for better performance.
             * Since 'filteredImage' was initialized earlier, updating filteredPtr_row[indx_col] automatically modifies the output image.
            */
            filteredPtr_row[indx_col] = static_cast<quint8>(magnitude);
        }
    }
}// applySobelEgeDetection

