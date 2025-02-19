/*
    Created by 

    Shirish Kishore Kumar  
    Soorya Vijayaragavan

    This program extracts the feature points and creates histogram then saves those values in a CSV file
    We created our own program to store the data in a CSV file 

*/

#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <dirent.h>






// This function converts a whole image to a mat which the center 9*9 pixel of the image
cv::Mat extract_9times9_rgb_values_from_center_of_an_feature_image(cv::Mat &src)
{
    cv::Mat mat = src(cv::Rect(src.cols / 2 - 4, src.rows / 2 - 4, 9, 9));
    return mat;
}

// This function converts a image to a vector in the order of BGR 
std::vector<float> image_to_vector(const cv::Mat &image) 
{
    std::vector<float> vectorized_image;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            for (int k = 0; k < image.channels(); k++) {
                vectorized_image.push_back(image.at<cv::Vec3b>(i, j)[k]);
            }
        }
    }
    
    return vectorized_image;
}

// This function creates a 3d Histogram and gets a image as a input 
std::vector<float> histogram3d(cv::Mat &src) {
    // Declaring the histogram size
    int histogram_size = 16;
    // Calculating the histogram width 
    float bin_width = 256 / histogram_size;
    int histogram[] = {histogram_size, histogram_size, histogram_size};
    // Declaring a mat to hold the feature values as it increments 
    cv::Mat feature_vector = cv::Mat::zeros(3, histogram, CV_32F);
    // Declared for debugging purpose
    int num_channels1 = feature_vector.channels();
    //std::cout << feature_vector.size() << std::endl;
    //std::cout << "Number of channels in the created image: " << num_channels1 << std::endl;


    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            // Calculating which bin the channel value of a pixel falls into
            float b = src.at<cv::Vec3b>(i, j)[0] / bin_width;
            float g = src.at<cv::Vec3b>(i, j)[1] / bin_width;
            float r = src.at<cv::Vec3b>(i, j)[2] / bin_width;
            // Incrementing the value by one for each bin ( 512 feature points for 8 bins)
            feature_vector.at<float>(b, g, r)++;
        }
    }

    // Reshaping the feature vector 
    cv::Mat dst = feature_vector.reshape(1, (feature_vector.rows * feature_vector.cols * feature_vector.channels()));
    dst.convertTo(dst, CV_32F);

    return dst;
}

// Reference:
// https://stackoverflow.com/questions/49444856/transpose-of-a-2d-matrix-using-vectors-in-c
// Taking a transpose of the feature vector to store in the csv file 
template<typename T>
void transpose(std::vector<std::vector<T>> &input2d) 
{
    
    if (input2d.empty())
        return;
    
    //std::cout << input2d[0].size() << "x" << input2d.size() << std::endl;

    std::vector<std::vector<T> > trans_vec(input2d[0].size(), std::vector<T>());

    for (int i = 0; i < input2d.size(); i++) {
        for (int j = 0; j < input2d[i].size(); j++) {
            trans_vec[j].push_back(input2d[i][j]);
        }
    }

    //std::cout << trans_vec[0].size() << "x" << trans_vec.size() << std::endl;

    input2d = trans_vec;
}

