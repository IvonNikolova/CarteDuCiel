#ifndef BOUNDINGBOXCREATOR_H
#define BOUNDINGBOXCREATOR_H

#include <cmath>
#include <iomanip>

using namespace std;
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

#include "Boundary.h"
using namespace std;



class BoundingBoxCreator
{
public:
    BoundingBoxCreator();

    // Function to calculate and create a bounding box (minimum spanning window) around a set of three points (the circles' centers) with a margin
    cv::Rect calculateBoundingBox(const std::vector<cv::Point>& points, int margin);
    // Calculate the global coordinates relative to the whole astrographic map of each minimum spanning window created that depicts an actual detected asterism
    std::pair<cv::Point, cv::Point> calculateGlobalRectangleCoordinates(
        const cv::Rect& boundingBox, // The rectangle within the sub-image
        const Boundary& subSquare    // The top-left corner of the sub-image within the larger image
    );
    // Crop and save the minimum spanning windows that contain the actual asterisms
    void saveCroppedImage(const std::vector<double>& imageA, const std::pair<cv::Point, cv::Point>& globalCoordinates, const std::string& folderPath, int imageWidth);
};

#endif // BOUNDINGBOXCREATOR_H
