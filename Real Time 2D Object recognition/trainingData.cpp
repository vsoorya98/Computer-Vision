/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * Using Humoments, Fill percentage, H/W ratio and mu22alpha
 */


#include <utility>
#include <opencv2/opencv.hpp>
#include "trainingData.h"

trainingData::trainingData(cv::Mat region_i)
{
    this->region = std::move(region_i);
}

// This function calculates the mu22alpha
double trainingData::calculateMuAlpha(
        cv::Mat mat_region,
        double centroidX,
        double centroidY,
        double alpha,
        cv::Moments moments)
{
    double beta = alpha + 1.57;
    double sum = 0.0;
    for (int y = 0; y < mat_region.rows; y++) {
        for (int x = 0; x < mat_region.cols; x++) {
            if (mat_region.at<bool>(y, x)) { // only consider non-zero pixels
                double yDiff = y - centroidY;
                double xDiff = x - centroidX;
                double value = yDiff * cos(beta) + xDiff * sin(beta);
                sum += value * value;
            }
        }
    }
    return sum / moments.m00;
}

// Reference
// https://stackoverflow.com/questions/40404031/drawing-bounding-box-for-a-rotated-object
// https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html

//cv::RotatedRect trainingData::_computeBoundingBox(cv::Mat &region, double centroidX, double centroidY, double alpha)
//{
//    std::vector<std::vector<cv::Point>> contours;
//    cv::findContours(region, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//    cv::RotatedRect rect = cv::minAreaRect(contours[0]);
//    return rect;
//}

//cv::RotatedRect trainingData::_computeBoundingBox(cv::Mat &region, double centroidX, double centroidY, double alpha)
//{
//    // Find the contours of the region
//    std::vector<std::vector<cv::Point>> contours;
//    cv::findContours(region, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//
//    // Fit a line to the contour points
//    cv::Vec4f line_params;
//    cv::fitLine(contours[0], line_params, cv::DIST_L2, 0, 0.01, 0.01);
//
//    // Compute the center and orientation of the bounding box
//    cv::Point2f line_pt(line_params[2], line_params[3]);
//    cv::Point2f line_dir(line_params[0], line_params[1]);
//    cv::Point2f perp_dir(-line_dir.y, line_dir.x);
//    cv::Point2f center = line_pt + 0.5 * perp_dir;
//    double angle = std::atan2(line_dir.y, line_dir.x) * 180 / CV_PI;
//
//    // Compute the dimensions of the bounding box
//    cv::Rect bounding_rect = cv::boundingRect(contours[0]);
//    double width = bounding_rect.width;
//    double height = bounding_rect.height;
//
//    // Construct and return the bounding box
//    cv::RotatedRect rect(center, cv::Size2f(width, height), angle + alpha);
//    return rect;
//}

cv::RotatedRect trainingData::_computeBoundingBox(cv::Mat &region, double centroidX, double centroidY, double alpha)
{
    // Approximate the contour with a simpler polygon
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(region, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    double epsilon = 0.1 * cv::arcLength(contours[0], true);
    std::vector<cv::Point> approx;
    cv::approxPolyDP(contours[0], approx, epsilon, true);

    // Find the bounding box of the approximated polygon
    cv::RotatedRect rect = cv::minAreaRect(approx);

    return rect;
}


// This function checks if the pixel is in the bounding box
bool isPixelInBoundingBox(cv::RotatedRect rect, int x, int y) {
    cv::Point2f vertices[4];
    rect.points(vertices);
    for (int i = 0; i < 4; i++) {
        cv::Point2f p1 = vertices[i];
        cv::Point2f p2 = vertices[(i + 1) % 4];
        double a = p1.y - p2.y;
        double b = p2.x - p1.x;
        double c = p1.x * p2.y - p2.x * p1.y;
        if (a * x + b * y + c < 0) {
            return false;
        }
    }
    return true;
}
// Computes the fill percentage in the bounding box
double trainingData::_computeFillPercentage() const {
    double area = this->bounding_box.size.area();
    if (area <= 0) {
        return 0;
    }

    int count = 0;
    for (int y = 0; y < this->region.rows; y++) {
        for (int x = 0; x < this->region.cols; x++) {
            if (this->region.at<bool>(y, x))
            {
                if (isPixelInBoundingBox(this->bounding_box, x, y)) {
                    count++;
                }
            }
        }
    }
    return count / area;
}

// Extracts all the 10 features
void trainingData::extractFeatures()
{
    cv::Moments momentValue = moments(region, true);

    double hu[7];
    cv::HuMoments(momentValue, hu);

    double centroidX = int(momentValue.m10 / momentValue.m00);
    double centroidY = int(momentValue.m01 / momentValue.m00);
    double alpha = 0.5* atan2(2 * momentValue.mu11, momentValue.mu20 - momentValue.mu02);

    double mu22alpha = calculateMuAlpha(region, centroidX, centroidY, alpha, momentValue);

    this->bounding_box = _computeBoundingBox(region, centroidX, centroidY, alpha);

    double fillPercentage = this->_computeFillPercentage();
    //std::cout<< fillPercentage << std::endl;
    double heightWidthRatio = this->_computeHWRatio();

    this->features = feature(hu, mu22alpha, fillPercentage, heightWidthRatio);
}

double trainingData::_computeHWRatio() const
{
    double  var1 = this->bounding_box.size.height;
    double  var2 = this->bounding_box.size.width;
    // considering the value with highest value as height
    if (var1 < 20) var1 = 20;
    if (var2 < 20) var2 = 20;
    if (var2 > var1){
        return var2 / var1;
    }
    else{
        return var1 / var2;
    }
}

void trainingData::setLabel(std::string label)
{
    this->features.setLabel(std::move(label));
}

feature trainingData::getFeatures() const
{
    return this->features;
}

trainingData::~trainingData() = default;
