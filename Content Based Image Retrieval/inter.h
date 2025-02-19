/*
    Created by 

    Shirish Kishore Kumar  
    Soorya Vijayaragavan 

    This file contains all the functions used in the inter.cpp program 

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>

void extractValues(std::ifstream &file1, double col_idx, std::vector<double> &column_values);

double SSD(std::vector<double> &featureVector1, std::vector<double> &featureVector2);

double HM(std::vector<double> &featureVector1, std::vector<double> &featureVector2);

double MHM(std::vector<double> &featureVector1, std::vector<double> &featureVector2);

double CMHM(std::vector<double> &featureVector1, std::vector<double> &featureVector2);

double THM(std::vector<double> &featureVector1, std::vector<double> &featureVector2);

template <typename T>
void printVectors(std::vector<std::vector<T>> columns);

void extractFeatures(std::ifstream &file, std::vector<std::vector<double>> &columns, std::map<double, std::string> &map);