// Reference:
// https://stackoverflow.com/questions/26499307/transfer-a-2d-array-to-a-csv-file
// Writing the feature vector of each image in such a way that it stores each image in a column with its image name 
void writeToCsv(const std::string &csv_file_path, std::vector<std::vector<std::string>> &histData) {
    std::ofstream csv_file;

    std::cout << "Writing to CSV..." << std::endl;
    // CSV file name and storing it in the same file as the program is in.
    csv_file.open(csv_file_path);

    for (auto &row: histData) 
    {
        //Storing the feature points one by one in a column
        for (const auto &col: row)
            csv_file << col << ',';
        csv_file << '\n';
    }
    std::cout << "Creating the CSV file Completed ..." << std::endl;
    csv_file.close();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// This function calculates the feature points of all images from a given directory 
void feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(const std::string &folder_path, const std::string &csv_file_path, char &in)
{
    // This loop calculates 2 histigram 1) RGB of whole image, 2) Center 50*50 RGB histogram
     if ( in == 'A' || in == 'a' )
    {
        // Going through all images in the directory 
        DIR *dir;
        struct dirent *ent;
        // Declaring a new vector 
        std::vector<std::vector<std::string>> histData;
        std::cout << "Extracting the features: " <<  std::endl;
        int column_count = 0;
        if ((dir = opendir(folder_path.c_str())) != nullptr) 
        {
            while ((ent = readdir(dir)) != nullptr) 
            {
                std::vector<std::string> row;
                std::string file_name = ent->d_name;
                std::string file_path = folder_path + "/" + file_name;
                // Reading the image one by one in the folder
                cv::Mat image = cv::imread(file_path);
                cv::Mat image1 ;
                if (!image.empty()) 
                {
                    // This function calculates the histogram and then storing it in a vector 
                    std::vector<float> feature = histogram3d(image);
                    // Creating a new image that contains only the center 50*50 pixels 
                    image1 = image(cv::Rect(image.cols / 2 - 4, image.rows / 2 - 4, 50, 50));
                    // Calculates the histogram of the center pixel 
                    std::vector<float> feature1 = histogram3d(image1);
                    // Combing that to the previous vector that already has the whole image feature points 
                    feature.insert(feature.end(), feature1.begin(), feature1.end());
                    // Storing the image name at the top of the column before storing the feature points 
                    row.push_back(file_name);
                    // Storing the feature points one by one in to a vector 
                    for (float i : feature) 
                    {
                        // csv_file  << feature[i] << endl;
                        row.push_back(std::to_string(i));
                    }
                    // Storing those values into the vector declared before 
                    histData.push_back(row);
                } 
                
                else 
                {
                    //std::cout << "Failed to read image: " << file_name << std::endl;
                }
            
        
        }
        std::cout << "<--------------------------------Done---------------------------->" << std::endl;
        closedir(dir);
        }

    

    else 
    {
        std::cout << "Could not open folder: " << folder_path << std::endl;
        return;
    }
    // Taking transpose of the vector that contains all the data 
    transpose(histData);
    // This function stores those values to the csv file 
    writeToCsv(csv_file_path, histData);

    }
    // This loop extracts two histograms 1--> Top half , 2--> Second half
    else if ( in == 'B' || in == 'b' )
    {
        // Implementing the same logic as before 
        DIR *dir;
        struct dirent *ent;

        std::vector<std::vector<std::string>> histData;
        std::cout << "Extracting the features: " <<  std::endl;
        int column_count = 0;
        if ((dir = opendir(folder_path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr)
            {
                std::vector<std::string> row;
                std::string file_name = ent->d_name;
                std::string file_path = folder_path + "/" + file_name;
                cv::Mat image = cv::imread(file_path);
                // ---------------------------------------------------- Read the Top half of the image ------------------------------------------------------------------------//
                cv::Mat imageh1 = image(cv::Rect(0, 0, image.cols, image.rows / 2));
                cv::Mat image1 ;
                if (!image.empty()) {
                    std::vector<float> feature = histogram3d(imageh1);

                    // ---------------------------------------------------- Read the Bottom half of the image ---------------------------------------------------------------------//
                    cv::Mat image1 = image(cv::Rect(0, image.rows / 2, image.cols, image.rows / 2));
                    std::vector<float> feature1 = histogram3d(image1);
                    feature.insert(feature.end(), feature1.begin(), feature1.end());
                    row.push_back(file_name);
                    for (float i : feature) {
                        // csv_file  << feature[i] << endl;
                        row.push_back(std::to_string(i));
                    }
                    histData.push_back(row);
                }

                else
                {
                    //std::cout << "Failed to read image: " << file_name << std::endl;
                }


            }
            std::cout << "<--------------------------------Done---------------------------->" << std::endl;
            closedir(dir);
        }



        else
        {
            std::cout << "Could not open folder: " << folder_path << std::endl;
            return;
        }

        transpose(histData);
        writeToCsv(csv_file_path, histData);

    }
    // This loop calculates 2 histogram 1--> Texture in this case magnitude, 2--> RGB histogram of the whole image 
    else if ( in == 'C' || in == 'c' )
    {

        DIR *dir;
        struct dirent *ent;

        std::vector<std::vector<std::string>> histData;
        std::cout << "Extracting the features: " <<  std::endl;
        int column_count = 0;
        if ((dir = opendir(folder_path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr)
            {
                std::vector<std::string> row;
                std::string file_name = ent->d_name;
                std::string file_path = folder_path + "/" + file_name;
                cv::Mat image = cv::imread(file_path);


                if (!image.empty()) {

                    // std::cout << "Matrix size: " << imageh1.rows << " x " << imageh1.cols << std::endl;
                    // std::cout << "Matrix type: " << imageh1.type() << std::endl;
                    std::vector<float> feature = histogram3d(image);
                    cv::Mat image1 = image(cv::Rect(0, 0, image.cols, image.rows));
                    cv::Mat dst1;
                    // Using the Magnitude filter created from previous assignment 
                    magnitude(image1,dst1);
                    cv::Mat imagez2 = dst1;

                    std::vector<float> feature1 = histogram3d(imagez2);
                    feature.insert(feature.end(), feature1.begin(), feature1.end());
                    row.push_back(file_name);
                    for (float i : feature) {
                        // csv_file  << feature[i] << endl;
                        row.push_back(std::to_string(i));
                    }
                    histData.push_back(row);
                }

                else
                {
                    //std::cout << "Failed to read image: " << file_name << std::endl;
                }


            }
            std::cout << "<--------------------------------Done---------------------------->" << std::endl;
            closedir(dir);
        }



        else
        {
            std::cout << "Could not open folder: " << folder_path << std::endl;
            return;
        }

        transpose(histData);
        writeToCsv(csv_file_path, histData);

    }
    // This loop calculates 3 Histogram for extension part 1--> RGB of whole image 2--> Texture (Magnitude) 3--> Center pixels 
    else if ( in == 'D' || in == 'd' )
    {

        DIR *dir;
        struct dirent *ent;

        std::vector<std::vector<std::string>> histData;
        std::cout << "Extracting the features: " <<  std::endl;
        int column_count = 0;
        if ((dir = opendir(folder_path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr)
            {
                std::vector<std::string> row;
                std::string file_name = ent->d_name;
                std::string file_path = folder_path + "/" + file_name;
                cv::Mat image = cv::imread(file_path);

                // ---------------------------------------------------- Read RGB of the image -----------------------------------------------------------------------------//
                // cv::Mat imageh1 = image(cv::Rect(0, 0, image.cols, image.rows / 2));
                // cv::Mat image1 ;

                if (!image.empty()) {

                    std::vector<float> feature = histogram3d(image);

                    // ---------------------------------------------------- Converting to Magnitude image -----------------------------------------------------------------//

                    cv::Mat image1 = image(cv::Rect(0, 0, image.cols, image.rows));
                    cv::Mat dst1;
                    magnitude(image1,dst1);
                    cv::Mat imagez2 = dst1;

                    std::vector<float> feature1 = histogram3d(imagez2);
                    feature.insert(feature.end(), feature1.begin(), feature1.end());
                    row.push_back(file_name);
                    // -------------------------------------------------- Calculating the center part of the image -------------------------------------------------------//
                    cv::Mat image3 = image(cv::Rect(image.cols / 2 - 4, image.rows / 2 - 4, 50, 50));
                    std::vector<float> feature3 = histogram3d(image3);
                    feature.insert(feature.end(), feature3.begin(), feature3.end());
                    for (float i : feature)
                    {
                        // csv_file  << feature[i] << endl;
                        row.push_back(std::to_string(i));
                    }

                    histData.push_back(row);


                }

                else
                {
                    //std::cout << "Failed to read image: " << file_name << std::endl;
                }


            }
            std::cout << "<--------------------------------Done---------------------------->" << std::endl;
            closedir(dir);
        }



        else
        {
            std::cout << "Could not open folder: " << folder_path << std::endl;
            return;
        }
//    for (int i = 0; i < histData.size(); i++) {
//   for (int j = 0; j < histData[i].size(); j++) {
//     std::cout << histData[i][j] << ' ';
//   }
//   std::cout << std::endl;
// }

        transpose(histData);
        //std::cout << " Culprit" << std::endl;
        writeToCsv(csv_file_path, histData);

    }
    // This loop performs the baseline matching 
    else if ( in == 'E' || in == 'e')
    {
        DIR *dir;
        struct dirent *ent;
        std::vector<std::vector<std::string>> histData;
        std::cout << "Extracting the features : " <<  std::endl;
        int column_count = 0;
        if ((dir = opendir(folder_path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr)
            {
                std::vector<std::string> row;
                std::string file_name = ent->d_name;
                std::string file_path = folder_path + "/" + file_name;
                cv::Mat image = cv::imread(file_path);
                if (!image.empty()) {
                    //std::cout << "culprit" << std::endl;
                    // This function extracts the center pixel values 
                    cv::Mat img = extract_9times9_rgb_values_from_center_of_an_feature_image(image);
                    std::vector<float> feature = image_to_vector(img);

                    //std::cout << ":::"<< feature.size() << std::endl;
                    // csv_file << file_name << endl;

                    row.push_back(file_name);
                    for (float i : feature) {
                        row.push_back(std::to_string(i));
                    }
                    // csv_file << ",";


                    histData.push_back(row);
                }

                else
                {
                    //std::cout << "Failed to read image: " << file_name << std::endl;
                }

            }
            std::cout << "<--------------------------------Done---------------------------->" << std::endl;
            closedir(dir);
        } else {
            std::cout << "Could not open folder: " << folder_path << std::endl;
            return;
        }

        transpose(histData);
        writeToCsv(csv_file_path, histData);

    }
    // This loop performs single histogram matching 
    else if ( in == 'F' || in == 'f')
    {

        DIR *dir;
        struct dirent *ent;

        std::vector<std::vector<std::string>> histData;
        std::cout << "Extracting the features : " <<  std::endl;
        int column_count = 0;
        if ((dir = opendir(folder_path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr)
            {
                std::vector<std::string> row;
                std::string file_name = ent->d_name;
                std::string file_path = folder_path + "/" + file_name;
                cv::Mat image = cv::imread(file_path);
                if (!image.empty()) {
                    std::vector<float> feature = histogram3d(image);
                    //std::cout << ":::"<< feature.size() << std::endl;
                    // csv_file << file_name << endl;
                    row.push_back(file_name);
                    for (float i : feature) {
                        // csv_file  << feature[i] << endl;
                        row.push_back(std::to_string(i));
                    }
                    // csv_file << ",";


                    histData.push_back(row);
                }

                else
                {
                    //std::cout << "Failed to read image: " << file_name << std::endl;
                }

            }
            std::cout << "<--------------------------------Done---------------------------->" << std::endl;
            closedir(dir);
        } else {
            std::cout << "Could not open folder: " << folder_path << std::endl;
            return;
        }

        transpose(histData);
        writeToCsv(csv_file_path, histData);

    }



}
//-----------------------------------------------------------------------------------------------------------------------------------------------//
// This function extracts the 9*9 rgb values of the target image 
void extract_9times9_rgb_values_from_center_of_an_target_image(cv::Mat &src)
{

    std::ofstream file("fe1h.csv");

    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        //return 1;
    }
    // Naming the columns as Target feature vector in csv 
    file << "Target_feature_vector" << std::endl;
    cv::Mat image1 = src;
    for (int i = image1.rows / 2 - 4; i < image1.rows / 2 + 5; ++i)
    {
        for (int j = image1.cols / 2 - 4; j < image1.cols / 2 + 5; ++j)
        {
            // Storing each channel value of the pixel into the csv file 
            file << (int)image1.at<cv::Vec3b>(i, j)[0];
            file << std::endl;
            file << (int)image1.at<cv::Vec3b>(i, j)[1];
            file << std::endl;
            file << (int)image1.at<cv::Vec3b>(i, j)[2];
            file << std::endl;

        }
    }
    file.close();

}

// This function stores the single histogram value of the target image into the csv file 
void target_single_histogram3d(cv::Mat &src)
{

    std::vector<float> data = histogram3d(src);
    std::ofstream file("fe1h.csv");

    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        //return 1;
    }
    file << "Target_feature_vector" << std::endl;
    for (float i: data)
    {
        file << i << std::endl;
    }
    file.close();

}

// This function stores the multiple histogram value of the target image into the csv file 
void target_multi_histogram3d(cv::Mat &src1, cv::Mat &src2 )
{
    // Storing the histogram values into a vector 
    std::vector<float> data1 = histogram3d(src1);
    std::vector<float> data2 = histogram3d(src2);
    std::ofstream file("fe1h.csv");

    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        //return 1;
    }
    // Storing those values into the csv file 
    file << "Target_feature_vector" << std::endl;
    for (float i: data1)
    {
        file << i << std::endl;
    }
    for (float i: data2)
    {
        file << i << std::endl;
    }
    file.close();

}

// This function stores the triple histogram value of the target image into the csv file 
void target_triple_histogram3d(cv::Mat &src1, cv::Mat &src2, cv::Mat &src3  )
{
    // Storing the histogram values into a vector 
    std::vector<float> data1 = histogram3d(src1);
    std::vector<float> data2 = histogram3d(src2);
    std::vector<float> data3 = histogram3d(src3);
    std::ofstream file("fe1h.csv");

    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        //return 1;
    }
    file << "Target_feature_vector" << std::endl;
    // Storing those values into the csv file 
    for (float i: data1)
    {
        file << i << std::endl;
    }
    for (float i: data2)
    {
        file << i << std::endl;
    }
    for (float i: data3)
    {
        file << i << std::endl;
    }
    file.close();

}