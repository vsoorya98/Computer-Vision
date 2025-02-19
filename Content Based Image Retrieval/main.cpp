/*
    Created by 

    Shirish Kishore Kumar  
    Soorya Vijayaragavan 

    This program contains the main function, this program gets the following input from the user 
    1) The image directory (make sure it is in the same file as this program file) [for example : ./main ./(image_directory)]
    2) The image name (Target image) from the image directory [for example : ./main ./(image_directory) ./(image_directoy)/(image_name.jpg)]

    After that it will ask for what type of feature extraction process the user requires

    Then it will ask for the number of top similar results the user wants

*/

#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include "filter.cpp"
#include "inter.cpp"
#include "inter.h"
#include "feature_extractor.cpp"
#include "feature_extractor.h"



int main(int argc, char **argv)
{
    // Printing the image directory and image name from that directory 
    std::cout << "You have entered " << argc << " arguments:" << "\n";
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << "\n";
    // Getting the input alphabhet from the user specifying what kind of feature extraction the program need to exceute 
    char in;
    std::cout << "<------------------------------------------------------------------------------------------------------------------>" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Enter the type of feature extraction process you want " << std::endl;
    std::cout << "A : Center 50*50 as a first Histogram and whole image RGB as second histogram \nB : Multi Histogram Matching  \nC : Texture and color \nD : Extension (3 Histogram) \nE : Baseline Matching  \nF : Histogram Matching " << std::endl;
    std::cin >> in ;
    // If 'a' is chosen then it creates two histogram where one histrogram represents RGB of the whole image and the other represents the RGB of the center 50*50 pixels of the image 
    if (in == 'A' || in == 'a')
        {
            // Declaring the image directory
            std::string folder_path = argv[1];
            // Declaring the csv path for the features extracted from all images of the directory given
            std::string csv_file_path = "fe2h.csv";
            // This function performs the feature extraction of all images from a directory 
            feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(folder_path, csv_file_path, in);
            // Reading the target image 
            cv::Mat image = imread(argv[2], cv::IMREAD_COLOR);
            // ---------------------------------------------------- Read the center 50*50 pixel of a image ----------------------------------------------------------------//
            cv::Mat feature = image(cv::Rect(image.cols / 2 - 4, image.rows / 2 - 4, 50, 50));
            target_multi_histogram3d(image, feature);
        }
    // If 'b' is chosen then it creates two histogram where one represents the top half of the image and the other bottom half of the image 
    else if (in == 'B' || in == 'b')
    {
        std::string folder_path = argv[1];
        std::string csv_file_path = "fe2h.csv";
        feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(folder_path, csv_file_path, in);
        cv::Mat image = imread(argv[2], cv::IMREAD_COLOR);

        // ---------------------------------------------------- Read the Bottom half of the image ---------------------------------------------------------------------//
        cv::Mat feature = image(cv::Rect(0, image.rows / 2, image.cols, image.rows / 2));

        // ---------------------------------------------------- Read the Top half of the image ------------------------------------------------------------------------//
        cv::Mat image1h = image(cv::Rect(0, 0, image.cols, image.rows / 2));

        target_multi_histogram3d(image1h, feature);
    }
    // If 'c' is chosen then it creates two histogram where one represents the RGB of the whole image and the other magnitude of the whole image 
    else if (in == 'C' || in == 'c')
    {
        std::string folder_path = argv[1];
        std::string csv_file_path = "fe2h.csv";
        feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(folder_path, csv_file_path, in);
        cv::Mat image = imread(argv[2], cv::IMREAD_COLOR);

        // ---------------------------------------------------- Read the RGB of the image ------------------------------------------------------------------------//
        cv::Mat image1h = image(cv::Rect(0, 0, image.cols, image.rows ));

        // ---------------------------------------------------- converting RGB to Magnitude image ---------------------------------------------------------------------//
        cv::Mat dst;
        magnitude(image,dst);
        //cv::Mat image = dst;
        cv::Mat feature = dst(cv::Rect(0, 0, dst.cols, dst.rows ));

        target_multi_histogram3d(image1h, feature);
    }
    // If 'd' is chosen then it creates 3 Histograms where 1 represents RGB of whole image, 2 represents Magnitude of whole image, 3 represents center 50*50 RGB 
    else if (in == 'D' || in == 'd')
    {
        std::string folder_path = argv[1];
        std::string csv_file_path = "fe2h.csv";
        feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(folder_path, csv_file_path, in);
        cv::Mat image = imread(argv[2], cv::IMREAD_COLOR);


        // ---------------------------------------------------- Read the RGB of the image  ------------------------------------------------------------------------//
        cv::Mat image1h = image(cv::Rect(0, 0, image.cols, image.rows ));

        // ---------------------------------------------------- converting RGB to Magnitude image -----------------------------------------------------------------//
        cv::Mat dst;
        // This function is used from the previous assignment (filter.cpp)
        magnitude(image,dst);
        //cv::Mat image = dst;
        cv::Mat feature = dst(cv::Rect(0, 0, dst.cols, dst.rows ));
        // ---------------------------------------------------- Creating a 50*50 mat from the image ---------------------------------------------------------------//
        cv::Mat feature1 = image(cv::Rect(image.cols / 2 - 4, image.rows / 2 - 4, 50, 50));
        target_triple_histogram3d(image1h, feature, feature1);
    }
    // If 'e' is chosen then it creates a 9*9 feature vector from the image  
    else if (in == 'E' || in == 'e')
    {
        std::string folder_path = argv[1];
        std::string csv_file_path = "fe2h.csv";
        feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(folder_path, csv_file_path, in);
        cv::Mat image = imread(argv[2], cv::IMREAD_COLOR);
        // This function creates the 9*9 feature vector
        extract_9times9_rgb_values_from_center_of_an_target_image(image);
    }
    // If 'f' is selected then performs RGB histogram matching 
    else if ( in =='F'||in =='f')
    {
        std::string folder_path = argv[1];
        std::string csv_file_path = "fe2h.csv";
        feature_vector_calculation_of_all_images_from_a_folder_Multi_Histogram(folder_path, csv_file_path, in);//
        //Mat image = imread("/home/warrior/CV_ASSIGNMENTS/white.png", IMREAD_COLOR);
        cv::Mat image = imread(argv[2], cv::IMREAD_COLOR);
        // This function creates a 3d histogram of a single image(Target image)
        target_single_histogram3d(image);
    }
    else
    {
        std::cout << "Try again " << std::endl;
    }
//-------------------------------------------------------------------------Distance metric---------------------------------------------------------------------------------------------//
    // Declaring the variable which represents the number of top/similar image to be calculated 
    double DISP_COUNT ;
    std::cout << "Enter the Number of closest image you want" << std::endl;
    // Getting the input of 'N' images 
    std::cin >> DISP_COUNT;
    // Reading the Target image feature vectors from the stored data i.e) from the saved csv file 
    std::ifstream file1("fe1h.csv");
    std::string line;
    // index of the column we want
    double column_index = 0;  
    // Declaring a vector to store target feature vector 
    std::vector<double> targetFeatureVector;
    // This function extracts the values of target feature vector
    extractValues(file1, 0, targetFeatureVector);

    for (const auto &value : targetFeatureVector) {
        //std::cout  << value <<  std::endl;
    }


    // Reading the image feature vectors from the stored data which was collected the image directory  i.e) from the saved csv file 
    std::ifstream file("fe2h.csv");
    std::vector<std::vector<double>> featureVectors;
    std::map<double, std::string> indexToImageName;

    extractFeatures(file, featureVectors, indexToImageName);
    // This function was created for debugging purpose
    printVectors(featureVectors);
    // Creating a map to calculate the distance values 
    std::map<double, double> indexToSsd;
    std::map<double, double> ssdToIndex;
    std::vector<double> ssdValues;
    // This function calculates the distance metric
    computeSsd(targetFeatureVector, featureVectors, ssdValues, indexToSsd, ssdToIndex, in );

    std::cout  << std::endl <<  std::endl;
    // Calculating and displaying the Top 'N' images requested 
    for (double idx = 0; idx < DISP_COUNT; idx++) {
        double ssdVal = ssdValues[idx];
        //std::cout  << idx + 1  << ".\t" << ssdVal << "\t:\t" << indexToImageName[ssdToIndex[ssdVal]] <<  std::endl;
        std::cout  << idx + 1  << ".\t" << "\t:\t" << indexToImageName[ssdToIndex[ssdVal]] <<  std::endl;
    }
    return 0;
}







