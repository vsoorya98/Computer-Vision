
#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include<cstring>
#include<cstdio>
#include<math.h>
#include<cstdlib>
#include "filters.h"

using namespace cv;
using namespace std;


int main()
{
    VideoCapture cap(0); // opens camera 
    if (!cap.isOpened())  
        return -1;
    int k = 1;
    namedWindow("Window", 1); 
    Mat frame;
    Mat gray_frame;
    Mat g_frame1;
    Mat blurred;
    Mat sx;
    Mat sy;
    Mat mag;
    Mat bquant;
    Mat cartoons;
    Mat sx1;
    Mat sy1;
    Mat mag1;
    Mat negative, sat, sharp_img;
    int l,th,sv;
    int f = 0, z = 0,d=0;
    char key = 0;
    for (;;)
    {
        cap >> frame; //captures each frame
        
        if (key == 'g')
            k = 2;
        if (key == 'h')
            k = 3;
        if (key == 'b')
            k = 4;
        if (key == 'x')
            k = 5;
        if (key == 'y')
            k = 6;
        if (key == 'm')
            k = 7;
        if (key == 'i')
            k = 8;
        if (key == 'c')
            k = 9;
        if (key == 'n')
            k = 10;
        if (key == 't')
            k = 11;
        if (key == 'v')
            k = 12;
            
        if (k == 2)
        {
            cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
            imshow("Window", gray_frame);
            imwrite("o1.jpg", frame);
            imwrite("m1.jpg", gray_frame);

        }
        else if (k == 3)
        {
            alt_grey(frame,g_frame1);
            imshow("Window", g_frame1);
            imwrite("o2.jpg", frame);
            imwrite("m2.jpg", g_frame1);
        }
        else if (k == 4)
        {
            blur5x5(frame, blurred);
            imshow("Window", blurred);
            imwrite("o3.jpg", frame);
            imwrite("m3.jpg", blurred);
        }
        else if (k == 5)
        {
            sobelX3x3(frame,sx);
            convertScaleAbs(sx, sx1, 4);
            imshow("Window", sx1);
            imwrite("o4.jpg", frame);
            imwrite("m4.jpg", sx1);
        }
        else if (k == 6)
        {
            sobelY3x3(frame, sy);
            convertScaleAbs(sy, sy1, 4);
            imshow("Window", sy1);
            imwrite("o5.jpg", frame);
            imwrite("m5.jpg", sy1);
        }
        else if (k == 7)
        {
            sobelX3x3(frame, sx);
            sobelY3x3(frame, sy);
            magnitude(sx,sy,mag);
            convertScaleAbs(mag, mag1, 4);
            imshow("Window", mag1);
            imwrite("o6.jpg", frame);
            imwrite("m6.jpg", mag1);
        }
        else if (k == 8)
        {
            if (d == 0) {
                system("cls");
                cout << "Enter level value to which color channel should be quantized into:";
                cin >> l;
                d = 1;
            }
            blurQuantize(frame,bquant,l);
            imshow("Window", bquant);
            imwrite("o7.jpg", frame);
            imwrite("m7.jpg", bquant);
        }
        else if (k == 9)
        {
            if( f == 0) {
                system("cls");
                cout << "Enter level value to which color channel should be quantized into:";
                cin >> l;
                cout << "Enter magnitude threshold value:";
                cin >> th;
                f = 1;
            }

            cartoon(frame,cartoons,l,th);
            imshow("Window", cartoons);
            imwrite("o8.jpg", frame);
            imwrite("m8.jpg", cartoons);
        }
       
        else if (k == 10)
        {
            cvtnegative(frame,negative);
            imshow("Window", negative);
            imwrite("o9.jpg", frame);
            imwrite("m9.jpg", negative);
        }
        else if (k == 11)
        {
            if (z == 0)
            {
                system("cls");
                cout << "Enter value to change saturation: ";
                cin >> sv;
                z = 1;
            }
            saturation(frame,sat, sv);
            imshow("Window", sat);
            imwrite("o10.jpg", frame);
            imwrite("m10.jpg", sat);
        }
        else if (k == 12)
        {
            sharp(frame, sharp_img);
            imshow("Window", sharp_img);
            imwrite("o11.jpg", frame);
            imwrite("m11.jpg", sharp_img);
        }
        else
            imshow("Window", frame);

        key = waitKey(30);

        if (key == 'q')
            break;

        else if (key == 's')
            imwrite("saved_frame.jpg", frame);
    }


    return 0;
}