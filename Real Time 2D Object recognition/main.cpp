/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * This program file gets inputs from the user such as entering the label name of a object
 * When you want to enter the object name press the respective objects label number to store the objects name into the CSV file
 * When the user presses 'S' or 's' it saves the frame
 * It also asks for the image name to be saved
 * This program file also calulates the oriented bounding box and projects it on the image (i.e on the object)
 * When the user presses the 'Q' or 'q' the program quits
 * Press 'B' or 'b' for Thresholding
 * Press 'M' or 'm' for Morphological processing
 * Press 'C' or 'c' for applying color to the regions
 * Press 'N' or 'n' for training mode
 * Press the labels respective number to select that object and name it
 * Press 'W' or 'w' to save the csv file once you have labeled all the parts
 * Press 'I' or 'i' for inference mode
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../filters.h"
#include "../filters.cpp"
#include <QApplication>
#include <QtWidgets>
#include "../train/trainingData.h"
#include "../csv_util.h"
#include "../infer/Classifier.h"


// Declaring the variables for storing the data from connected component analysis opencv function
cv::Mat labels, stats, centroids;
// Declaring vector for all the labels
std::vector<int> topNLabels;
// Declaring vector to store the features
std::vector<feature> features_vector;

Classifier classifier = Classifier();

// Writing the features to the csv file named as features.csv
void dumpFeatures() {
    write_to_csv("features.csv", features_vector);
}

// Getting input from the user to store the objects name
void useLabel(int label) {
    // Select region that has the specified label
    cv::Mat region;
    if (label > labels.dims) {
        std::cerr << "No such label..." << std::endl;
        return;
    }
    region = (labels == label);
    // Sending the data to the traningData.cpp to be used in calculations like extracting features, calculating mu22alpha ..
    trainingData data = trainingData(region.clone());
    // Calculating features like Fill percentage , height to width ratio , Hu moments and mu22alpha
    data.extractFeatures();

    QString user_input = QInputDialog::getText(nullptr, "Enter Label", "Enter label for image:");
    data.setLabel(user_input.toStdString());
    // Storing the features in the features_vector
    features_vector.push_back(data.getFeatures());
}

