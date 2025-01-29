#include "boundingboxcreator.h"

BoundingBoxCreator::BoundingBoxCreator() { }//BoundingBoxCreator

// Function to calculate and create a bounding box (minimum spanning window) around a set of three points (the circles' centers) with a margin
cv::Rect BoundingBoxCreator::calculateBoundingBox(const std::vector<cv::Point>& points, int margin)
{
    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = INT_MIN;
    int maxY = INT_MIN;

    for (const cv::Point& point : points)
    {
        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);
        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
    }

    // Add margin to the bounding box
    minX = std::max(minX - margin, 0);
    minY = std::max(minY - margin, 0);
    maxX = std::min(maxX + margin, INT_MAX);
    maxY = std::min(maxY + margin, INT_MAX);

    return cv::Rect(minX, minY, maxX - minX, maxY - minY);
}//calculateBoundingBox


// Calculate the global coordinates relative to the whole astrographic map of each minimum spanning window created that depicts an actual detected asterism
std::pair<cv::Point, cv::Point> BoundingBoxCreator::calculateGlobalRectangleCoordinates(
    const cv::Rect& boundingBox, // The rectangle within the sub-image
    const Boundary& subSquare    // The top-left corner of the sub-image within the larger image
) {
    // Calculate the top-left corner of the rectangle in the context of the larger image
    int globalTopLeftX = subSquare.startCol + boundingBox.x;
    int globalTopLeftY = subSquare.startRow + boundingBox.y;

    // Calculate the bottom-right corner of the rectangle in the context of the larger image
    int globalBottomRightX = subSquare.startCol + boundingBox.x + boundingBox.width;
    int globalBottomRightY = subSquare.startRow + boundingBox.y + boundingBox.height;


    // Return the global coordinates of the rectangle
    return { cv::Point(globalTopLeftX, globalTopLeftY), cv::Point(globalBottomRightX, globalBottomRightY) };
}//calculateGlobalRectangleCoordinates


// Crop and save the minimum spanning windows that contain the actual asterisms
void BoundingBoxCreator::saveCroppedImage(const std::vector<double>& imageA, const std::pair<cv::Point, cv::Point>& globalCoordinates, const std::string& folderPath, int imageWidth) {
    // Create the folder if it doesn't exist
    std::filesystem::create_directories(folderPath);

    // Extract the top-left and bottom-right points from the global coordinates
    cv::Point topLeft = globalCoordinates.first;
    cv::Point bottomRight = globalCoordinates.second;

    // Define the ROI using the bounding box
    cv::Rect roi(topLeft, bottomRight);

    // Ensure the ROI is within the bounds of the image
    roi.x = std::max(0, roi.x);
    roi.y = std::max(0, roi.y);
    roi.width = std::min(roi.width, imageWidth - roi.x);

    // Check if the ROI dimensions are valid
    if (roi.width > 0 && roi.height > 0) {
        // Convert the 1D data to a 2D image (cv::Mat)
        cv::Mat imageMat(imageA.size() / imageWidth, imageWidth, CV_8U);
        for (int i = 0; i < imageMat.rows; ++i) {
            for (int j = 0; j < imageMat.cols; ++j) {
                imageMat.at<uchar>(i, j) = static_cast<uchar>(imageA[i * imageMat.cols + j]);
            }
        }

        // Extract the cropped region using the ROI
        cv::Mat croppedImage = imageMat(roi);

        static int numb;
        // Create the filename
        std::string filename = "cropped_rectangle_"+std::to_string(++numb)+".jpg";

        // Save the cropped image to the specified folder
        cv::imwrite(std::filesystem::path(folderPath) / filename, croppedImage);
    }
}//saveCroppedImage


//// ******************* All detected asterism on one astro map at once - so we draw all minimum spanning windows in green in the origianal format astro. map *******************
//void BoundingBoxCreator::saveCroppedImage(const std::vector<double>& imageA, const std::pair<cv::Point, cv::Point>& globalCoordinates, const std::string& folderPath, int imageWidth) {
//    // Create the folder if it doesn't exist
//    std::filesystem::create_directories(folderPath);

//    // Extract the top-left and bottom-right points from the global coordinates
//    cv::Point topLeft = globalCoordinates.first;
//    cv::Point bottomRight = globalCoordinates.second;

//    // Define the ROI using the bounding box
//    cv::Rect roi(topLeft, bottomRight);

//    // Ensure the ROI is within the bounds of the image
//    roi.x = std::max(0, roi.x);
//    roi.y = std::max(0, roi.y);
//    roi.width = std::min(roi.width, imageWidth - roi.x);

//    // Check if the ROI dimensions are valid
//    if (roi.width > 0 && roi.height > 0) {
//        // Convert the 1D data to a 2D image (cv::Mat)
//        cv::Mat imageMat(imageA.size() / imageWidth, imageWidth, CV_8U);
//        for (int i = 0; i < imageMat.rows; ++i) {
//            for (int j = 0; j < imageMat.cols; ++j) {
//                imageMat.at<uchar>(i, j) = static_cast<uchar>(imageA[i * imageMat.cols + j]);
//            }
//        }

//        // Extract the cropped region using the ROI
//        cv::Mat croppedImage = imageMat(roi);

//        cv::cvtColor(imageMat, imageMat, cv::COLOR_GRAY2BGR);
//        // Vector to store all ROIs
//           static std::vector<cv::Rect> rois;

//            // Dynamically add ROIs as needed
//            rois.push_back(roi);
//            // Loop through each ROI and draw it on the imageMat
//                for (const auto& roi : rois) {
//                    cv::rectangle(imageMat, roi, cv::Scalar(0, 255, 0), 1);  // Green rectangle
//                }

//        // Draw the rectangle around the ROI in green (Scalar(0, 255, 0))
//            cv::rectangle(imageMat, roi, cv::Scalar(0, 255, 0), 1);  // Green color in BGR format

//        static int numb;
//        // Create the filename
//        std::string filename = "cropped_rectangle_"+std::to_string(++numb)+".jpg";

//        // Save the cropped image to the specified folder
//        cv::imwrite(std::filesystem::path(folderPath) / filename, imageMat);
//    }
//}//saveCroppedImage
