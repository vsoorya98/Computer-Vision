/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * Defining 10 features
 * Using Humoments, Fill percentage, H/W ratio and mu22alpha
 */

#include "feature.h"
#include <utility>
#include <iostream>

// Declaring the feature variables
feature::feature(
        double mu_22_alpha,
        double fill_percent,
        double h_w_ratio,
        double hu_moment_1,
        double hu_moment_2,
        double hu_moment_3,
        double hu_moment_4,
        double hu_moment_5,
        double hu_moment_6,
        double hu_moment_7) {

    this->mu_22_alpha = mu_22_alpha;
    this->fill_percent = fill_percent;
    this->h_w_ratio = h_w_ratio;
    this->hu_moment_1 = hu_moment_1;
    this->hu_moment_2 = hu_moment_2;
    this->hu_moment_3 = hu_moment_3;
    this->hu_moment_4 = hu_moment_4;
    this->hu_moment_5 = hu_moment_5;
    this->hu_moment_6 = hu_moment_6;
    this->hu_moment_7 = hu_moment_7;

}

// Member function that sets the label of the feature object
void feature::setLabel(std::string label_s) {
    this->label = std::move(label_s);
}

// Default constructor that creates an empty feature object
feature::feature() {}

// Constructor that initializes the member variables of the feature class with
// the values passed in an array of 7 doubles, along with three other double values
feature::feature(double *pDouble, double mu22alpha, double fill_percent, double hw_ratio) {
    this->mu_22_alpha = mu22alpha;
    this->fill_percent = fill_percent;
    this->h_w_ratio = hw_ratio;
    this->hu_moment_1 = pDouble[0];
    this->hu_moment_2 = pDouble[1];
    this->hu_moment_3 = pDouble[2];
    this->hu_moment_4 = pDouble[3];
    this->hu_moment_5 = pDouble[4];
    this->hu_moment_6 = pDouble[5];
    this->hu_moment_7 = pDouble[6];
}

// Member function that prints the member variables of the feature object to the standard output
void feature::print() const {
    std::cout << "label: " << this->label << std::endl;

    std::cout << "mu_22_alpha: " << this->mu_22_alpha << std::endl;
    std::cout << "fill_percent: " << this->fill_percent << std::endl;
    std::cout << "h_w_ratio: " << this->h_w_ratio << std::endl;
    std::cout << "hu_moment_1: " << this->hu_moment_1 << std::endl;
    std::cout << "hu_moment_2: " << this->hu_moment_2 << std::endl;
    std::cout << "hu_moment_3: " << this->hu_moment_3 << std::endl;
    std::cout << "hu_moment_4: " << this->hu_moment_4 << std::endl;
    std::cout << "hu_moment_5: " << this->hu_moment_5 << std::endl;
    std::cout << "hu_moment_6: " << this->hu_moment_6 << std::endl;
    std::cout << "hu_moment_7: " << this->hu_moment_7 << std::endl;
}
