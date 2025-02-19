/*
Created by SHIRISH & SOORYA on 4/12/23.
This program is used to apply different filters to the live video stream to create background image and perform saturated subtraction
if you press 'g' or 'G' it will apply gray scale filter
if you press 'n' or 'N' it will apply normal filter
if you press 'b' or 'B' it will create background image
if you press 's' or 'S' it will perform saturated subtraction
if you press 'q' or 'Q' it will quit the program
if you press 't' or 'T' it will apply thresholding
if you press 'm' or 'M' it will apply magnitude
if you press 'x' or 'X' it will display all the process from the beginning and display the saturated difference image
if you press 'f' or 'F' it will turn on the external device like fan, light, etc using arduino
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <QtWidgets>
#include <QApplication>
#include "filters.h"
#include <unistd.h>
#include <thread>


int main(int argc, char** argv)
{
    //Setting up the serial communication with arduino
    int serial_fd = configure_serial("/dev/cu.usbmodem14301");
    //Checking if the serial communication is established
    if (serial_fd < 0)
    {
        std::cerr << "Failed to configure serial communication" << std::endl;
        return -1;
    }
    //Setting up the GUI
    QApplication app(argc, argv);
    // Setting up the live video stream
    cv::VideoCapture *capdev;
    capdev = new cv::VideoCapture(0);
    // Setting the frame/window size
    capdev->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    capdev->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    // Setting the frame rate per second
    capdev->set(cv::CAP_PROP_FPS, 30);
    //Setting the number of consecutive images to be captured
    int number_of_concequitive_images = 5;
    //Checking if the camera is opened
    if (!capdev->isOpened())
    {
        std::cout << "Can't access the default camera" << std::endl;
        return -1;
    }
    // Defining global Matrix for live stream and final modified frames
    cv::Mat frame;
    cv::Mat filter_frame;
    cv::Mat dst;
    *capdev >> frame;
    cv::Mat src = frame;
    int Mode = 0;
    //Defining the type of process to be performed
    while (true)
    {
        *capdev >> frame;
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        if (!frame.empty()) {
            //Applying gray scale filter
            if (Mode == 0)
            {
                cv::Mat dst1, dst2;
                cv::Mat src = frame;
                cv::cvtColor(src, dst1, cv::COLOR_BGR2GRAY);
                cv::imshow("Live Video", dst1);
                filter_frame = dst1;
            }
            //Normal image
            else if (Mode == 1)
            {
                cv::Mat src = frame;
                cv::imshow("Live Video", src);
                filter_frame = src;
            }
            //Collecting the background image and displaying the first consecutive image
            else if (Mode == 2)
            {
                cv::Mat bkg_ground_img;
                cv::Mat dst1, dst2;
                cv::Mat src = frame;
                cv::cvtColor(src, dst1, cv::COLOR_BGR2GRAY);
                //Creating a background image by calculating the standard deviation of each pixel in the image
                //Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];
                // Capture the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    //Capture the consecutive images
                    *capdev >> img[i];
                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                }

                //Display the first consecutive image
                if (!img[0].empty())
                {
                    //conveting them to gray scale
                    cv::cvtColor(img[0], dst2, cv::COLOR_BGR2GRAY);
                    cv::imshow("Live Video", dst2);
                    filter_frame = dst2;
                }
                else
                {
                    std::cerr << "First consecutive image is empty" << std::endl;
                }
            }
            //Reference
            //https://learnopencv.com/simple-background-estimation-in-videos-using-opencv-c-python/
            //Theory reference to understand the concept
            //Lecture notes of Prof Bruce Maxwell & https://www.researchgate.net/post/What-does-Standard-deviation-of-image-signifies
            //Calculating the per pixel standard deviation of each and every pixel and displaying the background image
            //by making the pixels with standard deviation less than threshold as the background image pixels
            else if (Mode == 3)
            {
                cv::Mat dst1, dst2;
                cv::Mat src = frame;
                cv::cvtColor(src, dst1, cv::COLOR_BGR2GRAY);
                //Creating a background image by calculating the standard deviation of each pixel in the image
                cv::Mat bkg_ground_img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

                //Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];

                //Capturing the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    *capdev >> img[i];
                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                    cv::cvtColor(img[i], img[i], cv::COLOR_BGR2GRAY);
                }
                //Declaring the threshold value for the standard deviation
                double threshold = 10.0;
                //Calculating the per-pixel standard deviation
                for (int row = 0; row < src.rows; row++)
                {
                    for (int col = 0; col < src.cols; col++)
                    {
                        //Declaring the vector to store the pixel values of the consecutive images
                        std::vector<double> pixel_values;
                        for (int i = 0; i < number_of_concequitive_images; i++)
                        {
                            pixel_values.push_back(static_cast<double>(img[i].at<uchar>(row, col)));
                        }
                        //Calculating the mean and variance
                        double mean = std::accumulate(pixel_values.begin(), pixel_values.end(), 0.0) / pixel_values.size();
                        double variance = 0.0;
                        //Calculating the variance
                        for (const double& value : pixel_values)
                        {
                            variance += (value - mean) * (value - mean);
                        }
                        variance /= pixel_values.size();
                        double stddev = std::sqrt(variance);
                        //Checking if the standard deviation is less than the threshold if yes then the pixel mean is the background pixel
                        if (stddev < threshold)
                        {
                            bkg_ground_img.at<uchar>(row, col) = static_cast<uchar>(mean);
                        }
                    }
                }
                //Displaying the bkg_ground_img
                cv::imshow("Live Video", bkg_ground_img);
                filter_frame = bkg_ground_img;
            }
            //Calculating the per pixel standard deviation of each and every pixel and displaying the background image by applying the magnitude filter
            else if (Mode == 4)
            {
                cv::Mat dst1, dst2;
                cv::Mat src = frame;
                cv::cvtColor(src, dst1, cv::COLOR_BGR2GRAY);
                //Creating a background image by calculating the standard deviation of each pixel in the image
                cv::Mat bkg_ground_img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

                //Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];

                //Capturing the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    *capdev >> img[i];
                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                    cv::cvtColor(img[i], img[i], cv::COLOR_BGR2GRAY);
                }
                //Declaring the threshold value for the standard deviation
                double threshold = 10.0;

                //Calculating the per-pixel standard deviation
                for (int row = 0; row < src.rows; row++)
                {
                    for (int col = 0; col < src.cols; col++)
                    {
                        //Declaring the vector to store the pixel values of the consecutive images
                        std::vector<double> pixel_values;
                        for (int i = 0; i < number_of_concequitive_images; i++)
                        {
                            pixel_values.push_back(static_cast<double>(img[i].at<uchar>(row, col)));
                        }
                        //Calculating the mean and variance
                        double mean = std::accumulate(pixel_values.begin(), pixel_values.end(), 0.0) / pixel_values.size();
                        double variance = 0.0;
                        for (const double& value : pixel_values)
                        {
                            variance += (value - mean) * (value - mean);
                        }
                        variance /= pixel_values.size();
                        double stddev = std::sqrt(variance);
                        //Checking if the standard deviation is less than the threshold if yes then the pixel mean is the background pixel
                        if (stddev < threshold)
                        {
                            bkg_ground_img.at<uchar>(row, col) = static_cast<uchar>(mean);
                        }
                    }
                }
                //Display the bkg_ground_img
                //Applying gradient magnitude filter
                gray_magnitude(bkg_ground_img, dst2);
                cv::imshow("Live Video", dst2);
                filter_frame = dst2;
            }
            //Calculating the per pixel standard deviation of each and every pixel and displaying the background image by applying the magnitude filter and the threshold filter
            else if (Mode == 5)
            {
                cv::Mat dst1, dst2;
                cv::Mat src = frame;
                cv::cvtColor(src, dst1, cv::COLOR_BGR2GRAY);
                //Creating a background image by calculating the standard deviation of each pixel in the image
                cv::Mat bkg_ground_img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

                //Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];

                //Capturing the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    *capdev >> img[i];
                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                    cv::cvtColor(img[i], img[i], cv::COLOR_BGR2GRAY);
                }

                double threshold = 10.0;

                //Calculating the per-pixel standard deviation
                for (int row = 0; row < src.rows; row++)
                {
                    for (int col = 0; col < src.cols; col++)
                    {
                        //Declaring the vector to store the pixel values of the consecutive images
                        std::vector<double> pixel_values;
                        for (int i = 0; i < number_of_concequitive_images; i++)
                        {
                            pixel_values.push_back(static_cast<double>(img[i].at<uchar>(row, col)));
                        }
                        //Calculating the mean and variance
                        double mean = std::accumulate(pixel_values.begin(), pixel_values.end(), 0.0) / pixel_values.size();
                        double variance = 0.0;
                        for (const double& value : pixel_values)
                        {
                            variance += (value - mean) * (value - mean);
                        }
                        variance /= pixel_values.size();
                        double stddev = std::sqrt(variance);
                        //Checking if the standard deviation is less than the threshold if yes then the pixel mean is the background pixel
                        if (stddev < threshold)
                        {
                            bkg_ground_img.at<uchar>(row, col) = static_cast<uchar>(mean);
                        }
                    }
                }
                //Display the bkg_ground_img
                //Applying gradient magnitude filter
                gray_magnitude(bkg_ground_img, dst2);
                cv::Mat dst3;
                //Applying threshold filter
                thresholding(dst2, dst3);
                cv::imshow("Live Video", dst3);
                filter_frame = dst3;

            }
            //Calculating the per pixel standard deviation of each and every pixel and displaying the background image by
            //applying the magnitude filter and the threshold filter and the night vision filter
            else if (Mode == 6)
            {
                cv::Mat dst1, dst2, dst11;
                cv::Mat src = frame;
                cv::cvtColor(src, dst11, cv::COLOR_BGR2GRAY);
                nightvision(dst11, dst1);
                //Creating a background image by calculating the standard deviation of each pixel in the image
                cv::Mat bkg_ground_img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

                //Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];

                //Capturing the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    *capdev >> img[i];
                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                    cv::cvtColor(img[i], img[i], cv::COLOR_BGR2GRAY);
                }

                double threshold = 10.0;

                //Calculating the per-pixel standard deviation
                for (int row = 0; row < src.rows; row++)
                {
                    for (int col = 0; col < src.cols; col++)
                    {
                        //Declaring the vector to store the pixel values of the consecutive images
                        std::vector<double> pixel_values;
                        for (int i = 0; i < number_of_concequitive_images; i++)
                        {
                            pixel_values.push_back(static_cast<double>(img[i].at<uchar>(row, col)));
                        }
                        //Calculating the mean and variance
                        double mean = std::accumulate(pixel_values.begin(), pixel_values.end(), 0.0) / pixel_values.size();
                        double variance = 0.0;
                        for (const double& value : pixel_values)
                        {
                            variance += (value - mean) * (value - mean);
                        }
                        variance /= pixel_values.size();
                        double stddev = std::sqrt(variance);
                        //Checking if the standard deviation is less than the threshold if yes then the pixel mean is the background pixel
                        if (stddev < threshold)
                        {
                            bkg_ground_img.at<uchar>(row, col) = static_cast<uchar>(mean);
                        }
                    }
                }
                //Display the bkg_ground_img
                //Applying gradient magnitude filter
                gray_magnitude(bkg_ground_img, dst2);
                cv::Mat dst3;
                //Applying threshold filter
                thresholding(dst2, dst3);
                cv::imshow("Live Video", dst3);
                filter_frame = dst3;
            }

            else if (Mode == 7)
            {
                cv::Mat dst1, dst2, dst11, dst1_dup, dst2_dup, dst11_dup;
                cv::Mat src_dup ;
                *capdev >> src_dup;
                cv::Mat src = frame.clone();

                cv::cvtColor(src, dst11, cv::COLOR_BGR2GRAY);
                cv::cvtColor(frame, dst11_dup, cv::COLOR_BGR2GRAY);
                nightvision(dst11, dst1);
                nightvision(dst11_dup, dst1_dup);
                // Creating a background image by calculating the standard deviation of each pixel in the image
                cv::Mat bkg_ground_img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

                // Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];

                // Capture the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    *capdev >> img[i];
                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                    cv::cvtColor(img[i], img[i], cv::COLOR_BGR2GRAY);
                }

                double threshold = 15.0;

                // Calculate the per-pixel standard deviation
                for (int row = 0; row < src.rows; row++)
                {
                    for (int col = 0; col < src.cols; col++)
                    {
                        std::vector<double> pixel_values;
                        for (int i = 0; i < number_of_concequitive_images; i++)
                        {
                            pixel_values.push_back(static_cast<double>(img[i].at<uchar>(row, col)));
                        }

                        double mean = std::accumulate(pixel_values.begin(), pixel_values.end(), 0.0) / pixel_values.size();
                        double variance = 0.0;
                        for (const double& value : pixel_values)
                        {
                            variance += (value - mean) * (value - mean);
                        }
                        variance /= pixel_values.size();
                        double stddev = std::sqrt(variance);

                        if (stddev < threshold)
                        {
                            bkg_ground_img.at<uchar>(row, col) = static_cast<uchar>(img[4].at<uchar>(row, col));
                        }
                    }
                }

                //performing saturated subtraction between result and result1 frames
                cv::Mat result2, result3, result4 ;
                cv::subtract(bkg_ground_img, dst11_dup, result2);
                //Display the bkg_ground_img
                //Applying gradient magnitude filter

                gray_magnitude(result2, result3);
                thresholding(result2, result4);
                cv::namedWindow("Background image", cv::WINDOW_AUTOSIZE);
                cv::imshow("Background image", bkg_ground_img);
                //Dsiplaying the live stream in a new window
                cv::imshow("Live Video", frame);
                cv::namedWindow("Detecting changes", cv::WINDOW_AUTOSIZE);
                cv::imshow("Detecting changes", result4);


            }
            //Triggering the Device to turn on if the detected pixels count are greater than 100
            else if (Mode == 8)
            {
                cv::Mat dst1, dst2, dst11, dst1_dup, dst2_dup, dst11_dup;
                cv::Mat src_dup ;
                *capdev >> src_dup;
                cv::Mat src = frame.clone();

                cv::cvtColor(src, dst11, cv::COLOR_BGR2GRAY);
                cv::cvtColor(frame, dst11_dup, cv::COLOR_BGR2GRAY);
                nightvision(dst11, dst1);
                nightvision(dst11_dup, dst1_dup);
                // Creating a background image by calculating the standard deviation of each pixel in the image
                cv::Mat bkg_ground_img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

                // Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];

                // Capture the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    *capdev >> img[i];
                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                    cv::cvtColor(img[i], img[i], cv::COLOR_BGR2GRAY);
                }

                double threshold = 15.0;

                // Calculate the per-pixel standard deviation
                for (int row = 0; row < src.rows; row++)
                {
                    for (int col = 0; col < src.cols; col++)
                    {
                        std::vector<double> pixel_values;
                        for (int i = 0; i < number_of_concequitive_images; i++)
                        {
                            pixel_values.push_back(static_cast<double>(img[i].at<uchar>(row, col)));
                        }

                        double mean = std::accumulate(pixel_values.begin(), pixel_values.end(), 0.0) / pixel_values.size();
                        double variance = 0.0;
                        for (const double& value : pixel_values)
                        {
                            variance += (value - mean) * (value - mean);
                        }
                        variance /= pixel_values.size();
                        double stddev = std::sqrt(variance);

                        if (stddev < threshold)
                        {
                            bkg_ground_img.at<uchar>(row, col) = static_cast<uchar>(img[4].at<uchar>(row, col));
                        }
                    }
                }

                //performing saturated subtraction between result and result1 frames
                cv::Mat result2, result3, result4 ;
                cv::subtract(bkg_ground_img, dst11_dup, result2);
                //Display the bkg_ground_img
                //Applying gradient magnitude filter

                gray_magnitude(result2, result3);
                thresholding(result2, result4);
                //Checking for the number of white pixels in the image
                int white_pixel_count = 0;
                for (int row = 0; row < result4.rows; row++)
                {
                    for (int col = 0; col < result4.cols; col++)
                    {
                        if (result4.at<uchar>(row, col) == 255)
                        {
                            white_pixel_count++;
                        }
                    }
                }
                //Checking if the number of white pixels is greater than 1000
                if (white_pixel_count > 100)
                {
                    std::cout << "Motion Detected" << std::endl;
                    // Digital pin 13 turn on (high) 
                    write(serial_fd, "H", 1);
                    
                }
                else
                {
                    std::cout << "No Motion Detected" << std::endl;
                    // Digital pin 13 turn on (low) 
                    write(serial_fd, "L", 1);
                }
                //Displaying the live stream in a new window
                cv::namedWindow("Detecting changes", cv::WINDOW_AUTOSIZE);
                cv::imshow("Detecting changes", result4);
                cv::imshow("Live Video", frame);
                filter_frame = src;


            }
            else if (Mode == 9)
            {
                //std::thread worker(get_live_frames_through_multi_threading)
                
                cv::Mat dst1, dst2, dst11, dst1_dup, dst2_dup, dst11_dup;
                cv::Mat src_dup ;
                *capdev >> src_dup;
                cv::Mat src = src_dup.clone();

                cv::cvtColor(src, dst11, cv::COLOR_BGR2GRAY);
                cv::cvtColor(src_dup, dst11_dup, cv::COLOR_BGR2GRAY);
                nightvision(dst11, dst1);
                nightvision(dst11_dup, dst1_dup);
                // Creating a background image by calculating the standard deviation of each pixel in the image
                cv::Mat bkg_ground_img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

                // Declaring the cv::Mat for the consecutive images
                cv::Mat img[number_of_concequitive_images];

                // Capture the consecutive images
                for (int i = 0; i < number_of_concequitive_images; i++)
                {
                    *capdev >> img[i];


                    if (img[i].empty())
                    {
                        std::cerr << "Failed to capture an image" << std::endl;
                        break;
                    }
                    cv::cvtColor(img[i], img[i], cv::COLOR_BGR2GRAY);
                }

                double threshold = 15.0;

                // Calculate the per-pixel standard deviation
                for (int row = 0; row < src.rows; row++)
                {
                    for (int col = 0; col < src.cols; col++)
                    {
                        std::vector<double> pixel_values;
                        for (int i = 0; i < number_of_concequitive_images; i++)
                        {
                            pixel_values.push_back(static_cast<double>(img[i].at<uchar>(row, col)));
                        }

                        double mean = std::accumulate(pixel_values.begin(), pixel_values.end(), 0.0) / pixel_values.size();
                        double variance = 0.0;
                        for (const double& value : pixel_values)
                        {
                            variance += (value - mean) * (value - mean);
                        }
                        variance /= pixel_values.size();
                        double stddev = std::sqrt(variance);

                        if (stddev < threshold)
                        {
                            bkg_ground_img.at<uchar>(row, col) = static_cast<uchar>(img[4].at<uchar>(row, col));
                        }
                    }
                }

                //performing saturated subtraction between result and result1 frames
                cv::Mat result2, result3, result4 ;
                cv::subtract(bkg_ground_img, dst11_dup, result2);
                //Display the bkg_ground_img
                //Applying gradient magnitude filter
                gray_magnitude(result2, result3);
                thresholding(result2, result4);
                //Checking for the number of white pixels in the image
                int white_pixel_count = 0;
                for (int row = 0; row < result4.rows; row++)
                {
                    for (int col = 0; col < result4.cols; col++)
                    {
                        if (result4.at<uchar>(row, col) == 255)
                        {
                            white_pixel_count++;
                        }
                    }
                }
                //Checking if the number of white pixels is greater than 1000
                if (white_pixel_count > 10)
                {
                    std::cout << "Motion Detected" << std::endl;
                    // Digital pin 13 turn on (high) 
                    write(serial_fd, "H", 1);
                    
                }
                else
                {
                    std::cout << "No Motion Detected" << std::endl;
                    // Digital pin 13 turn on (low) 
                    write(serial_fd, "L", 1);
                }
                cv::namedWindow("Background image", cv::WINDOW_AUTOSIZE);
                cv::namedWindow("Normal colored Background image", cv::WINDOW_AUTOSIZE);
                cv::imshow("Background image", bkg_ground_img);
                //Dsiplaying the live stream in a new window
                cv::imshow("Normal colored Background image", frame);
                cv::namedWindow("Detecting changes", cv::WINDOW_AUTOSIZE);
                cv::imshow("Detecting changes", result4);
            }

        

        }
        // Checking for the key press
        char key = cv::waitKey(30);

        if (key == 'q' || key == 'Q')
        {
            break;
        }
        if (key == 'g' || key == 'G')
        {
            Mode = 0;
        }
        if (key == 'n' || key == 'N')
        {
            Mode = 1;
        }
        if (key == 'd' || key == 'D')
        {
            Mode = 2;
        }
        if (key == 'b' || key == 'B')
        {
            Mode = 3;
        }
        if (key == 'm' || key == 'M')
        {
            Mode = 4;
        }
        if (key == 't' || key == 'T')
        {
            Mode = 5;
        }
        if (key == 'z' || key == 'Z')
        {
            Mode = 6;
        }
        if (key == 'x' || key == 'X')
        {
            Mode = 7;
        }
        if (key == 'c' || key == 'C')
        {
            Mode = 8;
        }
        if (key == 'f' || key == 'F')
        {
            Mode = 9;
        }

    }

    close(serial_fd);    
    return 0;
}
