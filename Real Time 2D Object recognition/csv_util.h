/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * This file contains all the function headers
 */

#ifndef CV_CSV_UTIL_H
#define CV_CSV_UTIL_H

#include "train/feature.h"

void write_to_csv(const std::string& filename, const std::vector<feature>& data);
void read_from_csv(const std::string& filename, std::vector<feature>& data);

#endif //CV_CSV_UTIL_H
