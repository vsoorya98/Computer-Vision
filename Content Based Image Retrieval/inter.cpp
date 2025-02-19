/*
    Created by 

    Shirish Kishore Kumar  
    Soorya Vijayaragavan

    This program files calulates the distance between target and the images from the database
    It performs calculations such as sum of squared difference and histogram intersection


*/



#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>
#include "inter.h"





// This function extracts the feature values of all images that was created from the directory 
void extractValues(std::ifstream &file1, double col_idx, std::vector<double> &column_values) 
{
    std::string line;

    while (std::getline(file1, line)) 
    {
        std::stringstream line_stream(line);
        std::string cell;
        double column = 0;
        while (std::getline(line_stream, cell, ',')) {
        if (column == col_idx) {
            try {
                column_values.push_back(std::stof(cell));
            } catch (const std::invalid_argument &e) {
                //std::cout << "Invalid argument: " << e.what() << std::endl;
            } catch (const std::out_of_range &e) {
                //std::cout << "Out of range: " << e.what() << std::endl;
            }
        }
        column++;
        }
    }
}

// Distannce metric for Sum of squared difference 
double SSD(std::vector<double> &featureVector1, std::vector<double> &featureVector2) 
{
    double sum = 0.0;
    if (featureVector1.size() != featureVector2.size()) {
        return -1.0; // return -1 if the sizes are unequal
    }
    for (double i = 0; i < featureVector1.size(); i++) 
    {
        sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        //std::cout << featureVector1.size() << std::endl;
        //sum += (std::min(featureVector2.at(i), featureVector1.at(i)));
    }
    return sum;
}


