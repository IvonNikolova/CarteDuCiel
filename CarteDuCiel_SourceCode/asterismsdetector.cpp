#include "asterismsdetector.h"
#include <cmath>
#include <iomanip>

using namespace std;

AsterismsDetector::AsterismsDetector() {}// AsterismsDetector



//-------------------------------------------- RULE-A --------------------------------------------
double AsterismsDetector::calculateIntensityThreshold(
                      const std::vector<double> horizontalLines,
                       const std::vector<double> verticalLines,
                       const std::vector<double> imageData,
                       int imageHeight,
                       int imageWidth,
        std::vector<std::vector<double>> subSquareDataCollection,
        double dp, double minDist, double param1, double param2, double minRadius, double maxRadius)

{
    std::vector<Boundary> subSquareBoundaries;
    double avgThresholdForAGivenSubImage;

    for (int row = 0; row < horizontalLines.size() - 1; ++row)
    {
        for (int col = 0; col < verticalLines.size() - 1; ++col)
        {

            Boundary subSquare;
            subSquare.startCol = verticalLines[col];
            subSquare.startRow = horizontalLines[row];
            subSquare.endCol = verticalLines[col + 1];
            subSquare.endRow = horizontalLines[row + 1];
            subSquareBoundaries.push_back(subSquare);

            std::vector<double> subSquareData;
            for (int subRow = subSquare.startRow; subRow < subSquare.endRow; ++subRow)
            {
                for (int subCol = subSquare.startCol; subCol < subSquare.endCol; ++subCol)
                {
                    double subSquareValue = imageData[subRow * imageWidth + subCol];
                    subSquareData.push_back(subSquareValue);
                }
            }


            subSquareDataCollection.push_back(subSquareData);

            cv::Mat subSquareImage(subSquare.endRow - subSquare.startRow, subSquare.endCol - subSquare.startCol, CV_8UC1);

             int subSquareDataIdx = subSquareDataCollection.size() - 1;
             for (int row = 0; row < subSquare.endRow - subSquare.startRow; ++row)
             {
                 for (int col = 0; col < subSquare.endCol - subSquare.startCol; ++col)
                 {
                   subSquareImage.at<uchar>(row, col) = static_cast<uchar>(subSquareDataCollection[subSquareDataIdx][row * (subSquare.endCol - subSquare.startCol) + col]);

                 }
             }


             cv::GaussianBlur(subSquareImage, subSquareImage, cv::Size(21, 21), 3, 3);
             std::vector<cv::Vec3f> circles;
             cv::HoughCircles(subSquareImage, circles, cv::HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);

             cv::Mat subSquareImageColor;
             cv::cvtColor(subSquareImage, subSquareImageColor, cv::COLOR_GRAY2BGR);

             cv::Mat subSquareImageColor_UsedForIntensity = subSquareImage;
             cv::cvtColor(subSquareImage, subSquareImageColor_UsedForIntensity, cv::COLOR_GRAY2BGR);


            double meanIntensity;
            static int circlesSize;
            long double sumOfMeanIntensities;

            for (const cv::Vec3f& circle : circles)
            {
                cv::Point center(circle[0], circle[1]);
                int radius = cvRound(circle[2]);

                 int roiX = std::max(0, center.x - radius);
                 int roiY = std::max(0, center.y - radius);
                 int roiWidth = std::min(2 * radius, subSquareImageColor_UsedForIntensity.cols - roiX);
                 int roiHeight = std::min(2 * radius, subSquareImageColor_UsedForIntensity.rows - roiY);


                  // Convert the ROI to grayscale
                  cv::Mat circleGray; // the whole image
                  cv::cvtColor(subSquareImageColor_UsedForIntensity, circleGray, cv::COLOR_BGR2GRAY);

                  cv::Rect roiRect(roiX, roiY, roiWidth, roiHeight);
                  cv::Mat circleROI = circleGray(roiRect).clone();  // Make a copy of the ROI

                 meanIntensity = cv::mean(circleROI)[0];


                 // Add the mean intensity to the total
                 sumOfMeanIntensities += meanIntensity;

                 circlesSize++;
            }

            /**
             *  Circle Intensity Threshold (detected in a given sub-square region of the reseau),
             *          The sum of all average intensities of all circles (detected in a given sub-square region of the reseau),
             *          divided by the number of all circles (detected in a given sub-square region of the reseau),
           */
           avgThresholdForAGivenSubImage = sumOfMeanIntensities / circlesSize;
        }
    }
   return avgThresholdForAGivenSubImage;
}//calculateIntensityThreshold


//-------------------------------------------- RULE-B --------------------------------------------
// Calculate the Euclidean distance between two points
// it is employed in the triangle detection logic
// NOTE: it is used within extractSubSquares() function!
double AsterismsDetector::calculateDistance(const cv::Point& p1, const cv::Point& p2)
{
//    double dx = static_cast<double>(p1.x - p2.x);
//    double dy = static_cast<double>(p1.y - p2.y);
//    return std::sqrt(dx * dx + dy * dy);

//or, same as above
    return cv::norm(p1 - p2);// cv::norm function in OpenCV can calculate the Euclidean distance between two points or vectors
}//calculateDistance

