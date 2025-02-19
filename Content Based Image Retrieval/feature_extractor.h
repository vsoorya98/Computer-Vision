/*
    Created by 

    Shirish Kishore Kumar  
    Soorya Vijayaragavan 

    This file contains all the functions used in the feature extractor.cpp program 

*/

#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <dirent.h>




std::vector<float> histogram3d(cv::Mat &src);
cv::Mat extract_9times9_rgb_values_from_center_of_an_feature_image(cv::Mat &src);
std::vector<float> histogram3d(cv::Mat &src);
template<typename T>
void transpose(std::vector<std::vector<T>> &input2d) ;
void writeToCsv(const std::string &csv_file_path, std::vector<std::vector<std::string>> &histData);
void feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(const std::string &folder_path, const std::string &csv_file_path, char &in);
void extract_9times9_rgb_values_from_center_of_an_target_image(cv::Mat &src);
void target_single_histogram3d(cv::Mat &src);
void target_multi_histogram3d(cv::Mat &src1, cv::Mat &src2 );
void target_triple_histogram3d(cv::Mat &src1, cv::Mat &src2, cv::Mat &src3  );