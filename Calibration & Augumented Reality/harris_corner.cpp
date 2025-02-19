/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * This program uses opencv harris corner detector and displays the features in the live stream
 */
#include <opencv2/opencv.hpp>

int main() {
    //setting the camera to default
    cv::VideoCapture *cap;
    //Setting the frame height and width
    cap->set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    //Setting the frame rate per second
    cap->set(cv::CAP_PROP_FPS, 30);
    cap = new cv::VideoCapture(0);
    if (!cap->isOpened()) {
        std::cout << "could not access the camera" << std::endl;
        return -1;
    }
    //Creating a mat to store the live camera frames
    cv::Mat frame;
    while (true)
    {

        //Checking for input from the user
        char key = cv::waitKey(10);
        //Getting the frame from the live feed camera
        *cap >> frame;
        if (frame.empty()) {
            std::cout << "frame is empty" <<std::endl;
            break;
        }
        //Creating a mat for storing the greyscale image
        cv::Mat grey;
        //Converting to greyscale image
        cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
        //Reference
        //Opencv Harris detector function
        //https://docs.opencv.org/3.4/d4/d7d/tutorial_harris_detector.html
        cv::Mat output = cv::Mat::zeros(grey.size(), CV_32FC1);
        cv::cornerHarris(grey, output, 2, 3, 0.04);

        /*Calculating the threshold value based on the result we have obtained from the cornerharris detector
          if we use the standard the 200 value it did not detect any corners so used a similar method described in this
          tutorial to get the thresholding value: https://www.projectpro.io/recipes/detect-corners-harris-corner-method-opencv
          in this method they just pick the top 10 percent maximum value and display it as shown in the below code, but we have decided to use
          mean and standard deviation to get the thresholded values since it yields much better results*/
//        cv::Mat sorted;
//        cv::sort(output.reshape(0,1), sorted, cv::SORT_DESCENDING);
//        float thresh = sorted.at<float>(static_cast<int>(std::floor(0.0001 * sorted.total())));
        //Reference
        //Opencv meanstandarddeviation
        //https://docs.opencv.org/3.4/d2/de8/group__core__array.html#gab473bf2eb6d14ff97e89b355dac20707
        /*Using this function we are calculating the mean and standard deviation of the matrix and using the
        value and we have set the threshold as mean+(2.5)standard deviation*/
        cv::Scalar mean, stddev;
        cv::meanStdDev(output, mean, stddev);
        float thresh = static_cast<float>(mean.val[0] + 2.5 * stddev.val[0]);


        //std::cout<< thresh <<std::endl;

        //displaying the harris corners
        for (int i = 0; i < output.rows ; i++) {
            for(int j = 0; j < output.cols; j++) {
                if (output.at<float>(i,j) > thresh) {
                    circle(frame, cv::Point(j,i), 1, cv::Scalar(255, 0, 255), 3);
                }
            }
        }

        imshow("Video", frame);
        //Press "q" to end the program
        if (key == 'q')
        {
            break;
        }

    }
    cv::destroyAllWindows();

    return 0;
}
