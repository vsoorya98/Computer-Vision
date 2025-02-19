//
// Created by SHIRISH & SOORYA on 4/12/23.
// This file contains the implementation of the functions declared in filters.h
// such as magnitude, blur5, gray_magnitude, thresholding, nightvision, Morphological and configure_serial
//

#include "filters.h"
#include <opencv2/opencv.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

// Used from project 1
// Magnitude image from X and Y sobel images
int magnitude(cv::Mat &src, cv::Mat &dst)
{
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel x
    cv::Mat first = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel x
    cv::Mat dst1 = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel y
    cv::Mat dss = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel y
    cv::Mat dss1 = cv::Mat::zeros(src.size(), CV_16SC3);
    //Performing SOBEL X
    for (int i = 1; i<src.rows-1;i++)
    {
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

        cv::Vec3s *dptr = first.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr[j][c] = (1*rptrm1[j-1][c]
                              +2*rptr[j-1][c]
                              +1*rptrp1[j-1][c])/4;
            }
        }
    }
    for (int i = 1; i<first.rows-1;i++)
    {
        cv::Vec3s *rptrm12 = first.ptr<cv::Vec3s>(i-1);
        cv::Vec3s *rptr2 = first.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp12 = first.ptr<cv::Vec3s>(i+1);

        cv::Vec3s *dptr2 = dst1.ptr<cv::Vec3s>(i);

        for (int j = 1; j<first.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr2[j][c] = (-1*rptrm12[j-1][c]   +0*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
            }
        }
    }
    //Performing SOBEL Y
    for (int i = 1; i<src.rows-1;i++)
    {
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

        cv::Vec3s *dptr = dss.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr[j][c] = (-1*rptrm1[j-1][c]
                              +0*rptr[j-1][c]
                              +1*rptrp1[j-1][c])/4;
            }
        }
    }
    for (int i = 1; i<dss1.rows-1;i++)
    {
        cv::Vec3s *rptrm12 = dss.ptr<cv::Vec3s>(i-1);
        cv::Vec3s *rptr2 = dss.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp12 = dss.ptr<cv::Vec3s>(i+1);

        cv::Vec3s *dptr2 = dss1.ptr<cv::Vec3s>(i);

        for (int j = 1; j<dss.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr2[j][c] = (1*rptrm12[j-1][c]   +2*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
            }
        }
    }
    //Declaring Mat for scaling calculations
    cv::Mat dis;
    cv::Mat dis1;
    cv::Mat diss;
    cv::Mat diss1;

    //X SOBEL
    cv::convertScaleAbs(first,dis);
    cv::convertScaleAbs(dst1,dis1);
    //Y SOBEL
    cv::convertScaleAbs(dss,diss);
    cv::convertScaleAbs(dss1,diss1);

    cv::Mat xsob = dis1;
    cv::Mat ysob = diss1;
    //Converting thier data type and taking thier sum and square root

    dis1.convertTo(dis1, CV_32F);
    diss1.convertTo(diss1, CV_32F);

    cv::multiply(dis1,dis1,dis1);
    cv::multiply(diss1,diss1,diss1);
    cv::addWeighted(dis1, 0.5, diss1, 0.5, 0, dst);
    cv::sqrt(dst,dst);

    dst.convertTo(dst, CV_8U);
    return 0;
}