// Function to check if two circles are too close based on a fixed distance
bool AsterismsDetector::areTwoCirclesClose(const cv::Vec3f& circle1, const cv::Vec3f& circle2, double distanceThreshold)
{
    // Extract center and radius information for each circle
    cv::Point center1(circle1[0], circle1[1]);
    cv::Point center2(circle2[0], circle2[1]);

    // Calculate the Euclidean distance between the centers of the two circles Circle 1 and Circle 2 to determine proximity.
    double distance = calculateDistance(center1, center2);

    // Check if the calculated distance is <= 12 which is the threshold which best suits our cases
     return (distance <= distanceThreshold);
     // Two possible result cases:
            // return true -> the circles are considered too close, they are part of the same group
            // return false -> distance is equal to or greater than the threshold, the circles are NOT part of the same group
}//areCirclesTooClose

//-------------------------------------------- RULE-D --------------------------------------------

// Check the distance between all three pairs forming the potetial valid triangle group for a given asterism
bool AsterismsDetector::checkDistanceBetweenAll(const cv::Vec3f& circle1, const cv::Vec3f& circle2, const cv::Vec3f& circle3, double distanceThreshold) {
    cv::Point center1(circle1[0], circle1[1]);
    cv::Point center2(circle2[0], circle2[1]);
    cv::Point center3(circle3[0], circle3[1]);

    double distance12 = calculateDistance(center1, center2);
    double distance13 = calculateDistance(center1, center3);
    double distance23 = calculateDistance(center2, center3);

    return (distance12 <= distanceThreshold && distance13 <= distanceThreshold && distance23 <= distanceThreshold);
}//checkDistanceBetweenAll

//-------------------------------------------- RULE-F --------------------------------------------
// Function to calculate the angle between three points
double AsterismsDetector::calculateAngle(const cv::Point& p1, const cv::Point& p2, const cv::Point& p3)
{
    cv::Point v1 = p1 - p2;
    cv::Point v2 = p3 - p2;

    double dotProduct = v1.x * v2.x + v1.y * v2.y;
    double magnitudeV1 = cv::norm(v1);
    double magnitudeV2 = cv::norm(v2);

    return acos(dotProduct / (magnitudeV1 * magnitudeV2)) * (180.0 / CV_PI);
}//calculateAngle



