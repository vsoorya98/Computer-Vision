/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * created for testing purpose
 */
#include <iostream>
#include <vector>
#include "../csv_util.h"

std::vector<feature> features_vector;

void readFeatures() {
    read_from_csv("features.csv", features_vector);
}

int main(int argc, char** argv) {
    readFeatures();
    std::cout << features_vector.size() << std::endl;
    // print the first feature
    feature f = features_vector[0];
    f.print();
}