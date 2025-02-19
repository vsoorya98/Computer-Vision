/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * This program file contains thresholding function which convert grayscale to binary image
 * This program file also contains OTSU grayscale to binary image conversion
 * We also perform Morphological process in this file
 */

#include <opencv2/opencv.hpp>




// Converting a blur image to binary thresholded image
int thresholding( cv::Mat &src, cv::Mat &dst )
{
    float thres = 127.5;
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            // accessing every channel
            cv::Vec3b img = src.at<cv::Vec3b>(i, j);
            // Modifying the color channel weights
            uchar blue = img[0];
            uchar green = img[1];
            uchar red = img[2];
            // Converting the image from 3 channel to Single channel i.e) greyscale
            uchar gray = (blue + green + red) / 3;
            // thresholding the image to 0/255 based on the threshold value
            if (gray >= thres)
            {
                gray = 0;
            }
            else if (gray < thres)
            {
                gray = 255;
            }
            dst.at<uchar>(i, j) = gray;
        }
    }
    return 0;
}

// OTSU thresholding
int thresholding1( cv::Mat &src, cv::Mat &dst )
{
    cv::Mat grayscale_image;
    cv::cvtColor(src, grayscale_image, cv::COLOR_BGR2GRAY);
    // Apply Otsu thresholding to the input image
    cv::Mat thresholded_image;
    cv::threshold(grayscale_image, dst, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    return 0;
}

// Performing Morphological processing first 4 connected dilation (2 times) then 4 connected erosion (2 times)
cv::Mat Morphological(const cv::Mat& image)
{
    // Defining the number of iterations
    int iterations = 3;
    cv::Mat processedImage = image.clone();
    // Apply dilation two times
    for (int i = 0; i < iterations; i++) {
        cv::Mat tempImage = processedImage.clone();
        for (int y = 0; y < processedImage.rows; y++) {
            for (int x = 0; x < processedImage.cols; x++) {
                // Checking if the pixel is foreground
                if (processedImage.at<uchar>(y, x) == 255) {
                    // Performing the 4 connected dilation process
                    if (y > 0) tempImage.at<uchar>(y-1, x) = 255;
                    if (y < processedImage.rows-1) tempImage.at<uchar>(y+1, x) = 255;
                    if (x > 0) tempImage.at<uchar>(y, x-1) = 255;
                    if (x < processedImage.cols-1) tempImage.at<uchar>(y, x+1) = 255;
                }
            }
        }
        processedImage = tempImage;
    }

    // Apply erosion two times
    for (int i = 0; i < iterations; i++) {
        cv::Mat tempImage = processedImage.clone();
        for (int y = 0; y < processedImage.rows; y++) {
            for (int x = 0; x < processedImage.cols; x++) {
                // Checking if the pixel is background
                if (processedImage.at<uchar>(y, x) == 0) {
                    // Performing the 4 connected erosion process
                    if (y > 0 && processedImage.at<uchar>(y-1, x) == 0);
                    if (y < processedImage.rows-1 && processedImage.at<uchar>(y+1, x) == 0);
                    if (x > 0 && processedImage.at<uchar>(y, x-1) == 0) ;
                    if (x < processedImage.cols-1 && processedImage.at<uchar>(y, x+1) == 0);
                    tempImage.at<uchar>(y, x) = 0;
                }
            }
        }
        processedImage = tempImage;
    }

    return processedImage;
}
