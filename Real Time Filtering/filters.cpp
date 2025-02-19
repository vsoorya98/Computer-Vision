#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include<cstring>
#include<cstdio>
#include<math.h>
#include "filters.h"



using namespace cv;
using namespace std;

int alt_grey(Mat& src, Mat& dst) {
    dst = Mat::zeros(src.size(), src.type());

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            Vec3b p = src.at<Vec3b>(i, j);
            int p1 = p[2];
            dst.at<Vec3b>(i, j) = Vec3b(p1,p1,p1);
        }
    }
    return 0;
}

int blur5x5(cv::Mat& src, cv::Mat& dst)
{
    dst = cv::Mat(src.size(), src.type());
    int h = src.rows;
    int w = src.cols;
    if (h != dst.rows || w != dst.cols) //check for same dimension
    {
        return -1;
    }

    int ker1[5] = { 1,2,4,2,1 }; //defining filter values
    int ker2[5] = { 1,2,4,2,1 };

    cv::Mat temp = cv::Mat::zeros(src.size(), src.type());

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for (int i = 0; i < 5; i++)
            {
                int x_p = x + i - 2;
                if (x_p < 0 || x_p >= w)
                    continue;

                // Multiply filter with the pixel value
                r += ker1[i] * src.at<cv::Vec3b>(y, x_p)[0]; //normalising it by factor of 10 (sum of positive coeff of the filter =10)
                g += ker1[i] * src.at<cv::Vec3b>(y, x_p)[1];
                b += ker1[i] * src.at<cv::Vec3b>(y, x_p)[2];
            }

            temp.at<cv::Vec3b>(y, x)[0] = r / 10;
            temp.at<cv::Vec3b>(y, x)[1] = g / 10;
            temp.at<cv::Vec3b>(y, x)[2] = b / 10;
        }
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for (int i = 0; i < 5; i++)
            {
                int y_p = y + i - 2;
                if (y_p < 0 || y_p >= h)
                    continue;

                r += ker2[i] * temp.at<cv::Vec3b>(y_p, x)[0];
                g += ker2[i] * temp.at<cv::Vec3b>(y_p, x)[1];
                b += ker2[i] * temp.at<cv::Vec3b>(y_p, x)[2];
            }

            dst.at<cv::Vec3b>(y, x)[0] = r / 10;
            dst.at<cv::Vec3b>(y, x)[1] = g / 10;
            dst.at<cv::Vec3b>(y, x)[2] = b / 10;
        }
    }

    return 0;
}

int sobelX3x3(cv::Mat& src, cv::Mat& sx)
{
    Mat sx1;

    sx = cv::Mat(src.size(), CV_16SC3); //declaring the final output as signed value

    int h = src.rows;
    int w = src.cols;

    int ker1[3] = { 1,0,-1 };
    int ker2[3] = { 1,2,1 };


    cv::Mat temp = cv::Mat::zeros(src.size(), CV_16SC3);

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for (int i = 0; i < 3; i++)
            {
                int x_p = x + i - 2;
                if (x_p < 0 || x_p >= w)
                    continue;

                r += ker1[i] * src.at<cv::Vec3b>(y, x_p)[0];
                g += ker1[i] * src.at<cv::Vec3b>(y, x_p)[1];
                b += ker1[i] * src.at<cv::Vec3b>(y, x_p)[2];
            }

            temp.at<cv::Vec3s>(y, x)[0] = r / 4;
            temp.at<cv::Vec3s>(y, x)[1] = g / 4;
            temp.at<cv::Vec3s>(y, x)[2] = b / 4;
        }
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for (int i = 0; i < 3; i++)
            {
                int y_p = y + i - 2;
                if (y_p < 0 || y_p >= h)
                    continue;

                r += ker2[i] * temp.at<cv::Vec3s>(y_p, x)[0];
                g += ker2[i] * temp.at<cv::Vec3s>(y_p, x)[1];
                b += ker2[i] * temp.at<cv::Vec3s>(y_p, x)[2];
            }

            sx.at<cv::Vec3s>(y, x)[0] = r /4;
            sx.at<cv::Vec3s>(y, x)[1] = g /4;
            sx.at<cv::Vec3s>(y, x)[2] = b /4;
        }
    }
    return 0;
}


