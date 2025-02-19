//
// Created by master on 2/27/23.
//

#ifndef CV_UTILS_H
#define CV_UTILS_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <complex>

// Reference:
// https://stackoverflow.com/questions/33268513/calculating-standard-deviation-variance-in-c
template<typename T>
T variance(const std::vector<T> &vec) {
    const size_t sz = vec.size();
    if (sz <= 1) {
        return 0.0;
    }

    // Calculate the mean
    const T mean = std::accumulate(vec.begin(), vec.end(), 0.0) / sz;

    // Now calculate the variance
    auto variance_func = [&mean, &sz](T accumulator, const T& val) {
        return accumulator + ((val - mean)*(val - mean) / (sz - 1));
    };

    return std::accumulate(vec.begin(), vec.end(), 0.0, variance_func);
}

template<typename T>
T standard_deviation(const std::vector<T> &vec) {
    return std::sqrt(variance(vec));
}

#endif //CV_UTILS_H
