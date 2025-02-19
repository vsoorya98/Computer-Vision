/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * This file performs the classifying work uing KNN and also calculates the scaled euclidean distance
 */

#include <map>
#include "Classifier.h"
#include "../csv_util.h"
#include "../utils.h"

// Reading the feature values from the csv file and storing it in a map
Classifier::Classifier() {
    read_from_csv("features.csv", this->data);

    for (const feature& f : this->data) {
        this->mu_22_alpha.push_back(f.mu_22_alpha);
        this->fill_percent.push_back(f.fill_percent);
        this->h_w_ratio.push_back(f.h_w_ratio);
        this->hu_moment_1.push_back(f.hu_moment_1);
        this->hu_moment_2.push_back(f.hu_moment_2);
        this->hu_moment_3.push_back(f.hu_moment_3);
        this->hu_moment_4.push_back(f.hu_moment_4);
        this->hu_moment_5.push_back(f.hu_moment_5);
        this->hu_moment_6.push_back(f.hu_moment_6);
        this->hu_moment_7.push_back(f.hu_moment_7);
    }

    this->map_sd_index["mu_22_alpha"] = 0;
    this->map_sd_index["fill_percent"] = 1;
    this->map_sd_index["h_w_ratio"] = 2;
    this->map_sd_index["hu_moment_1"] = 3;
    this->map_sd_index["hu_moment_2"] = 4;
    this->map_sd_index["hu_moment_3"] = 5;
    this->map_sd_index["hu_moment_4"] = 6;
    this->map_sd_index["hu_moment_5"] = 7;
    this->map_sd_index["hu_moment_6"] = 8;
    this->map_sd_index["hu_moment_7"] = 9;

    this->compute_sd();
}


// This function calculates the standard deviation for all the feature values
void Classifier::compute_sd() {
    this->sd.push_back(standard_deviation(this->mu_22_alpha));
    this->sd.push_back(standard_deviation(this->fill_percent));
    this->sd.push_back(standard_deviation(this->h_w_ratio));
    this->sd.push_back(standard_deviation(this->hu_moment_1));
    this->sd.push_back(standard_deviation(this->hu_moment_2));
    this->sd.push_back(standard_deviation(this->hu_moment_3));
    this->sd.push_back(standard_deviation(this->hu_moment_4));
    this->sd.push_back(standard_deviation(this->hu_moment_5));
    this->sd.push_back(standard_deviation(this->hu_moment_6));
    this->sd.push_back(standard_deviation(this->hu_moment_7));
}

// This function exccutes the scaled euclidean distance from the target feature values to the feature values in the database
double Classifier::calculate_distance(const feature &target, const feature &other) {
    double distance = 0;
    double part;

    part = (target.mu_22_alpha - other.mu_22_alpha) / this->sd[this->map_sd_index["mu_22_alpha"]];
    distance += part * part;

    part = (target.fill_percent - other.fill_percent) / this->sd[this->map_sd_index["fill_percent"]];
    distance += part * part;

    part = (target.h_w_ratio - other.h_w_ratio) / this->sd[this->map_sd_index["h_w_ratio"]];
    distance += part * part;

    part = (target.hu_moment_1 - other.hu_moment_1) / this->sd[this->map_sd_index["hu_moment_1"]];
    distance += part * part;

    part = (target.hu_moment_2 - other.hu_moment_2) / this->sd[this->map_sd_index["hu_moment_2"]];
    distance += part * part;

    part = (target.hu_moment_3 - other.hu_moment_3) / this->sd[this->map_sd_index["hu_moment_3"]];
    distance += part * part;

    part = (target.hu_moment_4 - other.hu_moment_4) / this->sd[this->map_sd_index["hu_moment_4"]];
    distance += part * part;

    part = (target.hu_moment_5 - other.hu_moment_5) / this->sd[this->map_sd_index["hu_moment_5"]];
    distance += part * part;

    part = (target.hu_moment_6 - other.hu_moment_6) / this->sd[this->map_sd_index["hu_moment_6"]];
    distance += part * part;

    part = (target.hu_moment_7 - other.hu_moment_7) / this->sd[this->map_sd_index["hu_moment_7"]];
    distance += part * part;
    // Getting the sum of all values from all the features
    return distance;
}

// Performing the KNN
std::string Classifier::classify(const feature& target, int num_neighbors) {
    std::vector<std::pair<double, std::string>> distances;
    for (const feature& f : this->data) {
        double distance = calculate_distance(target, f);
        distances.emplace_back(distance, f.label);
    }

    std::sort(distances.begin(), distances.end());

    std::map<std::string, int> count;
    for (int i = 0; i < num_neighbors; i++) {
        count[distances[i].second]++;
    }

    int max_count = -1;
    std::string max_label;

    for (const auto& [label, c] : count) {
        if (c > max_count) {
            max_count = c;
            max_label = label;
        }
    }

    return max_label;
}

Classifier::~Classifier() = default;


