#include <opencv2/opencv.hpp>
#include <cstdio>
#include <cstring>

int main(int argc, char** argv)

{
    cv::Mat image1;
    image1 = cv::imread("image.jpg");

    if (image1.data == NULL) { // check if imread was successful
        printf("Unable to read image\n");
        exit(-2);
    }

    cv::namedWindow("Car", cv::WINDOW_AUTOSIZE);
    cv::imshow("Car", image1);

    char key;

    do
    {
        key = cv::waitKey(0);
        if (key == 'q') // Quit on 'q'
            break;

    } while (true);

    return 0;

}