// Used from project 1
// Blur using 5*5 kernal
int blur5( cv::Mat &src, cv::Mat &dst )
{
    // Declaring the first processed image i.e) after perfroming 5*1 Multiplication
    cv::Mat first = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*5 Multiplication
    cv::Mat dst1 = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the scaled image Mat
    cv::Mat dis;

    for (int i = 0; i<src.rows;i++)
    {

        //Performing 5*1 Multiplication

        cv::Vec3b *rptrm2 = src.ptr<cv::Vec3b>(i-2);
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);
        cv::Vec3b *rptrp2 = src.ptr<cv::Vec3b>(i+2);

        cv::Vec3s *dptr = first.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c1 = 0; c1<3; c1++)
            {
                //Changing the boundary values to 255
                if ( i == 0 || j == 0 || i == src.rows -1 || j == src.cols-1 )
                {
                    dptr[j][c1] = 255;
                    dptr[i][c1] = 255;
                }
                else
                {
                    dptr[j][c1] = (      1*rptrm2[j-2][c1]
                                         +4*rptrm1[j-1][c1]
                                         +6*rptr[j-1][c1]
                                         +4*rptrp1[j-1][c1]
                                         +1*rptrp2[j-2][c1])/16;

                }

            }
        }
    }

    //Performing 1*5 Multiplication
    for (int i1 = 0; i1<first.rows;i1++)
    {
        cv::Vec3s *rptrm13 = first.ptr<cv::Vec3s>(i1-2);
        cv::Vec3s *rptrm12 = first.ptr<cv::Vec3s>(i1-1);
        cv::Vec3s *rptr2 = first.ptr<cv::Vec3s>(i1);
        cv::Vec3s *rptrp12 = first.ptr<cv::Vec3s>(i1+1);
        cv::Vec3s *rptrp13 = first.ptr<cv::Vec3s>(i1+2);

        cv::Vec3s *dptr2 = dst1.ptr<cv::Vec3s>(i1);

        for (int j1 = 1; j1<first.cols-1;j1++)
        {
            for (int c = 0; c<3; c++)
            {
                //Changing the boundary values to 255
                if ( i1 == 0 || j1 == 0 || i1 == src.rows -1|| j1 == src.cols -1 )
                {
                    dptr2[j1][c] = 255;
                    dptr2[i1][c] = 255;
                }
                else
                {
                    dptr2[j1][c] = (1*rptrm12[j1-2][c] + 4*rptrm12[j1-1][c]  +6*rptrm12[j1][c]  +4*rptrm12[j1+1][c] + 1*rptrm12[j1+2][c])/16;
                }

            }
        }
    }
    //Scaling back the values
    cv::convertScaleAbs(first,dis);
    cv::convertScaleAbs(dst1,dst);
    return 0;
}


int gray_magnitude(cv::Mat &src, cv::Mat &dst) {
    // Ensure the input image is grayscale
    if (src.channels() != 1) {
        return -1;
    }

    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel x
    cv::Mat first = cv::Mat::zeros(src.size(), CV_16SC1);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel x
    cv::Mat dst1 = cv::Mat::zeros(src.size(), CV_16SC1);
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel y
    cv::Mat dss = cv::Mat::zeros(src.size(), CV_16SC1);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel y
    cv::Mat dss1 = cv::Mat::zeros(src.size(), CV_16SC1);

    // Performing SOBEL X
    for (int i = 1; i < src.rows - 1; i++) {
        auto *rptrm1 = src.ptr<uchar>(i - 1);
        auto *rptr = src.ptr<uchar>(i);
        auto *rptrp1 = src.ptr<uchar>(i + 1);

        auto *dptr = first.ptr<short>(i);

        for (int j = 1; j < src.cols - 1; j++) {
            dptr[j] = (1 * rptrm1[j - 1]
                       + 2 * rptr[j - 1]
                       + 1 * rptrp1[j - 1]) / 4;
        }
    }

    for (int i = 1; i < first.rows - 1; i++) {
        auto *rptrm12 = first.ptr<short>(i - 1);
        auto *rptr2 = first.ptr<short>(i);
        auto *rptrp12 = first.ptr<short>(i + 1);

        auto *dptr2 = dst1.ptr<short>(i);

        for (int j = 1; j < first.cols - 1; j++) {
            dptr2[j] = (-1 * rptrm12[j - 1] + 0 * rptrm12[j] + 1 * rptrm12[j + 1]) / 1;
        }
    }

    // Performing SOBEL Y
    for (int i = 1; i < src.rows - 1; i++) {
        auto *rptrm1 = src.ptr<uchar>(i - 1);
        auto *rptr = src.ptr<uchar>(i);
        auto *rptrp1 = src.ptr<uchar>(i + 1);

        auto *dptr = dss.ptr<short>(i);

        for (int j = 1; j < src.cols - 1; j++) {
            dptr[j] = (-1 * rptrm1[j - 1]
                       + 0 * rptr[j - 1]
                       + 1 * rptrp1[j - 1]) / 4;
        }
    }

    for (int i = 1; i < dss1.rows - 1; i++) {
        auto *rptrm12 = dss.ptr<short>(i - 1);
        auto *rptr2 = dss.ptr<short>(i);
        auto *rptrp12 = dss.ptr<short>(i + 1);
        auto *dptr2 = dss1.ptr<short>(i);

        for (int j = 1; j < dss.cols - 1; j++) {
            dptr2[j] = (1 * rptrm12[j - 1] + 2 * rptrm12[j] + 1 * rptrm12[j + 1]) / 1;
        }
    }

// Declaring Mat for scaling calculations
    cv::Mat dis;
    cv::Mat dis1;
    cv::Mat diss;
    cv::Mat diss1;

// X SOBEL
    cv::convertScaleAbs(first, dis);
    cv::convertScaleAbs(dst1, dis1);
// Y SOBEL
    cv::convertScaleAbs(dss, diss);
    cv::convertScaleAbs(dss1, diss1);

    cv::Mat xsob = dis1;
    cv::Mat ysob = diss1;

// Converting their data type and taking their sum and square root
    dis1.convertTo(dis1, CV_32F);
    diss1.convertTo(diss1, CV_32F);

    cv::multiply(dis1, dis1, dis1);
    cv::multiply(diss1, diss1, diss1);
    cv::addWeighted(dis1, 0.5, diss1, 0.5, 0, dst);
    cv::sqrt(dst, dst);

    dst.convertTo(dst, CV_8U);
    return 0;
}


