//
// Created by SHIRISH and SOORYA on 4/12/23.
//
#include<opencv2/opencv.hpp>

int magnitude(cv::Mat &src, cv::Mat &dst);
int blur5( cv::Mat &src, cv::Mat &dst );
int gray_magnitude( cv::Mat &src, cv::Mat &dst );
int thresholding( cv::Mat &src, cv::Mat &dst );
int nightvision(cv::Mat &src, cv::Mat &dst);
cv::Mat Morphological(const cv::Mat& image);
int configure_serial(const char *port);