/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * In this program file we write all the features to the csv file
 */

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "csv_util.h"

// Writing the feature values from the vector to CSV in which each columns represent a object
void write_to_csv(const std::string& filename, const std::vector<feature>& data)
{
    std::ofstream csvFile;
    csvFile.open(filename, std::ofstream::trunc);

    for (const auto& f_data : data)
    {
        csvFile << f_data.label << ",";
        csvFile << f_data.mu_22_alpha << ",";
        csvFile << f_data.fill_percent << ",";
        csvFile << f_data.h_w_ratio << ",";
        csvFile << f_data.hu_moment_1 << ",";
        csvFile << f_data.hu_moment_2 << ",";
        csvFile << f_data.hu_moment_3 << ",";
        csvFile << f_data.hu_moment_4 << ",";
        csvFile << f_data.hu_moment_5 << ",";
        csvFile << f_data.hu_moment_6 << ",";
        csvFile << f_data.hu_moment_7;
        csvFile << "\n";
    }
}

// Reading the feature values from the CSV file to the vector
void read_from_csv(const std::string& filename, std::vector<feature>& data)
{
    std::ifstream csvFile;
    csvFile.open(filename);

    std::string line;
    while (std::getline(csvFile, line))
    {
        feature f_data;
        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        f_data.label = token;

        std::getline(ss, token, ',');
        f_data.mu_22_alpha = std::stof(token);

        std::getline(ss, token, ',');
        f_data.fill_percent = std::stof(token);

        std::getline(ss, token, ',');
        f_data.h_w_ratio = std::stof(token);

        std::getline(ss, token, ',');
        f_data.hu_moment_1 = std::stof(token);

        std::getline(ss, token, ',');
        f_data.hu_moment_2 = std::stof(token);

        std::getline(ss, token, ',');
        f_data.hu_moment_3 = std::stof(token);

        std::getline(ss, token, ',');
        f_data.hu_moment_4 = std::stof(token);

        std::getline(ss, token, ',');
        f_data.hu_moment_5 = std::stof(token);

        std::getline(ss, token, ',');
        f_data.hu_moment_6 = std::stof(token);

        std::getline(ss, token, ',');
        f_data.hu_moment_7 = std::stof(token);

        data.push_back(f_data);
    }
}