// Thresholding a grayscale image
int thresholding(cv::Mat& src, cv::Mat& dst)
{
    // Apply Binary thresholding to the input image
    cv::Mat thresholded_image;
    cv::threshold(src, dst, 50, 255, cv::THRESH_BINARY);
    return 0;
}

int nightvision(cv::Mat &src, cv::Mat &dst)
{

    // Apply histogram equalization to enhance the image
    cv::Mat eq;
    equalizeHist(src, eq);

    // Apply gamma correction to enhance the image
    cv::Mat gamma;
    double gamma_value = 1.5;
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
    {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_value) * 255.0);
    }
    cv::LUT(eq, lookUpTable, gamma);

    // Apply bilateral filter to reduce noise
    cv::Mat bilateral;
    int d = 9; // diameter of each pixel neighborhood
    double sigmaColor = 9; // filter sigma in the color space
    double sigmaSpace = 7; // filter sigma in the coordinate space
    bilateralFilter(gamma, bilateral, d, sigmaColor, sigmaSpace);
    //applying the gray_magnitude function to the enhanced image
    gray_magnitude(bilateral, dst);
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
                if (processedImage.at<uchar>(y, x) == 0) {
                    // Performing the 4 connected dilation process
                    if (y > 0) tempImage.at<uchar>(y-1, x) = 0;
                    if (y < processedImage.rows-1) tempImage.at<uchar>(y+1, x) = 0;
                    if (x > 0) tempImage.at<uchar>(y, x-1) = 0;
                    if (x < processedImage.cols-1) tempImage.at<uchar>(y, x+1) = 0;
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
                if (processedImage.at<uchar>(y, x) == 255) {
                    // Performing the 4 connected erosion process
                    if (y > 0 && processedImage.at<uchar>(y-1, x) == 255);
                    if (y < processedImage.rows-1 && processedImage.at<uchar>(y+1, x) == 255);
                    if (x > 0 && processedImage.at<uchar>(y, x-1) == 255) ;
                    if (x < processedImage.cols-1 && processedImage.at<uchar>(y, x+1) == 255);
                    tempImage.at<uchar>(y, x) = 255;
                }
            }
        }
        processedImage = tempImage;
    }

    return processedImage;
}

//Reference
//https://man7.org/linux/man-pages/man3/termios.3.html
//https://en.wikibooks.org/wiki/Serial_Programming
//https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
//Function to establish serial communication
int configure_serial(const char *port)
{
    //Opening the serial port (currently set to an standard FTDI USB-UART cable type device)
    int fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    //Checking if the port is open
    if (fd < 0)
    {
        std::cout << "Error opening " << port << ": " << strerror(errno) << std::endl;
        return -1;
    }
    //Setting the serial port parameters
    struct termios tty;
    //Getting the current serial port parameters
    memset(&tty, 0, sizeof tty);
    //Checking if the current serial port parameters are correct
    if (tcgetattr(fd, &tty) != 0) {
        std::cout << "Error from tcgetattr: " << strerror(errno) << std::endl;
        return -1;
    }
    //Reference
    //https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
    //Setting the serial port parameters
    cfsetospeed(&tty, (speed_t)B9600);
    cfsetispeed(&tty, (speed_t)B9600);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cout << "Error from tcsetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    return fd;
}