int sobelY3x3(cv::Mat& src, cv::Mat& sy)
{

    Mat sy1;

    sy = cv::Mat(src.size(), CV_16SC3);

    int h = src.rows;
    int w = src.cols;

   
    int ker1[3] = { 1,2,1 };
    int ker2[3] = { 1,0,-1 };

    cv::Mat temp = cv::Mat::zeros(src.size(), CV_16SC3);


    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for (int i = 0; i < 3; i++)
            {
                int x_p = x + i - 2;
                if (x_p < 0 || x_p >= w)
                    continue;

                r += ker1[i] * src.at<cv::Vec3b>(y, x_p)[0];
                g += ker1[i] * src.at<cv::Vec3b>(y, x_p)[1];
                b += ker1[i] * src.at<cv::Vec3b>(y, x_p)[2];
            }

            temp.at<cv::Vec3s>(y, x)[0] = r / 4;
            temp.at<cv::Vec3s>(y, x)[1] = g / 4;
            temp.at<cv::Vec3s>(y, x)[2] = b / 4;
        }
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for (int i = 0; i < 3; i++)
            {
                int y_p = y + i - 2;
                if (y_p < 0 || y_p >= h)
                    continue;

                r += ker2[i] * temp.at<cv::Vec3s>(y_p, x)[0];
                g += ker2[i] * temp.at<cv::Vec3s>(y_p, x)[1];
                b += ker2[i] * temp.at<cv::Vec3s>(y_p, x)[2];
            }

            sy.at<cv::Vec3s>(y, x)[0] = r / 4;
            sy.at<cv::Vec3s>(y, x)[1] = g / 4;
            sy.at<cv::Vec3s>(y, x)[2] = b / 4;
        }
    }

    return 0;
}

int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& grad_mag) {

    Mat grad_mag1;

    if (sx.empty() || sy.empty()) {
        return -1;
    }
    if (sx.rows != sy.rows || sx.cols != sy.cols) {
        return -1;
    }

    grad_mag = cv::Mat::zeros(sx.rows, sx.cols, CV_8UC3);

    for (int y = 0; y < sx.rows; y++) {
        for (int x = 0; x < sx.cols; x++) {
            // Extract the sx and sy values
            cv::Vec3s sx1 = sx.at<cv::Vec3s>(y, x);
            cv::Vec3s sy1 = sy.at<cv::Vec3s>(y, x);

            cv::Vec3b mag;
            for (int i = 0; i < 3; i++) {
                mag[i] = cv::saturate_cast<uchar>(sqrt(sx1[i] * sx1[i] + sy1[i] * sy1[i]));
            }
            grad_mag.at<cv::Vec3b>(y, x) = mag;
        }
    }

    return 0;
}



int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels)
{
    
    blur5x5(src, dst);  // calling the blur function

    int b = 255 / levels; //calculating bucket value


    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            cv::Vec3b& color = dst.at<cv::Vec3b>(i, j);

            for (int k = 0; k < 3; k++)
            {
                int xt = color[k] / b;
                int xf = xt * b;
                color[k] = xf;
            }
        }
    }

    return 0;
}

int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold)
{
    cv::Mat dst1;
    cv::Mat sx, sy;
    cv::Mat grad_mag;
    cv::Mat bq;
    sobelX3x3(src, sx);
    sobelY3x3(src, sy);
    magnitude(sx, sy, grad_mag);

    blurQuantize(src, dst, levels);

    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (grad_mag.at<Vec3b>(i, j)[k] > magThreshold)
                    dst.at<Vec3b>(i, j)[k] = 0;                     //setting black color to values above threshold
            }
        }
    }

    return 0;
}

int cvtnegative(Mat& src, Mat& dst)
{
    dst = Mat::zeros(src.size(), src.type());
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            Vec3b p = src.at<Vec3b>(i, j);
            p[0] = 255 - p[0];
            p[1] = 255 - p[1];
            p[2] = 255 - p[2];
            dst.at<Vec3b>(i, j) = p;
        }
    }

    return 0;
}

int sharp(Mat& src, Mat& dst)
{
    Mat k = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0); //3x3 mat to sharpen image
    filter2D(src, dst, -1, k);
    return 0;
}

int saturation(Mat& src, Mat& dst, int sv)
{
    Mat temp;
    int h, sat, v;
    Vec3b p;

    dst = Mat::zeros(src.size(), src.type());
    cvtColor(src, temp, COLOR_BGR2HSV);

    for (int y = 0; y < temp.rows; y++)
    {
        for (int x = 0; x < temp.cols; x++)
        {
            p = temp.at<Vec3b>(y, x);
            h = p.val[0];
            sat = p.val[1];
            v = p.val[2];
            sat = sat + sv;         //changing the saturation value
            if (sat > 255)
                sat = 255;
            p.val[1] = sat;
            temp.at<Vec3b>(y, x) = p;
        }
    }
    cvtColor(temp, dst, COLOR_HSV2BGR);
    return 0;
}