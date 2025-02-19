/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * Defining 10 features
 * Using Humoments, Fill percentage, H/W ratio and mu22alpha
 */

#ifndef CV_FEATURE_H
#define CV_FEATURE_H


#include <string>

class feature {
public:
    feature(double pDouble[7], double mu22alpha, double fill_percent, double hw_ratio);

    double mu_22_alpha{};
    double fill_percent{};
    double h_w_ratio{};
    double hu_moment_1{};
    double hu_moment_2{};
    double hu_moment_3{};
    double hu_moment_4{};
    double hu_moment_5{};
    double hu_moment_6{};
    double hu_moment_7{};

    std::string label;

    feature(
            double mu_22_alpha,
            double fill_percent,
            double h_w_ratio,
            double hu_moment_1,
            double hu_moment_2,
            double hu_moment_3,
            double hu_moment_4,
            double hu_moment_5,
            double hu_moment_6,
            double hu_moment_7
    );

    feature();


    void setLabel(std::string label_s);
    void print() const;
};


#endif //CV_FEATURE_H
