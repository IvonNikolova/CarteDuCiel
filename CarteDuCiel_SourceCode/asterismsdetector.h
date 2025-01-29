#ifndef ASTERISMSDETECTOR_H
#define ASTERISMSDETECTOR_H

// C++
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> // For std::sort
#include <fstream>
#include <cstdlib>
// OpenCV
#include <opencv2/opencv.hpp>
// Qt
#include <QMainWindow>
#include <QTextStream>
// PostgreSQL
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtSql>

#include "dbstoragehandler.h"
#include "boundingboxcreator.h"
using namespace std;


struct RectangleCoordinates {
    cv::Point topLeft;      // Top-left corner of the rectangle
    cv::Point bottomRight;  // Bottom-right corner of the rectangle
};

// Define a struct to hold the rectangle's global coordinates and the corresponding sub-square
struct RectangleInfo {
    cv::Point topLeft;
    cv::Point bottomRight;
    Boundary subSquare;
};

class AsterismsDetector
{
    // Define a structure to hold rectangle coordinates
    /*
    The coordinates stored in the RectangleCoordinates struct represent the coordinates of a rectangle in image-a.
    These coordinates are calculated based on the projections of the entire image-b within image-a
    (imageBProjectionStart and imageBProjectionEnd) and the projections of the sub-image
    within image-b (subImageProjectionStart and subImageProjectionEnd).
    The function calculateRectangleCoordinates computes the offset between image-a and image-b,
    as well as the offset between the sub-image and image-b,
    and then uses these offsets to determine the coordinates of the rectangle in image-a.
    Therefore, the topLeft and bottomRight points in the RectangleCoordinates struct
    represent the top-left and bottom-right corners of the rectangle within image-a.
    */
    public:
        // Define the structure to hold rectangle information
        struct DetectedRectangle {
            cv::Rect boundingBox;
        };

        AsterismsDetector();


        // RULE-A
        double calculateIntensityThreshold(
                              const std::vector<double> horizontalLines,
                               const std::vector<double> verticalLines,
                               const std::vector<double> imageData,
                               int imageHeight,
                               int imageWidth,
                std::vector<std::vector<double>> subSquareDataCollection,
                double dp, double minDist, double param1, double param2, double minRadius, double maxRadius);

        // RULE-B
        double calculateDistance(const cv::Point& p1, const cv::Point& p2);
        bool areTwoCirclesClose(const cv::Vec3f& circle1, const cv::Vec3f& circle2, double distanceThreshold);
        // RULE-D
        bool checkDistanceBetweenAll(const cv::Vec3f& circle1, const cv::Vec3f& circle2, const cv::Vec3f& circle3, double distanceThreshold);
        // RULE-F
        double calculateAngle(const cv::Point& p1, const cv::Point& p2, const cv::Point& p3);
        // RULE-G
        std::string determineOrientation(cv::Point center1, cv::Point center2, cv::Point center3);


        /*
            Segmentation and Extraction of data.

            Extract each sub-square data within the grid in given boundaries.
            The boundaries are defined by the horizontal - and vertical projection peaks.
            Then, segment and extract the asterisms from them.
        */
        void detectAsterismsFromEachReseauSubSquareRegion(const std::vector<double>& horizontalLines,
                               const std::vector<double>& verticalLines,
                               const std::vector<double>& imageData2,
                               const std::vector<double>& imageData,
                               int imageHeight,
                               int imageWidth,
                               std::vector<std::vector<double>>& subSquareDataCollection,
                               double dp, double minDist, double param1, double param2, double minRadius, double maxRadius,
                               std::string plateName);



        // Initialize the classes used here
        DBStorageHandler *database;
        BoundingBoxCreator *boundingbox;
};

#endif // ASTERISMSDETECTOR_H