//-------------------------------------------- RULE-G --------------------------------------------
// Function to determine the orientation of a triangle
std::string AsterismsDetector::determineOrientation(cv::Point center1, cv::Point center2, cv::Point center3)
{
    // Sort the centers by y-coordinate
    std::vector<cv::Point> centers = {center1, center2, center3};

    std::sort(centers.begin(), centers.end(), [](const cv::Point& a, const cv::Point& b)
    {
        return a.y < b.y;
    });

    // Calculate y-coordinate differences
    int diff1 = std::abs(centers[2].y - centers[1].y);
    int diff2 = std::abs(centers[2].y - centers[0].y);
    int diff3 = std::abs(centers[1].y - centers[0].y);

    // Determine the points forming the base (least difference)
    cv::Point base1, base2, deterministicPoint;

    if (diff1 <= diff2 && diff1 <= diff3) {
        base1 = centers[1];
        base2 = centers[2];
        deterministicPoint = centers[0];
    } else if (diff2 <= diff1 && diff2 <= diff3) {
        base1 = centers[0];
        base2 = centers[2];
        deterministicPoint = centers[1];
    } else {
        base1 = centers[0];
        base2 = centers[1];
        deterministicPoint = centers[2];
    }

    // Determine orientation based on the y-coordinates
    if (deterministicPoint.y > base1.y && deterministicPoint.y > base2.y)
    {
        return "Downward";
    }
    else if (deterministicPoint.y < base1.y && deterministicPoint.y < base2.y)
    {
        return "Upward";
    }
    else
    {
        return "Undefined";
    }
}//determineOrientation





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ASTERISM DETECTOR++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// The combined function that does the following things:
// extract all 676 sub-squres using the vertical and horizontal projections of the image,
// then apply Gaussian Filter and Hough Circle Transform on each sub-image,
// Draw the circles and their centers once they are detected.
// Apply a set of seven rules (A -- G) to verify the appearance a valid asterism group.
// Draw minimum spanning windows (bounding boxes) to confirm the detected asterism group.
// Connect to database and save the records.
// Save the sub-square segmented regions of the reseau.
// Save each of the cropped minimum spanning windows of the reseau.
// Save in a txt files also the data is going to be saved in the db.
void AsterismsDetector::detectAsterismsFromEachReseauSubSquareRegion(
                             const std::vector<double>& horizontalLines,
                             const std::vector<double>& verticalLines,
                             const std::vector<double>& imageData2, //raw image data
                             const std::vector<double>& imageData, //sobel applied already on the image data
                             int imageHeight,
                             int imageWidth,
                             std::vector<std::vector<double>>& subSquareDataCollection,
                             double dp, double minDist, double param1, double param2, double minRadius, double maxRadius,
        std::string plateName)
{
    // ++++++++++++++++++++++++++++++++++++++ Directories for saving outputs from the project
        const char* homeDir = getenv("HOME");
        QString desktopPath = QString(homeDir) + "/Desktop";
        QString folderPath;

    // ++++++++++++++++++++++++++++++++++++++ DB connection
    database->connectToDatabase();


    // ++++++++++++++++++++++++++++++++++++++ Check the intensity levels of all detected circles' contents (RULE-A) when a potential neighbour appears
    double avgThresholdForAGivenSubImage = calculateIntensityThreshold(horizontalLines,verticalLines,imageData,imageHeight,imageWidth,subSquareDataCollection,dp,  minDist,  param1,  param2,  minRadius,  maxRadius);



    // The boundaries of each sub-image of the reseua (image-B)
    /*
        This vector is used to store the boundaries of each sub-square.
        Each sub-square is defined by its starting column and row (startCol, startRow)
        and its ending column and row (endCol, endRow)
     */
    std::vector<Boundary> subSquareBoundaries;
    // The concrete sub-image for the given iteration
    int numberOfSubImage = 0;
    // All rectangles
    static std::vector<RectangleGlobalCoordinates> allRectangles; // all minimum spanning windows metadata

    int finalNumberOfAllRectangles = 0;

/*
    This nested loop iterates over the horizontal and vertical lines,
    creating sub-squares for each intersection.

    For each sub-square:
        1. It defines a Boundary struct (subSquare) representing the sub-square's boundaries
        using the current row and column indices.

        2. It extracts the pixel data within the sub-square from the imageData
        and stores it in a std::vector<double> named subSquareData.

        3. It adds the subSquareData vector to subSquareDataCollection.

        4. It creates a grayscale sub-square image (subSquareImage) using OpenCV's cv::Mat class.
*/
for (int row = 0; row < horizontalLines.size() - 1; ++row)
{
    for (int col = 0; col < verticalLines.size() - 1; ++col)
    {

numberOfSubImage++;
if(row == 0 && col == 0){
    numberOfSubImage = 0;
}
        // 1.
            Boundary subSquare;
            subSquare.startCol = verticalLines[col]; // X-s
            subSquare.startRow = horizontalLines[row]; // Y-s
            subSquare.endCol = verticalLines[col + 1];
            subSquare.endRow = horizontalLines[row + 1];
            subSquareBoundaries.push_back(subSquare);

        // 2.
            // Store sub-square data (each pixel data within those boundaries) in a vector
            // i.e. constructs and stores the data of each sub-square
            /*
                This section is responsible for constructing a vector subSquareData
                that holds the data values of the current sub-square being processed.

                It iterates through the rows and columns of the sub-square,
                extracts the corresponding values from the imageData,
                and pushes them into the subSquareData vector.

                After constructing this vector,
                it's added to the subSquare                                                                                                                       eDataCollection,
                which will hold the data for all sub-squares.
            */
            std::vector<double> subSquareData;
            for (int subRow = subSquare.startRow; subRow < subSquare.endRow; ++subRow)
            {
                for (int subCol = subSquare.startCol; subCol < subSquare.endCol; ++subCol)
                {
                    double subSquareValue = imageData[subRow * imageWidth + subCol];
                    subSquareData.push_back(subSquareValue);
                }
            }

            // 3.
            subSquareDataCollection.push_back(subSquareData);


            // 4.
            /*
                The nested loops after subSquareDataCollection.push_back(subSquareData);
                are used to populate the subSquareImage with pixel values from subSquareData.
                It iterates over each pixel in the sub-square and assigns the corresponding value
                from subSquareData to the subSquareImage Mat.
            */
                // Create a 1-channel image for the sub-square
                    cv::Mat subSquareImage(subSquare.endRow - subSquare.startRow, subSquare.endCol - subSquare.startCol, CV_8UC1);

                     int subSquareDataIdx = subSquareDataCollection.size() - 1;
                     for (int row = 0; row < subSquare.endRow - subSquare.startRow; ++row)
                     {
                         for (int col = 0; col < subSquare.endCol - subSquare.startCol; ++col)
                         {
                           subSquareImage.at<uchar>(row, col) = static_cast<uchar>(subSquareDataCollection[subSquareDataIdx][row * (subSquare.endCol - subSquare.startCol) + col]);

                         }
                     }

           //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ GAUSSIAN Filter + CHT

                    // Apply Gaussian blur to the sub-square image
                     // the bigger the filter kernel size, the smoother the sub-image data is -> could lead to deletion of some circle data,
                     // NOTE: If the parameters in calculateIntensityThreshold and here have to be the same!
                     cv::GaussianBlur(subSquareImage, subSquareImage, cv::Size(21, 21), 3, 3); // Gaussian kernel size and standard deviation

                     // Perform Hough Circle Transform on this sub-square
                     std::vector<cv::Vec3f> circles;

                     cv::HoughCircles(subSquareImage, circles, cv::HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius); //dp, param1, param2 ?

                    // Create a color version of the sub-square image: subSquareImage
                    cv::Mat subSquareImageColor;
                    cv::cvtColor(subSquareImage, subSquareImageColor, cv::COLOR_GRAY2BGR);

                    cv::Mat subSquareImageColor_UsedForIntensity = subSquareImage;
                    cv::cvtColor(subSquareImage, subSquareImageColor_UsedForIntensity, cv::COLOR_GRAY2BGR);


                    // Define a vector to store intensityUnderThr flags for each circle
                    std::vector<bool> intensityUnderThr(circles.size(), false);
                    static int numbOfCirles = 0;

                    for (int i = 0; i < circles.size(); ++i)
                    {

                        // Get the center and radius of the current circle
                            cv::Point center(circles[i][0], circles[i][1]);
                            int radius = cvRound(circles[i][2]);

                        // Log circle information
                            // std::cout << "\nCircle: Center(" << center.x << ", " << center.y << "), Radius: " << radius << std::endl;

                        // Ensure ROI boundaries are within the image bounds
                        int roiX = std::max(0, center.x - radius);
                        int roiY = std::max(0, center.y - radius);
                        int roiWidth = std::min(2 * radius, subSquareImageColor_UsedForIntensity.cols - roiX);
                        int roiHeight = std::min(2 * radius, subSquareImageColor_UsedForIntensity.rows - roiY);

                        // Convert the ROI to grayscale
                         cv::Mat circleGray; // the whole image
                         cv::cvtColor(subSquareImageColor_UsedForIntensity, circleGray, cv::COLOR_BGR2GRAY);

                         cv::Rect roiRect(roiX, roiY, roiWidth, roiHeight);
                         cv::Mat circleROI = circleGray(roiRect).clone();  // Make a copy of the ROI



                    // Calculate the mean intensity of the pixels within the circle ROI
                    /*
                        The cv::mean function directly computes the mean intensity of the pixels
                        within the region of interest (ROI).

                        The sum of the intensity of all pixels in the circle, divided by the number of pixels in the circle.
                     */
                         // Calculate the mean intensity for a given circle
                         double meanIntensity = cv::mean(circleROI)[0];

                         // Increase the number per each iteration
                         numbOfCirles++;


                        // Determine (CASE I) or (CASE II)
                        if (meanIntensity <= avgThresholdForAGivenSubImage) // going more to black - background, beacuse of Sobel edge detection filter interpretation (CASE I)
                        {
                            // std::cout << "Coloring circle with BLACK - RED" << std::endl;
                            cv::Scalar darkRed(0, 0, 100); // Dark Red color (BGR: 0, 0, 100)
                            cv::circle(subSquareImageColor, center, radius, darkRed, 2);
                        }
                        else // meanIntensity > avgThresholdForAGivenSubImage // going more to white - object  (CASE II)
                        {
                           // std::cout << "Coloring circle with WHITE - GREEN" << std::endl;
                            cv::Scalar darkGreen(0, 100, 0); // Dark Green color (BGR: 0, 100, 0)
                            cv::circle(subSquareImageColor, center, radius, darkGreen, 2);
                        }
                        // Determine if mean intensity is under and set the flag accordingly
                           bool intensityUnder = (meanIntensity <= avgThresholdForAGivenSubImage);
                           // Store the flag in the intensityUnder vector
                           intensityUnderThr[i] = intensityUnder;
                    }

    /*
        The distanceThreshold is a parameter that we can adjust based on the characteristics of the images.

        It determines how close circles need to be to be considered part of the same group.

        If the circles are closer than this threshold,
        they are more likely to be part of the same cluster.
    */
    double distanceThreshold = 12; //the distance that makes a circles from one group is if their centers are in distance less than 10 pixels
    double angleThreshold = 90.0; // Adjust the threshold for excluding obtuse-angled triangles

    // Keep track of circles that have already been used
    std::vector<bool> circleUsed(circles.size(), false);

    // The number of the min.spanning window
    int totalNumberOfRectangles = 0;
    // Check the curr rectangle circles' intensities
    int rectanglesWithAllCirclesUnderThr = 0;




//+++++++++++++++++++++++++= Minnimum Spanning Windows
/*
    In OpenCV, the `cv::HoughCircles` function detects circles in an image.
    It returns a vector of circles detected,
    where each circle is represented by a vector of three elements: `(x, y, radius)`.

    In your loop, `circles[i][2]` represents the radius of the `i`-th detected circle.

    Here's a breakdown:

    - `circles[i][0]` represents the x-coordinate of the center of the `i`-th circle.
    - `circles[i][1]` represents the y-coordinate of the center of the `i`-th circle.
    - `circles[i][2]` represents the radius of the `i`-th circle.

    So, `center` is defined as a `cv::Point` with `(x, y)` coordinates of the center of the circle,
    and `radius` is the radius of the circle.
*/
// Check where we can draw triangles
for (int i = 0; i < circles.size(); ++i)
{
        cv::Point center(circles[i][0], circles[i][1]);

        if (!circleUsed[i]) // To not be used already
        {
            // Create a vector to store the current group of circles
            std::vector<int> currentGroup;
            currentGroup.push_back(i);

            std::vector<cv::Point> currentGroupBorders;

            // Check for other circles in the group
            for (int j = i + 1; j < circles.size(); ++j)
            {
                 cv::Point center(circles[j][0], circles[j][1]);
                if (!circleUsed[j] && areTwoCirclesClose(circles[i], circles[j], distanceThreshold))
                {
                    currentGroup.push_back(j);
                }
            }

//-------------------------------------------- RULE-C --------------------------------------------
            if  (currentGroup.size() == 3)
            {
                for (int idx : currentGroup)
                {
                    circleUsed[idx] = true;

                    cv::Point center(circles[idx][0], circles[idx][1]);
                    int radius = circles[idx][2];
                    cv::circle(subSquareImageColor, center, radius, cv::Scalar(0, 255, 255), -1); // Draw the circle in yellow
                }

                // Log the coordinates of potential group members
                //                std::cout << "Potential group members:";
                //                for (int idx : currentGroup) {
                //                    std::cout << " Circle " << idx << " at coordinates (" << circles[idx][0] << ", " << circles[idx][1] << ")";
                //                }
                //                std::cout << std::endl;

// ++++++++++++++++++++++++  Check if any circle in the group has a neighbor outside the group
                bool hasNeighborOutsideGroup = false;
                for (int idx : currentGroup)
                {
                    for (int otherIdx = 0; otherIdx < circles.size(); ++otherIdx)
                    {
                        // Exclude the circles already in the potential group - not good because this could lead to not identifing a neighbour/ because we do not check the actual circle that could form a group if it has a neighbour/s
                        //if (!circleUsed[otherIdx] && std::find(currentGroup.begin(), currentGroup.end(), otherIdx) == currentGroup.end())
                        if (std::find(currentGroup.begin(), currentGroup.end(), otherIdx) == currentGroup.end())
                        {
                            // Check if the circle is close enough to be considered a neighbor
                            if(areTwoCirclesClose(circles[idx], circles[otherIdx], distanceThreshold))
                            {

// ----------------------------- WAY 1: Observing the intensity of a potential neighbour
                              /*
                                 *********** Check the intensity of a given potential neighbor - circle ***********
                                 IMPORTANT: What is the intensity of a given potential neighbor ?
                                    a - if its intensity refers more to an object => neighbor (GREEN) => break
                                    b - if its intensity refers more to a background => not a neighbor (RED), but we continue to search for neighbors i.e. GREEN circles, near the potential group
                                 TO THINK ABOUT and to know how code behaves:
                                    Q: What do we do if we have 3 circles - a potential asterism group and someone of them has a potential neighbor with intensity being more as a background (RED) ?
                                    ==> Answer: Well,  we do NOT consider it as a neighbor, but as a background! Thus, it doesn't interfere to form a minimum spanning shindow - an asterism!
                               */
                                                    // Retrieve the intensity flag for the partner circle (otherId)
                                                    bool partnerIntensityFlag = intensityUnderThr[otherIdx];

                                                    // If partnerIntensityFlag is true -> a circle is in RED closer to background than to an object, this is NOT A NEIGHBOUR
                                                    if (partnerIntensityFlag != false) {
                                                        // If the neighbor is not part of the current group
                                                          // consider it a neighbor outside the group
                                                        hasNeighborOutsideGroup = false;
                                                        // Mark the neighbor as used
                                                          circleUsed[otherIdx] = true;

                                                          cv::Point centerNeighbor(circles[otherIdx][0], circles[otherIdx][1]);
                                                        //  std::cout << "centerNeighbor" << centerNeighbor << "IS IT USED? " << (circleUsed[otherIdx] == true ? "true" : "false") << "hasNeighborOutsideGroup = " << false << endl;
                                                    }
                                                    else // Else if partnerIntensityFlag is false -> a circle is in GREEN closer to an object than to a background, this is A NEIGHBOUR
                                                    {
                                                        // If the neighbor is not part of the current group
                                                          // consider it a neighbor outside the group
                                                        hasNeighborOutsideGroup = true;
                                                       // Mark the neighbor as used
                                                        circleUsed[otherIdx] = true;

                                                        // Draw the neighbor in light pink
                                                        cv::Point centerNeighbor(circles[otherIdx][0], circles[otherIdx][1]);
                                                        int radiusNeighbor = circles[otherIdx][2];
                                                        cv::circle(subSquareImageColor, centerNeighbor, radiusNeighbor, cv::Scalar(255, 192, 203), -1); // Draw the neighbor in  light pink

                                                        // Break the loop as we found a neighbor outside the group
                                                        break; // breaks the outer for loop
                                                    }



// // ----------------------------- WAY 0: NOT observing the intensity
//                                                    // If the neighbor is not part of the current group
//                                                      // consider it a neighbor outside the group
//                                                    hasNeighborOutsideGroup = true;
//                                                   // Mark the neighbor as used
//                                                    circleUsed[otherIdx] = true;

//                                                    // Draw the neighbor in light pink
//                                                    cv::Point centerNeighbor(circles[otherIdx][0], circles[otherIdx][1]);
//                                                    int radiusNeighbor = circles[otherIdx][2];
//                                                    cv::circle(subSquareImageColor, centerNeighbor, radiusNeighbor, cv::Scalar(255, 192, 203), -1); // Draw the neighbor in light pink

//                                                    // Break the loop as we found a neighbor outside the group
//                                                    break;
                            }
                        }
                    }
                    // If we find out a neighbor outside the group, mark the group as invalid
                    if (hasNeighborOutsideGroup)
                    {
                        break;
                    }
                }//for

               if (!hasNeighborOutsideGroup)
               {

/*
Bounding box
A bounding box is a rectangular box that surrounds an object or region of interest within an image. It is defined by its top-left corner coordinates (x, y), its width, and its height. Bounding boxes are commonly used in computer vision and image processing tasks for object detection, localization, and segmentation.
*/

                   // Detected circles data
                   cv::Point center1(circles[currentGroup[0]][0], circles[currentGroup[0]][1]);
                   cv::Point center2(circles[currentGroup[1]][0], circles[currentGroup[1]][1]);
                   cv::Point center3(circles[currentGroup[2]][0], circles[currentGroup[2]][1]);


// ++++++++++++++++++++++++ Checks whether three circles can form a valid triangle i.e. whether pairs of circles are too close to each other in distance <=12
                   if (checkDistanceBetweenAll(circles[currentGroup[0]], circles[currentGroup[1]], circles[currentGroup[2]], distanceThreshold))
                   {
// --------------------------------------- RULE-E : Triangle inequality theorem ---------------------------------------
                       // Distance calculations
                       double dist1 = cv::norm(center1 - center2);
                       double dist2 = cv::norm(center1 - center3);
                       double dist3 = cv::norm(center2 - center3);
                       // Check if distances satisfy triangle inequality
                       if (dist1 < dist2 + dist3 && dist2 < dist1 + dist3 && dist3 < dist1 + dist2)
                       {

                           // Define the angle of the formed triangle
                           double angle1 = calculateAngle(center1, center2, center3);
                           double angle2 = calculateAngle(center2, center3, center1);
                           double angle3 = calculateAngle(center3, center1, center2);

// ++++++++++++++++++++++++ Check if all angles are less than or equal to the threshold i.e., not obtuse-angled
                           if (angle1 <= angleThreshold && angle2 <= angleThreshold && angle3 <= angleThreshold)
                           {
                                for (int l = 0; l < currentGroup.size(); ++l)
                                {
                                    circleUsed[currentGroup[l]] = true;
                                }
// ++++++++++++++++++++++++ Determine the triangle orientation

                                std::string orientation = determineOrientation(center1, center2, center3);
                                if(orientation == "Downward")
                                {
                                     //std::cout << "Triangle orientation: " << orientation << std::endl;
                                    // Draw the edges of the triangle - GREEN for VALID triangle
                                    cv::line(subSquareImageColor, center1, center2, cv::Scalar(0, 255, 0), 1);
                                    cv::line(subSquareImageColor, center2, center3,  cv::Scalar(0, 255, 0), 1);
                                    cv::line(subSquareImageColor, center3, center1, cv::Scalar(0, 255, 0), 1);
                                }
                                else if (orientation == "Upward")
                                {
                                    //std::cout << "Triangle orientation: " << orientation << std::endl;
                                    cv::line(subSquareImageColor, center1, center2, cv::Scalar(0, 0, 255), 1);
                                    cv::line(subSquareImageColor, center2, center3,  cv::Scalar(0, 0, 255), 1);
                                    cv::line(subSquareImageColor, center3, center1, cv::Scalar(0, 0, 255), 1);
                                }
                                else
                                {
                                    //std::cout << "Triangle orientation: " << orientation << std::endl;
                                    cv::line(subSquareImageColor, center1, center2, cv::Scalar(0, 0, 255), 1);
                                    cv::line(subSquareImageColor, center2, center3,  cv::Scalar(0, 0, 255), 1);
                                    cv::line(subSquareImageColor, center3, center1, cv::Scalar(0, 0, 255), 1);
                                }


// ++++++++++++++++++++++++ Draw the SQUARE GRID around the trinagle (asterism)
// ++++++++++++++++++++++++ DRAW ORANGE SQUARE AROUND THE CIRCLES BORDER + additional margin of 5 pixels to each of the sides
                               // Collect the borders of the circles in the group
                               for (int l = 0; l < currentGroup.size(); ++l)
                               {
                                   cv::Point center(circles[currentGroup[l]][0], circles[currentGroup[l]][1]);
                                   int radius = circles[currentGroup[l]][2];
                                   // Collect the borders by creating a square around each circle
                                   currentGroupBorders.push_back(cv::Point(center.x - radius, center.y - radius));
                                   currentGroupBorders.push_back(cv::Point(center.x + radius, center.y + radius));
                               }
                               // Draw the edges of the square around the bounding box of the circle borders
                               cv::Rect boundingBox = boundingbox->calculateBoundingBox(currentGroupBorders, 5);
                               cv::rectangle(subSquareImageColor, boundingBox, cv::Scalar(0, 255, 0), 1); // Draw in green

// ++++++++++++++++++++++++ Calculate rectangle ie.. min.spanning windows cooridnates in image-A

                               // Extracting details from the bounding box
                               cv::Point topLeft(boundingBox.x, boundingBox.y);
                               cv::Point bottomRight(boundingBox.x + boundingBox.width, boundingBox.y + boundingBox.height);

                               // Calculate the global coordinates
                               std::pair<cv::Point, cv::Point> globalCoordinates = boundingbox->calculateGlobalRectangleCoordinates(
                                   boundingBox, // The rectangle within the sub-image
                                   subSquare    // The top-left corner of the sub-image within the larger image
                               );
                                                               // Print the global coordinates
                                //                               std::cout << "Top-left corner in the larger image: (" << globalCoordinates.first.x << ", " << globalCoordinates.first.y << ")\n";
                                //                               std::cout << "Bottom-right corner in the larger image: (" << globalCoordinates.second.x << ", " << globalCoordinates.second.y << ")\n";


                                                               //                               std::cout << "Sub-image (" << subSquare.startCol << "," << subSquare.startRow << "), "
                                                               //                                            << "(" << subSquare.endCol << "," << subSquare.endRow << "): " << std::endl;

                               static int numb = 0;
                               allRectangles.push_back({
                                   ++numb, // Rectangle number (1-based index)
                                   subSquare, //subsquare
                                   globalCoordinates.first, // Top-left corner in the larger image
                                   globalCoordinates.second // Bottom-right corner in the larger image
                               });


// ++++++++++++++++++++++++++++++ Save the cropped images of all minimum spanning windows (bounding boxes) depicting detected valid asterisms from the astrographic map's reseau
                                 folderPath = desktopPath + "/CarteDuCiel_Outputs/minSpanningWindows";
                              // !!!!!!!!!ATTENTION!!!!!!!!!: Uncomment the below line to save them:
                              //boundingbox->saveCroppedImage(imageData2, globalCoordinates, folderPath.toStdString(), imageWidth);




                               // Check if the bounding box is valid (non-zero width and height)
                               if (boundingBox.width > 0 && boundingBox.height > 0)
                               {
                                   // Increment the counter for the number of created boxes
// ++++++++++++++++++++++++ Know the number of rectangles
                                   totalNumberOfRectangles++; // each drawn rectangle
                                    rectanglesWithAllCirclesUnderThr++;
                                   // Check if all circles in the current group have intensityUnderThr flag as false
                                    bool oneCircleUnderThr = false;
                                   for (int l = 0; l < currentGroup.size(); ++l) {
                                       //std::cout << "**********intensityUnderThr  = " << intensityUnderThr[currentGroup[l]] << " for circle: " << currentGroup[l] << std::endl;

                                       // if the intensity of the current circle is true
                                       if (intensityUnderThr[currentGroup[l]] != false) {
                                           oneCircleUnderThr = true;
                                           break;
                                       }

                                  }
                                  // If all circles have intensityUnderThr flag as false,
                                   // decrement the counter
                                       if (oneCircleUnderThr) {
                                           rectanglesWithAllCirclesUnderThr -= 1;
                                           oneCircleUnderThr = false;
                                       }
                                }



                          }//if - not obtused angle
                          else
                          {
                               for (int l = 0; l < 3; ++l)
                               {
                                   //circleUsed[currentGroup[l]] = true;
                                   cv::Point center(circles[currentGroup[l]][0], circles[currentGroup[l]][1]);
                                   int radius = circles[currentGroup[l]][2];

                                   cv::circle(subSquareImageColor, center, radius, cv::Scalar(0, 165, 255), -1); // ORANGE
                               }
                            }
                       }// if - triangle inequality theorem
                   }
                   else
                   {
                       // Exclude the entire group because not all pairs have same distance of <= 12 i.e.could not form a triangle
                       for (int l = 0; l < 3; ++l)
                       {
                           circleUsed[currentGroup[l]] = true;
                           cv::Point center(circles[currentGroup[l]][0], circles[currentGroup[l]][1]);
                           int radius = circles[currentGroup[l]][2];

                           cv::circle(subSquareImageColor, center, radius, cv::Scalar(255, 165, 0), -1); // Light blue
                       }
                   }
                }
                else if (hasNeighborOutsideGroup)
                {
                    // If it has a neighbour outside the group,
                    // we color THE WHOLE GROUP except the neighbout in dark pink
                   for (int l = 0; l < currentGroup.size(); ++l)
                   {
                       circleUsed[currentGroup[l]] = true;

                       cv::Point center(circles[currentGroup[l]][0], circles[currentGroup[l]][1]);
                       int radius = circles[currentGroup[l]][2];

                      cv::circle(subSquareImageColor, center, radius, cv::Scalar(255, 105, 180), -1);// Dark pink
                    }
                }
            }// else more or less than 3 circles
            else
            {
                // Colour all candidates in dark blue
                for (int l = 0; l < currentGroup.size(); ++l)
                {
                    cv::Point center(circles[currentGroup[l]][0], circles[currentGroup[l]][1]);
                    int radius = circles[currentGroup[l]][2];
                    cv::circle(subSquareImageColor, center, radius, cv::Scalar(255, 0, 0), -1); // DARK BLUE
                }
            }//else
          }// if (!circleUsed[i])
    }//for(int i ....circle.size()...
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Some Research-Logs that show number of detected asterisms if we do and do not observe their contents intensities (RULE-A) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        std::string allRectResults = std::to_string(totalNumberOfRectangles);
//        //std::string allRectResults = (totalNumberOfRectangles == 0 ? "-" : std::to_string(totalNumberOfRectangles));
//        std::string withIntensityRes = (allRectResults.find("-") != std::string::npos ? "-" :  std::to_string(rectanglesWithAllCirclesUnderThr));
//        // std::cout << "Sub-image:" << numberOfSubImage
//        //          << "| no Intensity: " << allRectResults
//        //          << "| with Intensity: " << withIntensityRes << std::endl;
//        std::cout << allRectResults << ","; // all created bounding boxes + the taken intensity in hasNeighbour се проверява -  Way 1
//        finalNumberOfAllRectangles += totalNumberOfRectangles; // all asterism that were detected with intensity of their contents (RULE-A) taken in mind
//        totalNumberOfRectangles = 0;
//        rectanglesWithAllCirclesUnderThr = 0;




// ++++++++++++++++++++++++++++++++++++++ Create and save each sub-image within the Image-B (i.e. the reseau)

                      // Save all sub-square regions with all detected circles
                    folderPath  = desktopPath + "/CarteDuCiel_Outputs/AllSubSquareRegionsWithDetectedAsterisms";
                    QDir().mkpath(folderPath);
                    std::string subSquareImageName = plateName +"_subsquare_" + std::to_string(subSquareDataIdx) + ".jpg";
                    QString fullPath = QDir(folderPath).filePath(QString::fromStdString(subSquareImageName));

                    //  !!!!!!!!!ATTENTION!!!!!!!!!: Uncomment the below comment to save each sub-image in a local folder on Desktop:
                    // cv::imwrite(fullPath.toStdString(), subSquareImageColor);
    }
}// End of the iteration, all sub-square segmented regions of the reseau were iterated and all possible asterisms were verified as such and detected.


//  ++++++++++++++++++++++++++++++++++++++ DB connection
              //  !!!!!!!!!ATTENTION!!!!!!!!!: Uncomment the below comment to save each sub-image in the local database
//             database->saveRectanglesToPostgres(allRectangles, plateName);
                    // //std::cout << "All asterisms detected and written in the database:" << finalNumberOfAllRectangles << std::endl;

//  ++++++++++++++++++++++++++++++++++++++ Save the boundaries of the sub-squares segmented regions of the reseu in a file, not only in the db
        folderPath  = desktopPath + "/CarteDuCiel_Outputs";
        QDir dir(folderPath);
        QString fullPath1 = QDir(folderPath).filePath("allSegmentedSubSquaresOfReseau_coordinates.txt");

        // Open the file for writing
        ofstream file(fullPath1.toStdString());


        // Write the vector data to the file
        for (int i = 0; i < subSquareBoundaries.size(); ++i)
        {
            const Boundary& subSquare = subSquareBoundaries[i];
                    file <<
                            "Sub-Square " << i + 1 << " boundaries:\n";
                    file <<
                            "Start: (" << subSquare.startCol << ", " << subSquare.startRow << ")\n";
                    file <<
                            "End: (" << subSquare.endCol << ", " << subSquare.endRow << ")\n";
                         file << "\n";
        }
        // Close the file
        file.close();
//  ++++++++++++++++++++++++++++++++++++++ Save all min.spanning windows data in a file, not only in the db
        QString fullPath2 = QDir(folderPath).filePath("allMinimumSpanningWindows_coordinates.txt");

        // Open the file for writing
        ofstream outFile(fullPath2.toStdString());


        // Write the results to the file
        for (const auto& rect : allRectangles) {
           outFile << "Rectangle " << rect.number << ": " << "Top-left corner (" << rect.topLeft.x << ", " << rect.topLeft.y << "), " << "Bottom-right corner (" << rect.bottomRight.x << ", " << rect.bottomRight.y << ")\n";
        }

        // Close the file
        outFile.close();
}//extractAsterismsFromEachReseauSubSquareRegion





/*
                Transfoming the cooridnates of rectangle drawn in the sub-images of image-B
                                to the original coordinates within the image-A

    first * corresponds to starting point of image-A
    |  image A
    *_______________
    |               | ---> offset between image-A and image-B
    |               | image B
    |  *_________   | first * corresponds to starting point of image-B which is calculated from the 1st. vals from vertical and horizontal projections
    |  | x |x |x|   |
    |   --------    | ---> offsets between image-B and its sub-images
    |  | x |x |X|   |
    |   --------*   | last * corresponds to end point of image-B which is calculated from last vals from vertical and horizontal projections
    |               |
    |_______________*
                    |
                    last * corresponds to end point of image-A

*/




