/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * Using Humoments, Fill percentage, H/W ratio and mu22alpha
 */

#ifndef CV_TRAININGDATA_H
#define CV_TRAININGDATA_H


#include <opencv2/core/mat.hpp>

#include "feature.h"

class trainingData {
public:
    trainingData(cv::Mat image);
    ~trainingData();
    void extractFeatures();
    double calculateMuAlpha(cv::Mat mat_region, double centroidX, double centroidY, double alpha, cv::Moments moments);
    void setLabel(std::string label);
    feature getFeatures() const;

    double gethwratio() const {
        return features.h_w_ratio;
    }
private:
    cv::Mat region;
    feature features;
    cv::RotatedRect bounding_box;
    cv::RotatedRect _computeBoundingBox(cv::Mat &region, double centroidX, double centroidY, double alpha);
    double _computeFillPercentage() const;
    double _computeHWRatio() const;
};


#endif //CV_TRAININGDATA_H
