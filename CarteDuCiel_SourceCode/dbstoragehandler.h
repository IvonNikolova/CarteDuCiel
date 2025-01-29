#ifndef DBSTORAGEHANDLER_H
#define DBSTORAGEHANDLER_H

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


#include <cmath>
#include <iomanip>
#include "Boundary.h"
using namespace std;

// Define a structure to hold the global coordinates of each rectangle
struct RectangleGlobalCoordinates {
 int number; // Number of the rectangle
 Boundary subSquare;
 cv::Point topLeft; // Top-left corner in the larger image
 cv::Point bottomRight; // Bottom-right corner in the larger image
};

class DBStorageHandler
{
public:
    DBStorageHandler();

    void connectToDatabase();
    void deleteRecordsByPlate(const QString& sourcePlate,  QSqlQuery& query);
    void saveRectanglesToPostgres(const std::vector<RectangleGlobalCoordinates>& rectangles, std::string plate);
};

#endif // DBSTORAGEHANDLER_H