// Reference
// https://stackoverflow.com/questions/40404031/drawing-bounding-box-for-a-rotated-object
// https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html
cv::RotatedRect computeBoundingBox(cv::Mat &region, double centroidX, double centroidY, double alpha) {
    // computing size of oriented bounding box using find contours and then finding the rotated rectangle
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(region, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::RotatedRect rect = cv::minAreaRect(contours[0]);
    return rect;
}


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    // Setting up the live video stream
    cv::VideoCapture *capdev;
    capdev = new cv::VideoCapture(0);
    // Setting the frame/window size
    capdev->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    capdev->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    // Setting the frame rate per second
    capdev->set(cv::CAP_PROP_FPS, 30);
    if (!capdev->isOpened())
    {
        std::cout << "Can't access the default camera" << std::endl;
        return -1;
    }
    // Defining global Matrix for live stream and final modified frames
    cv::Mat frame;
    cv::Mat filter_frame;
    cv::Mat src = frame;
    cv::Mat dst;
    *capdev  >> frame;
    int Mode = 0;
    // CHANGING THE EFFECTS BASED ON THE PRESENT FILTER
    while (true)
    {
        *capdev  >> frame;
        if( frame.empty() )
        {
            printf("frame is empty\n");
            break;
        }
        if (!frame.empty())
        {
            if (Mode == 0)
            {
                cv::Mat src = frame;
                cv::imshow("Live Video", src);
                filter_frame = src;
            }
            else if (Mode == 1)
            {
             cv::Mat src = frame;
             cv::Mat dst;
             // bluring the image before processing using 5*5 filter
             GaussianBlur(src, dst, cv::Size(5, 5), 0);
             cv::Mat dst1(src.rows, src.cols, CV_8UC1);
             // sending it to a thresholding function to convert it as a binary image
             thresholding(dst, dst1);
             cv::imshow("Live Video", dst1);
             filter_frame = dst1;
            }
            else if (Mode == 5)
            {
                cv::Mat src = frame;
                cv::Mat dst;
                // bluring the image before processing using 5*5 filter
                GaussianBlur(src, dst, cv::Size(5, 5), 0);
                cv::Mat dst1(src.rows, src.cols, CV_8UC1);
                // sending it to a thresholding function to convert it as a binary image
                thresholding1(dst, dst1);
                cv::imshow("Live Video", dst1);
                filter_frame = dst1;
            }
            else if (Mode == 2)
            {
                cv::Mat src = frame;
                cv::Mat dst;
                // bluring the image before processing using 5*5 filter
                GaussianBlur(src, dst, cv::Size(5, 5), 0);
                cv::Mat dst1(src.rows, src.cols, CV_8UC1);
                // sending it to a thresholding function to convert it as a binary image
                thresholding1(dst, dst1);
                cv::Mat morph = Morphological(dst1);
                cv::imshow("Live Video", dst1);
                filter_frame = dst1;
            }
            else if (Mode == 3)
            {
                cv::Mat src = frame;
                cv::Mat dst;
                // bluring the image before processing using 5*5 filter
                GaussianBlur(src, dst, cv::Size(5, 5), 0);
                cv::Mat dst1(src.rows, src.cols, CV_8UC1);
                // sending it to a thresholding function to convert it as a binary image
                thresholding1(dst, dst1);
                cv::Mat morph = Morphological(dst1);
                cv::Mat regionFrame = adding_color_to_the_foreground(morph, labels, stats, centroids, topNLabels);
                cv::imshow("Live Video", regionFrame);
                filter_frame = regionFrame;
            }
            else if (Mode == 4)
            {
             cv::Mat src = frame;
             cv::Mat dst;
             // bluring the image before processing using 5*5 filter
             GaussianBlur(src, dst, cv::Size(5, 5), 0);
             cv::Mat dst1(src.rows, src.cols, CV_8UC1);
             // sending it to a thresholding function to convert it as a binary image
             thresholding1(dst, dst1);
             cv::Mat morph = Morphological(dst1);
             cv::Mat regionFrame = adding_color_to_the_foreground(morph, labels, stats, centroids, topNLabels);

             // Draw oriented bounding boxes around the top N regions on the original frame
             for (int n = 0; n < topNLabels.size(); n++)
             {
                 int label = topNLabels[n];
                 cv::Mat region;
                 region = (labels == label);
                 // Reference
                 // https://docs.opencv.org/3.4/dd/d49/tutorial_py_contour_features.html
                 // calculate central moments, centroids, and alpha
                 Moments momentValue = moments(region, true);
                 double hu[7];
                 HuMoments(momentValue, hu);
                 // Calculating the centroidX value
                 double centroidX = int(momentValue.m10 / momentValue.m00);
                 // Calculating the centroidX value
                 double centroidY = int(momentValue.m01 / momentValue.m00);
                 // Calculating the alpha value ( i.e angle of least central moment )
                 double alpha = 0.5 * atan2(2 * momentValue.mu11, momentValue.mu20 - momentValue.mu02);
                 // calculate mu22alpha
                 double beta = alpha + 1.57;
                 double sum = 0.0;
                 // Calculating the mu22alpha based on the formulae given in the class
                 for (int y = 0; y < region.rows; y++) {
                     for (int x = 0; x < region.cols; x++) {
                         if (region.at<bool>(y, x)) {
                             double yDiff = y - centroidY;
                             double xDiff = x - centroidX;
                             double value = yDiff * cos(beta) + xDiff * sin(beta);
                             sum += value * value;
                         }
                     }
                 }
                 double mu22alpha = sum / momentValue.m00;
                 // Getting the Humoments value
                 // Reference
                 // https://cvexplained.wordpress.com/2020/07/21/10-4-hu-moments/
                 // https://learnopencv.com/shape-matching-using-hu-moments-c-python/
                 double hu7 = hu[6];
                 // draw oriented bounding box with alpha and mu22alpha information and setting line length
                 double length = 200.0;
                 // Calculating the edge points
                 double edge1 = length * sin(alpha);
                 double edge2 = sqrt(length * length - edge1 * edge1);
                 double xPrime = centroidX + edge2, yPrime = centroidY + edge1;
                 double x1Prime = centroidX - edge2, y1Prime = centroidY - edge1;
                 // Drawing the line on the live video on top of the region
                 // Reference
                 // https://www.geeksforgeeks.org/draw-a-line-using-opencv-in-c/
                 // https://docs.opencv.org/3.4/d6/d6e/group__imgproc__draw.html
                 line(src, Point(x1Prime, y1Prime), Point(xPrime, yPrime), cv::Scalar(0, 255, 255), 3);
                 // Getting the height to width ratio
                 trainingData data = trainingData(region.clone());
                 data.extractFeatures();
                 double hw = data.gethwratio();
                 // Displaying the label value on the foreground object
                 std::stringstream label_ss;
                 label_ss << "label=" << label;
                 cv::putText(src, label_ss.str(), Point(x1Prime, y1Prime - 10), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                             cv::Scalar(0, 255, 255), 2);
                 // Displaying the alpha value on the foreground object
                 std::stringstream alpha_ss;
                 alpha_ss << "alpha=" << alpha;
                 cv::putText(src, alpha_ss.str(), Point(x1Prime, y1Prime - 30), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                             cv::Scalar(0, 255, 255), 2);
                 // Displaying the hu7 value on the foreground object
                 std::stringstream mu22alpha_ss;
                 mu22alpha_ss << "HEIGHT2WIDTH_ratio=" << hw;
                 cv::putText(src, mu22alpha_ss.str(), Point(x1Prime, y1Prime - 50), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                             cv::Scalar(0, 255, 255), 2);
                 // display the oriented bounding box on the original frame using computeBoundingBox function
                 // Reference
                 // https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html
                 cv::RotatedRect rect = computeBoundingBox(region, centroidX, centroidY, alpha);
                 cv::Point2f vertices[4];
                 rect.points(vertices);
                 for (int i = 0; i < 4; i++) {
                     cv::line(src, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 255), 3);
                 }
             }

             cv::imshow("Live Video", src);
             filter_frame = src;
            }
            else if (Mode == 6)
            {
                cv::Mat src = frame;
                cv::Mat dst;
                // bluring the image before processing using 5*5 filter
                GaussianBlur(src, dst, cv::Size(5, 5), 0);
                cv::Mat dst1(src.rows, src.cols, CV_8UC1);
                // sending it to a thresholding function to convert it as a binary image
                thresholding(dst, dst1);
                cv::Mat morph = Morphological(dst1);
                cv::Mat regionFrame = adding_color_to_the_foreground(morph, labels, stats, centroids, topNLabels);

                // Draw oriented bounding boxes around the top N regions on the original frame
                for (int n = 0; n < topNLabels.size(); n++)
                {
                    int label = topNLabels[n];
                    cv::Mat region;
                    region = (labels == label);
                    // Reference
                    // https://docs.opencv.org/3.4/dd/d49/tutorial_py_contour_features.html
                    // calculate central moments, centroids, and alpha
                    Moments momentValue = moments(region, true);
                    double hu[7];
                    HuMoments(momentValue, hu);
                    // Calculating the centroidX value
                    double centroidX = int(momentValue.m10 / momentValue.m00);
                    // Calculating the centroidX value
                    double centroidY = int(momentValue.m01 / momentValue.m00);
                    // Calculating the alpha value ( i.e angle of least central moment )
                    double alpha = 0.5 * atan2(2 * momentValue.mu11, momentValue.mu20 - momentValue.mu02);
                    // calculate mu22alpha
                    double beta = alpha + 1.57;
                    double sum = 0.0;
                    // Calculating the mu22alpha based on the formulae given in the class
                    for (int y = 0; y < region.rows; y++) {
                        for (int x = 0; x < region.cols; x++) {
                            if (region.at<bool>(y, x)) {
                                double yDiff = y - centroidY;
                                double xDiff = x - centroidX;
                                double value = yDiff * cos(beta) + xDiff * sin(beta);
                                sum += value * value;
                            }
                        }
                    }
                    double mu22alpha = sum / momentValue.m00;
                    // Getting the Humoments value
                    // Reference
                    // https://cvexplained.wordpress.com/2020/07/21/10-4-hu-moments/
                    // https://learnopencv.com/shape-matching-using-hu-moments-c-python/
                    double hu7 = hu[6];
                    // draw oriented bounding box with alpha and mu22alpha information and setting line length
                    double length = 200.0;
                    // Calculating the edge points
                    double edge1 = length * sin(alpha);
                    double edge2 = sqrt(length * length - edge1 * edge1);
                    double xPrime = centroidX + edge2, yPrime = centroidY + edge1;
                    double x1Prime = centroidX - edge2, y1Prime = centroidY - edge1;
                    // Drawing the line on the live video on top of the region
                    // Reference
                    // https://www.geeksforgeeks.org/draw-a-line-using-opencv-in-c/
                    // https://docs.opencv.org/3.4/d6/d6e/group__imgproc__draw.html
                    line(src, Point(x1Prime, y1Prime), Point(xPrime, yPrime), cv::Scalar(0, 255, 255), 3);
                    // Getting the height to width ratio
                    trainingData data = trainingData(region.clone());
                    data.extractFeatures();

                    std::stringstream label_ss;
                    label_ss << "label=" << classifier.classify(data.getFeatures());
                    cv::putText(src, label_ss.str(), Point(x1Prime, y1Prime - 10), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                                cv::Scalar(0, 255, 255), 2);


                    // display the oriented bounding box on the original frame using computeBoundingBox function
                    // Reference
                    // https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html
                    cv::RotatedRect rect = computeBoundingBox(region, centroidX, centroidY, alpha);
                    cv::Point2f vertices[4];
                    rect.points(vertices);
                    for (int i = 0; i < 4; i++) {
                        cv::line(src, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 255), 3);
                    }
                }

                cv::imshow("Live Video", src);
                filter_frame = src;
            }
        }
        // Checking for the key press
        char key = cv::waitKey(10);
        if (key == 'q' || key=='Q')
        {
            break;
        }
        if (key == 'W' || key=='w')
        {
            dumpFeatures();
            break;
        }
        // Saving the frame
        if (key == 's' || key == 'S')
        {
            std::string filename;
            QString user_input = QInputDialog::getText(nullptr, "Saving the image", "Enter the name of the image you want to save:");
            if (imwrite(user_input.toStdString() +".jpg", filter_frame))
            {
                std::cout << "Image saved!" << std::endl;
            }
            else
            {
                std::cout << "Failed to save image" << std::endl;
            }
        }
        // Press 'B' or 'b' for Thresholding
        if (key == 'B' || key == 'b')
        {
            Mode = 1;
        }
        // Press 'M' or 'm' for Morphological processing
        if (key == 'M' || key == 'm')
        {
            Mode = 2;
        }
        // Press 'C' or 'c' for applying color to the regions
        if (key == 'C' || key == 'c')
        {
            Mode = 3;
        }
        // Press 'N' or 'n' for training mode
        if (key == 'N' || key == 'n')
        {
            Mode = 4;
        }
        // Press 'I' or 'i' for inference mode
        if (key == 'I' || key == 'i')
        {
            Mode = 6;
        }
        // Press 'O' or 'o' for training mode
        if (key == 'O' || key == 'o')
        {
            Mode = 5;
        }
        // if a number is pressed, call a function with that number as the argument
        if (key >= '0' && key <= '9')
        {
            int number = key - '0';
            if (number == 0)
            {
                number = 10;
            }
            std::cout << "You pressed " << number << std::endl;
            useLabel(number);
        }
    }
    return 0;
}