// Distance metric for Histrogram intersection 
double HM(std::vector<double> &featureVector1, std::vector<double> &featureVector2) 
{
    double intersection = 0.0;
    double sum = 0.0;
    if (featureVector1.size() != featureVector2.size()) {
        return -1.0; // return -1 if the sizes are unequal
    }
    for (double i = 0; i < featureVector1.size(); i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        //std::cout << featureVector1.size() << std::endl;
        double target_total = 0.0;
        double feature_total = 0.0;
        // Calculating the sum of all feature points which will be used in normalization process
        for ( int i = 0; i<featureVector1.size(); i++)
        {
            target_total += featureVector1[i];
            feature_total += featureVector2[i];
        }
        // Normalization process
        for (int i = 0; i<featureVector1.size(); i++)
        {
            double suma = featureVector1[i]/target_total;
            double sumb = featureVector2[i]/feature_total;
            intersection += suma < sumb ? suma : sumb;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    }
    // Histogram intersection calculation 
    return (1.0 - intersection);
}


//Multi Histogram matching 
double MHM(std::vector<double> &featureVector1, std::vector<double> &featureVector2) 
{
    double intersection = 0.0;
    double intersection1 = 0.0;
    double intersection2 = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;
    if (featureVector1.size() != featureVector2.size()) {
        return -1.0; // return -1 if the sizes are unequal
    }

    // ---------------------------------------------------------Target Feeature Vector seperation ------------------------------------------------------------------------//

    int n = featureVector1.size();
    int half1 = n / 2;
    std::vector<int> firstHalf1(featureVector1.begin(), featureVector1.begin() + half1);
    std::vector<int> secondHalf1(featureVector1.begin() + half1, featureVector1.end());

    // ---------------------------------------------------------Dataset Feature Vector seperation ------------------------------------------------------------------------//

    int n1 = featureVector2.size();
    int half2 = n1 / 2;
    std::vector<int> firstHalf2(featureVector2.begin(), featureVector2.begin() + half2);
    std::vector<int> secondHalf2(featureVector2.begin() + half2, featureVector2.end());
    
    //-------------------------------------------------------------------- Histogram 1 calculation --------------------------------------------------------------- //
    for (double i = 0; i < featureVector1.size()/2; i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        double target_total1 = 0.0;
        double feature_total1 = 0.0;

        for ( int i = 0; i<firstHalf2.size(); i++)
        {
            target_total1 += firstHalf1[i];
            feature_total1 += firstHalf2[i];
        }
        // Normalization process
        for (int i = 0; i<firstHalf2.size(); i++)
        {
            double suma1 = firstHalf1[i]/target_total1;
            double sumb1 = firstHalf2[i]/feature_total1;
            intersection1 += suma1 < sumb1 ? suma1 : sumb1;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    
    }
    float inter1 = 1.0 - intersection1;
    
    //-------------------------------------------------------------------- Histogram 2 calculation --------------------------------------------------------------- //
    for (double i = 0; i < featureVector1.size()/2; i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        double target_total2 = 0.0;
        double feature_total2 = 0.0;

        for ( int i = 0; i<secondHalf2.size(); i++)
        {
            target_total2 += secondHalf1[i];
            feature_total2 += secondHalf2[i];
        }
        // Normalization process
        for (int i = 0; i<secondHalf2.size(); i++)
        {
            double suma2 = secondHalf1[i]/target_total2;
            double sumb2 = secondHalf2[i]/feature_total2;
            intersection2 += suma2 < sumb2 ? suma2 : sumb2;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    }
    float inter2 = 1.0 - intersection2;
    // Alloting weight for each histogram 
    return (0.5*inter1 + 0.5*inter2);
}
// Center Multi histogram Matching 
double CMHM(std::vector<double> &featureVector1, std::vector<double> &featureVector2) 
{
    double intersection = 0.0;
    double intersection1 = 0.0;
    double intersection2 = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;
    if (featureVector1.size() != featureVector2.size()) {
        return -1.0; // return -1 if the sizes are unequal
    }

    // ---------------------------------------------------------Target Feeature Vector seperation ------------------------------------------------------------------------//

    int n = featureVector1.size();
    int half1 = n / 2;
    std::vector<int> firstHalf1(featureVector1.begin(), featureVector1.begin() + half1);
    std::vector<int> secondHalf1(featureVector1.begin() + half1, featureVector1.end());

    // ---------------------------------------------------------Dataset Feature Vector seperation ------------------------------------------------------------------------//

    int n1 = featureVector2.size();
    int half2 = n1 / 2;
    std::vector<int> firstHalf2(featureVector2.begin(), featureVector2.begin() + half2);
    std::vector<int> secondHalf2(featureVector2.begin() + half2, featureVector2.end());
    
    //-------------------------------------------------------------------- Histogram 1 calculation --------------------------------------------------------------- //
    for (double i = 0; i < featureVector1.size()/2; i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        double target_total1 = 0.0;
        double feature_total1 = 0.0;

        for ( int i = 0; i<firstHalf2.size(); i++)
        {
            target_total1 += firstHalf1[i];
            feature_total1 += firstHalf2[i];
        }
        // Normalization process
        for (int i = 0; i<firstHalf2.size(); i++)
        {
            double suma1 = firstHalf1[i]/target_total1;
            double sumb1 = firstHalf2[i]/feature_total1;
            intersection1 += suma1 < sumb1 ? suma1 : sumb1;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    
    }
    float inter1 = 1.0 - intersection1;
    
    //-------------------------------------------------------------------- Histogram 2 calculation --------------------------------------------------------------- //
    for (double i = 0; i < featureVector1.size()/2; i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        double target_total2 = 0.0;
        double feature_total2 = 0.0;

        for ( int i = 0; i<secondHalf2.size(); i++)
        {
            target_total2 += secondHalf1[i];
            feature_total2 += secondHalf2[i];
        }
        // Normalization process
        for (int i = 0; i<secondHalf2.size(); i++)
        {
            double suma2 = secondHalf1[i]/target_total2;
            double sumb2 = secondHalf2[i]/feature_total2;
            intersection2 += suma2 < sumb2 ? suma2 : sumb2;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    }
    float inter2 = 1.0 - intersection2;
    return (0.3*inter1 + 0.7*inter2);
}


//Triple Histogram matching 
double THM(std::vector<double> &featureVector1, std::vector<double> &featureVector2) 
{
    double intersection1 = 0.0;
    double intersection2 = 0.0;
    double intersection3 = 0.0;
    if (featureVector1.size() != featureVector2.size()) {
        return -1.0; // return -1 if the sizes are unequal
    }

    // ---------------------------------------------------------Target Feeature Vector seperation ------------------------------------------------------------------------//

    int n = featureVector1.size();
    int half1 = n / 3;
    std::vector<int> firstHalf1(featureVector1.begin(), featureVector1.begin() + half1);
    std::vector<int> secondHalf1(featureVector1.begin() + half1,featureVector1.begin() + half1+half1 );
    std::vector<int> thirdHalf1( featureVector1.begin() + half1+half1, featureVector1.end());
    // ---------------------------------------------------------Dataset Feature Vector seperation ------------------------------------------------------------------------//

    int n1 = featureVector2.size();
    int half2 = n1 / 3;
    std::vector<int> firstHalf2(featureVector2.begin(), featureVector2.begin() + half2);
    std::vector<int> secondHalf2(featureVector2.begin() + half2,featureVector2.begin() + half2+half2 );
    std::vector<int> thirdHalf2(featureVector2.begin() + half2+half2, featureVector2.end());
    
    //-------------------------------------------------------------------- Histogram 1 calculation --------------------------------------------------------------- //
    for (double i = 0; i < featureVector1.size()/3; i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        double target_total1 = 0.0;
        double feature_total1 = 0.0;

        for ( int i = 0; i<firstHalf2.size(); i++)
        {
            target_total1 += firstHalf1[i];
            feature_total1 += firstHalf2[i];
        }
        // Normalization process
        for (int i = 0; i<firstHalf2.size(); i++)
        {
            double suma1 = firstHalf1[i]/target_total1;
            double sumb1 = firstHalf2[i]/feature_total1;
            intersection1 += suma1 < sumb1 ? suma1 : sumb1;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    
    }
    float inter1 = 1.0 - intersection1;
    
    //-------------------------------------------------------------------- Histogram 2 calculation --------------------------------------------------------------- //
    for (double i = 0; i < featureVector1.size()/3; i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        double target_total2 = 0.0;
        double feature_total2 = 0.0;

        for ( int i = 0; i<secondHalf2.size(); i++)
        {
            target_total2 += secondHalf1[i];
            feature_total2 += secondHalf2[i];
        }
        // Normalization process
        for (int i = 0; i<secondHalf2.size(); i++)
        {
            double suma2 = secondHalf1[i]/target_total2;
            double sumb2 = secondHalf2[i]/feature_total2;
            intersection2 += suma2 < sumb2 ? suma2 : sumb2;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    }
    float inter2 = 1.0 - intersection2;
    //-------------------------------------------------------------------- Histogram 3 calculation --------------------------------------------------------------- //
    for (double i = 0; i < featureVector1.size()/3; i++) {
        //sum += pow(featureVector1.at(i) - featureVector2.at(i), 2);
        double target_total3 = 0.0;
        double feature_total3 = 0.0;

        for ( int i = 0; i<thirdHalf2.size(); i++)
        {
            target_total3 += thirdHalf1[i];
            feature_total3 += thirdHalf2[i];
        }
        // Normalization process
        for (int i = 0; i<thirdHalf2.size(); i++)
        {
            double suma3 = thirdHalf1[i]/target_total3;
            double sumb3 = thirdHalf2[i]/feature_total3;
            intersection3 += suma3 < sumb3 ? suma3 : sumb3;
        }
        //sum += (std::min(featureVector1.at(i), featureVector2.at(i)));
        //std::cout << featureVector1.size() << std::endl;
    }
    float inter3 = 1.0 - intersection3;
    return (0.45*inter1 + 0.45*inter2 + 0.1*inter3);
}


template <typename T>
void printVectors(std::vector<std::vector<T>> columns) 
{
    for (double i = 0; i < columns.size(); ++i) {
        //std::cout << "Column " << i << ":" << std::endl;
        for (const auto &value : columns[i]) {
            //std::cout << value << std::endl;
        }
        //std::cout << std::endl;
    }
}


void extractFeatures(std::ifstream &file, std::vector<std::vector<double>> &columns, std::map<double, std::string> &map) 
{
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream line_stream(line);
        std::string cell;
        double column = 0;
        while (std::getline(line_stream, cell, ',')) {
            if (column == columns.size()) {
                columns.emplace_back();
            }

            try {
                columns[column].push_back(std::stof(cell));
            } catch (const std::invalid_argument &e) {
                map[column] = cell;
                //std::cout << "Invalid argument: " << e.what() << std::endl;
            } catch (const std::out_of_range &e) {
                //std::cout << "Out of range: " << e.what() << std::endl;
            }
            column++;
        }
    }
}

// This function performs the Distance calculation between the feature points based on the feature extraction process
void computeSsd(std::vector<double> &targetFeatureVector, 
                std::vector<std::vector<double>> &featureVectors, 
                std::vector<double> &ssdValues, 
                std::map<double, double> &indexToSsd, 
                std::map<double, double> &ssdToIndex, char &in)
                 
    {
    // Calculating the distance for each points 
    for (double idx = 0; idx < featureVectors.size(); idx++) 
    {
        std::vector<double> featureVector = featureVectors[idx];
        // Calculates Sum of squared distance 
        if ( in == 'E'|| in == 'e')
        {
        double ssdVal = SSD(targetFeatureVector, featureVector);
        indexToSsd[idx] = ssdVal;
        ssdToIndex[ssdVal] = idx;
        ssdValues.push_back(ssdVal);
        }
        // Calculates Histogram intersection 
        else if ( in == 'F'|| in == 'f')
        {
        double ssdVal = HM(targetFeatureVector, featureVector);
        indexToSsd[idx] = ssdVal;
        ssdToIndex[ssdVal] = idx;
        ssdValues.push_back(ssdVal);
        }
        // Calculates Histogram intersection (for 3 histograms)
        else if ( in == 'D'|| in == 'd')
        {
        double ssdVal = THM(targetFeatureVector, featureVector);
        indexToSsd[idx] = ssdVal;
        ssdToIndex[ssdVal] = idx;
        ssdValues.push_back(ssdVal);
        }
        // Calculates Histogram intersection (for 2 histograms) 
        else if ( in == 'B'|| in == 'b'||in == 'C'|| in == 'c')
        {
        double ssdVal = MHM(targetFeatureVector, featureVector);
        indexToSsd[idx] = ssdVal;
        ssdToIndex[ssdVal] = idx;
        ssdValues.push_back(ssdVal);
        }
        // Calculates Histogram intersection (for 2 histograms) different weights 
        else if ( in == 'A'|| in == 'a')
        {
        double ssdVal = CMHM(targetFeatureVector, featureVector);
        indexToSsd[idx] = ssdVal;
        ssdToIndex[ssdVal] = idx;
        ssdValues.push_back(ssdVal);
        }
    }
    // Sorts the values in accesending order 
    std::sort (ssdValues.begin(), ssdValues.end());   
}
