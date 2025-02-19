#pragma once
#ifndef FILTERS_H
#define FILTERS_H
#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include<cstring>
#include<cstdio>
#include<math.h>

int alt_grey(cv::Mat& src, cv::Mat& dst);
int blur5x5(cv::Mat& src, cv::Mat& dst);
int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);
int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels);
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold);
int cvtnegative(cv::Mat& src, cv::Mat& dst);
int sharp(cv::Mat& src, cv::Mat& dst);
int saturation(cv::Mat& src, cv::Mat& dst, int sv);
#endif // !filter_h
