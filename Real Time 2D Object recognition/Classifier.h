/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * This file performs the classifying work uing KNN and also calculates the scaled euclidean distance
 */

#ifndef CV_CLASSIFIER_H
#define CV_CLASSIFIER_H


#include <vector>
#include "../train/feature.h"
#include <unordered_map>


class Classifier {
public:
    Classifier();
    ~Classifier();

    std::string classify(const feature& target, int num_neighbors = 10);

private:
    std::vector<feature> data;
    std::vector<double> sd;
    std::unordered_map<std::string, int> map_sd_index;

    std::vector<std::vector<double>> features;

    std::vector<double> mu_22_alpha;
    std::vector<double> fill_percent;
    std::vector<double> h_w_ratio;
    std::vector<double> hu_moment_1;
    std::vector<double> hu_moment_2;
    std::vector<double> hu_moment_3;
    std::vector<double> hu_moment_4;
    std::vector<double> hu_moment_5;
    std::vector<double> hu_moment_6;
    std::vector<double> hu_moment_7;

    void compute_sd();
    double calculate_distance(const feature& target, const feature& other);
};


#endif //CV_CLASSIFIER_H
