/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * Created on 03/22/2023
 * In this program file we have implemented the camera calibration process using opencv findchessboardcorner function and cornersubpix functions
 * once we have found the corners we are performing calibration process using opencv calibratecamera module, then we calculate the current position of the camera using solvePNP function
 * then we display a object on the checkerboard with the help of projectPoints function
 * The keywords for implementing the above tasks:
 * Press "s" for the saving the vector of corners found in that particular live frame (need atleast five corner data to perform the calibration process)
 * Press "c" for the camera calibration process (need atleast five corner data to perform the calibration process)
 * Press "d" to display the rotation and translation data obtained using solvePnP function
 * Press "e" to display the outside 4 corner points on the checker board
 * Press "v" to display the virtual object on the checker board
 * Press "m" to detect multiple checker board and display the objects on them
 */


#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

// this part of code is for extension (displaying .obj files) but we are still having some issues with this code
std::pair<std::vector<cv::Point3f>, std::vector<std::vector<unsigned int>>> read_the_points_and_face_values(std::string objfile)
{
    std::ifstream  obj(objfile);
    std::vector<std::string> v_values;
    std::vector<std::string> face_values;
    std::string line;
    std::vector<cv::Point3f> vertices;
    std::vector<unsigned int> faceInd;
    std::vector<std::vector<unsigned int>> faceVertices;

    faceVertices.clear();
    vertices.clear();
    v_values.clear();
    faceInd.clear();

    if (!obj.is_open())
    {
        std::cout << "Fail to open: " << objfile << std::endl;
    }

    while (std::getline(obj, line))
    {
        //Reference
        //Geeksforgeeks
        //https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/
        // initializing variables
        int start, end;
        start = end = 0;
        // defining the delimitation character
        char dl = ' ';
        while ((start = line.find_first_not_of(dl, end))
               != std::string::npos)
        {
            // str.find(dl, start) will return the index of dl
            // from start index
            end = line.find(dl, start);
            // substr function return the substring of the
            // original string from the given starting index
            // to the given end index
            v_values.push_back(line.substr(start, end - start));
        }


        //std::cout << "culprit number 2" << std::endl;
        if (v_values.size() > 0)
        {
            std::cout << "culprit" << std::endl;
            if(v_values[0].compare("v") == 0)
            {

                vertices.push_back(cv::Point3f(std::stof(v_values[1]),std::stof(v_values[3]),(-1 * std::stof(v_values[2]))));
                for (int i = 0; i < vertices.size(); i++) {
                    std::cout << vertices[i] << std::endl;
                }
            }

            else if (v_values[0].compare("f") == 0)
            {
                for(int i = 1 ; i<v_values.size() ; i++)
                {
                    faceInd.push_back(std::stoi(face_values[i]));
                }
                faceVertices.push_back(faceInd);
                faceInd.clear();
            }


        }

    }
    return std::make_pair(vertices, faceVertices);

}


//Creating a cube
std::vector<cv::Point3f> three_dimenesional_cube(float cubeSize)
{
    std::vector<cv::Point3f> cube_points;
    cube_points.push_back(cv::Point3f(0, 0, 0));
    cube_points.push_back(cv::Point3f(cubeSize, 0, 0));
    cube_points.push_back(cv::Point3f(cubeSize, cubeSize, 0));
    cube_points.push_back(cv::Point3f(0, cubeSize, 0));
    cube_points.push_back(cv::Point3f(0, 0, -cubeSize));
    cube_points.push_back(cv::Point3f(cubeSize, 0, -cubeSize));
    cube_points.push_back(cv::Point3f(cubeSize, cubeSize, -cubeSize));
    cube_points.push_back(cv::Point3f(0, cubeSize, -cubeSize));

    return cube_points;
}

//Creating a tapered cube
//std::vector<cv::Point3f> three_dimenesional_cube(float cubeSize)
//{
//    std::vector<cv::Point3f> cube_points;
//    float taperHeight = 2.0f;
//    cube_points.push_back(cv::Point3f(0, 0, 0));
//    cube_points.push_back(cv::Point3f(cubeSize, 0, 0));
//    cube_points.push_back(cv::Point3f(cubeSize, cubeSize, 0));
//    cube_points.push_back(cv::Point3f(0, cubeSize, 0));
//    cube_points.push_back(cv::Point3f(0, 0, -cubeSize ));
//    cube_points.push_back(cv::Point3f(cubeSize, 0, -cubeSize ));
//    cube_points.push_back(cv::Point3f(cubeSize, cubeSize, -cubeSize * taperHeight));
//    cube_points.push_back(cv::Point3f(0, cubeSize, -cubeSize * taperHeight));
//
//    return cube_points;
//}



int main()
{
    //Setting the camera to default
    cv::VideoCapture cap(0);
    //Setting the frame height and width
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    //Setting the frame rate per second
    cap.set(cv::CAP_PROP_FPS, 30);
    //Setting target/checker board as 6 rows and 9 columns
    int row = 6;
    int column = 9;
    int length = 2;
    int breath = 2;
    int depth = 2;
    cv::Size cube_size(column, row);

    //Declaring the size of the square in mm
    double square_dimension  = 0.023;
    //Declaring the variable for selecting the loop
    int loop_selection ;
    //Defining the world coordinates of the chessboard corners
    std::vector<cv::Vec3f> point_set;
    for (int i = 0; i < cube_size.height; ++i) {
        for (int j = 0; j < cube_size.width; ++j)
        {
            //Specifying that each cube is 23*23*0mm in dimension, for all the 54 cubes
            point_set.push_back(cv::Vec3f(j * square_dimension, i * square_dimension, 0));
        }
    }
    //Declaring the variable to store the cube dimensions for all the 54 cubes in the checker board
    std::vector<std::vector<cv::Vec3f>> point_list;
    //Declaring the variable for the storing the corner_list
    std::vector<std::vector<cv::Point2f>> corner_list;
    //Declaring the variable to store the corner values
    std::vector<cv::Point2f> corner_set;

    //Vectors and matrix declared for calibration process
    //Declaring the cameraMatrix which is a input output array
    cv::Mat camera_matrix = cv::Mat::zeros(3, 3, CV_64F);
    //Declaring the distortion coefficients which is a input output array
    cv::Mat distortion_matrix = cv::Mat::zeros(5,1,CV_64F);
    //Declaring the Rotation matrix in a vector format for all the image values
    std::vector<cv::Mat> rotation_matrix ;
    //Declaring the Translation matrix in a vector format for all the image values
    std::vector<cv::Mat> translation_matrix ;

    //Vectors and matrix declared for displaying the Rotation and translation values
    //Declaring the live Rotation matrix in a vector format for all the image values
    std::vector<cv::Mat> live_rotation_matrix ;
    //Declaring the live Translation matrix in a vector format for all the image values
    std::vector<cv::Mat> live_translation_matrix ;

    while (true)
    {
        char key = cv::waitKey(10);
        //If any key is pressed
        if (key != -1)
        {
            //Change the loop_selection variable to the pressed key, so that we can keep the code running in the same loop
            loop_selection = key;
        }
        //Declaring the mat for each frame from the video stream
        cv::Mat frame;
        //Setting the live frame as the new frame
        cap.read(frame);
        //Converting from rgb to grayscale image
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        //Reference
        //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga93efa9b0aa890de240ca32b11253dd4a
        //Input parameters (image, size, output array (corner points))
        bool corner_detected = cv::findChessboardCorners(gray, cube_size, corner_set);
        if (loop_selection == 'a')
        {
            if (corner_detected)
            {
                //Reference
                //https://docs.opencv.org/4.x/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
                //https://anothertechs.com/programming/cpp/opencv/corernsubpixel/
                //Input parameters (image, corner_set, window size, zero zone, criteria)
                cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1),
                                 cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022
                //Input parameters (image, patternsize, corner_set, pattern found)
                cv::drawChessboardCorners(frame, cube_size, corner_set, corner_detected);
                //std::cout << "Number of corners: " << corner_set.size() << std::endl;
                //std::cout << "First corner coordinates: (" << corner_set[0].x << ", " << corner_set[0].y << ")" << std::endl;
            }
        }
        cv::imshow("Frame", frame);
        if (key == 'q')
            break;

        //Task 2
        else if (key == 's')
        {
            if (corner_detected)
            {
                //Storing the world cordinate in the point_list vector
                point_list.push_back(point_set);

                //Storing the corner set values in corner_list variable
                corner_list.push_back(corner_set);
                //std::cout<< "corner_list" << corner_list[0] << std::endl;
                std::cout<<"Corners detected & saved"<<std::endl;
            }
            else
            {
                std::cout<<"Corners not detected"<<std::endl;
            }
        }
        //Camera Calibration process
        else if(key == 'c')
        {
            //Checking if the collected image data is more than 5
            if (point_list.size() < 5 )
            {
                std::cout<<"Not enough calibration data is aquired, add more data by saving more images" << std::endl;
            }
            //Reference
            //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga3207604e4b1a1758aa66acb6ed5aa65d
            else
            {
                std::cout << "Camera Calibration Results" << std::endl;
                //Calculating the reprojection error along with camera matrix, distortion matrix, rotation and translation matrix
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga3207604e4b1a1758aa66acb6ed5aa65d
                double calibrationError = cv::calibrateCamera(point_list, corner_list, cv::Size(frame.rows, frame.cols), camera_matrix, distortion_matrix, rotation_matrix, translation_matrix);
                std::cout << "Camera matrix: " << std::endl << camera_matrix << std::endl;
                std::cout << "RMS Reprojection error: " << calibrationError << std::endl;
                //Saving the intrinsic parameters to a yml file
                cv::FileStorage fs("calibration.yaml", cv::FileStorage::WRITE);
                fs << "cameraMatrix" << camera_matrix;
                fs << "distortion_coefficients" << distortion_matrix;
                fs.release();
            }
        }
        //Task 4
        //Calculating the Current Position of the Camera
        else if (loop_selection == 'd')
        {
            // Read in camera calibration parameters from file
            cv::Mat cm, dc;
            cv::FileStorage fs("calibration.yaml", cv::FileStorage::READ);
            fs["cameraMatrix"] >> cm;
            fs["distortion_coefficients"] >> dc;
            fs.release();
            //Execute if the corners are detected
            if (corner_detected)
            {
                //Reference
                //https://docs.opencv.org/4.x/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
                //https://anothertechs.com/programming/cpp/opencv/corernsubpixel/
                //Input parameters (image, corner_set, window size, zero zone, criteria)
                cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1),
                                 cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022
                //Input parameters (image, patternsize, corner_set, pattern found)
                cv::drawChessboardCorners(frame, cube_size, corner_set, corner_detected);
                cv::Mat rotation_values, translation_values;
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d
                //Debugging reference
                //https://answers.opencv.org/question/3023/solvepnp-assertion-failed/
                cv::solvePnP(point_set, corner_set, cm, dc, rotation_values, translation_values);
                //Printing the translation and rotation values
                std::cout << "Translation data:" << std::endl << translation_values << std::endl;
                std::cout << "Rotation data:" << std::endl << rotation_values << std::endl;
                cv::imshow("Frame", frame);
            }

            else
            {
                std::cout << "No corners detected" << std::endl;
                cv::imshow("Frame", frame);
            }
        }
        //Task 5
        //Project Outside Corners or 3D Axes
        else if (loop_selection == 'e')
        {
            // Read in camera calibration parameters from file
            cv::Mat cm, dc;
            cv::FileStorage fs("calibration.yaml", cv::FileStorage::READ);
            fs["cameraMatrix"] >> cm;
            fs["distortion_coefficients"] >> dc;
            fs.release();
            //Execute if the corners are detected
            if (corner_detected)
            {
                std::vector<cv::Point2f>  img_point;
                //Reference
                //https://docs.opencv.org/4.x/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
                //https://anothertechs.com/programming/cpp/opencv/corernsubpixel/
                //Input parameters (image, corner_set, window size, zero zone, criteria)
                cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1),
                                 cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022
                //Input parameters (image, patternsize, corner_set, pattern found)
                cv::drawChessboardCorners(frame, cube_size, corner_set, corner_detected);
                cv::Mat rotation_values, translation_values;
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d
                //Debugging reference
                //https://answers.opencv.org/question/3023/solvepnp-assertion-failed/
                cv::solvePnP(point_set, corner_set, cm, dc, rotation_values, translation_values);
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga1019495a2c8d1743ed5cc23fa0daff8c
                cv::projectPoints(point_set,rotation_values, translation_values, cm, dc, img_point);
                {
                    //Defining at which points to project display the points in the checker board
                    int index[] = {0, 8, 45, 53};
                    for (int i : index)
                    {
                        //Reference
                        //https://docs.opencv.org/4.x/d6/d6e/group__imgproc__draw.html#ga07d2f74cadcf8e305e810ce8eed13bc9
                        cv::rectangle(frame, cv::Rect(img_point[i].x-2 , img_point[i].y-2 ,8,8), cv::Scalar(0, 255, 0),4);
                    }
                }
                cv::imshow("Frame", frame);

            }

            else
            {
                std::cout << "No corners detected" << std::endl;
                cv::imshow("Frame", frame);
            }
        }
        //Task 6
        // Create a Virtual Object
        else if (loop_selection == 'v')
        {
            // Read in camera calibration parameters from file
            cv::Mat cm, dc;
            cv::FileStorage fs("calibration.yaml", cv::FileStorage::READ);
            fs["cameraMatrix"] >> cm;
            fs["distortion_coefficients"] >> dc;
            fs.release();
            //Execute if the corners are detected
            if (corner_detected)
            {
                std::vector<cv::Point2f>  img_point;
                //Reference
                //https://docs.opencv.org/4.x/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
                //https://anothertechs.com/programming/cpp/opencv/corernsubpixel/
                //Input parameters (image, corner_set, window size, zero zone, criteria)
                cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1),
                                 cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022
                //Input parameters (image, patternsize, corner_set, pattern found)
                cv::drawChessboardCorners(frame, cube_size, corner_set, corner_detected);
                cv::Mat rotation_values, translation_values;
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d
                //Debugging reference
                //https://answers.opencv.org/question/3023/solvepnp-assertion-failed/
                cv::solvePnP(point_set, corner_set, cm, dc, rotation_values, translation_values);
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga1019495a2c8d1743ed5cc23fa0daff8c
                cv::projectPoints(point_set,rotation_values, translation_values, cm, dc, img_point);
                {
                    //Defining at which points to project display the points in the checker board
//                    for (int i = 0; i < img_point.size(); i++)
//                    {
//                        //Reference
//                        //https://docs.opencv.org/4.x/d6/d6e/group__imgproc__draw.html#ga07d2f74cadcf8e305e810ce8eed13bc9
//                        cv::rectangle(frame, cv::Rect(img_point[i].x-2 , img_point[i].y-2 ,8,8), cv::Scalar(0, 255, 0),4);
//                    }
                }
                // Create a 3D virtual object (cube) with size that fits on the checkerboard
                float cubeSize = (float)0.2 / 2; // Modify the size according to the checkerboard square size
                std::vector<cv::Point3f> virtualObjectPoints = three_dimenesional_cube(cubeSize);

                // Project the virtual object points to the image
                std::vector<cv::Point2f> virtualObjectImagePoints;
                cv::projectPoints(virtualObjectPoints, rotation_values, translation_values, cm, dc, virtualObjectImagePoints);

                // Draw lines connecting the projected virtual object points
                std::vector<std::pair<int, int>> lineIndices = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
                for (const auto &lineIndex : lineIndices)
                {
                    cv::line(frame, virtualObjectImagePoints[lineIndex.first], virtualObjectImagePoints[lineIndex.second], cv::Scalar(0, 255, 0), 2);
                }
                cv::imshow("Frame", frame);

            }

            else
            {
                std::cout << "No corners detected" << std::endl;
                cv::imshow("Frame", frame);
            }
        }
        //Multiple checker board detection
        else if (loop_selection == 'm')
        {
            // Read in camera calibration parameters from file
            cv::Mat cm, dc;
            cv::FileStorage fs("calibration.yaml", cv::FileStorage::READ);
            fs["cameraMatrix"] >> cm;
            fs["distortion_coefficients"] >> dc;
            fs.release();

            //Function to draw a virtual object on a checkerboard
            //Used the same concept as above
            auto drawVirtualObject = [&](const std::vector<cv::Point2f> &corner_set, const cv::Size &pattern_size) {
                std::vector<cv::Point2f> img_point;
                cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
                cv::drawChessboardCorners(frame, pattern_size, corner_set, true);

                cv::Mat rotation_values, translation_values;
                cv::solvePnP(point_set, corner_set, cm, dc, rotation_values, translation_values);
                cv::projectPoints(point_set, rotation_values, translation_values, cm, dc, img_point);

                float cubeSize = (float)0.2 / 2;
                std::vector<cv::Point3f> virtualObjectPoints = three_dimenesional_cube(cubeSize);
                std::vector<cv::Point2f> virtualObjectImagePoints;
                cv::projectPoints(virtualObjectPoints, rotation_values, translation_values, cm, dc, virtualObjectImagePoints);

                std::vector<std::pair<int, int>> lineIndices = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
                for (const auto &lineIndex : lineIndices)
                {
                    cv::line(frame, virtualObjectImagePoints[lineIndex.first], virtualObjectImagePoints[lineIndex.second], cv::Scalar(0, 255, 0), 2);
                }
            };
            //We split the image into two region of interest and then we treat each ROI as a image and perform the same process
            std::vector<cv::Rect> rois ={
                    cv::Rect(0, 0, gray.cols / 2, gray.rows),
                    cv::Rect(gray.cols / 2, 0, gray.cols / 2, gray.rows)
            };

            for (const auto &roi : rois)
            {
                //Detecting the checkerboard for each ROI
                cv::Mat roi_gray = gray(roi);
                std::vector<cv::Point2f> corner_set;
                bool found = cv::findChessboardCorners(roi_gray, cube_size, corner_set, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

                if (found)
                {
                    //if the checker board is found then it loops over that roi
                    for (auto &corner : corner_set)
                    {
                        corner.x += roi.x;
                        corner.y += roi.y;
                    }
                    //Draws the object for each roi
                    drawVirtualObject(corner_set, cube_size);
                }
            }
            cv::imshow("Frame", frame);
        }

        // Display a .obj Virtual Object
        else if (loop_selection == 'o')
        {
            // Read in camera calibration parameters from file
            cv::Mat cm, dc;
            cv::FileStorage fs("calibration.yaml", cv::FileStorage::READ);
            fs["cameraMatrix"] >> cm;
            fs["distortion_coefficients"] >> dc;
            fs.release();
            //Execute if the corners are detected
            if (corner_detected)
            {
                std::vector<cv::Point2f>  img_point;
                //Reference
                //https://docs.opencv.org/4.x/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
                //https://anothertechs.com/programming/cpp/opencv/corernsubpixel/
                //Input parameters (image, corner_set, window size, zero zone, criteria)
                cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1),
                                 cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022
                //Input parameters (image, patternsize, corner_set, pattern found)
                cv::drawChessboardCorners(frame, cube_size, corner_set, corner_detected);
                cv::Mat rotation_values, translation_values;
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d
                //Debugging reference
                //https://answers.opencv.org/question/3023/solvepnp-assertion-failed/
                cv::solvePnP(point_set, corner_set, cm, dc, rotation_values, translation_values);
                //Reference
                //https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga1019495a2c8d1743ed5cc23fa0daff8c
                cv::projectPoints(point_set,rotation_values, translation_values, cm, dc, img_point);
                {
                    //Defining at which points to project display the points in the checker board
//                    for (int i = 0; i < img_point.size(); i++)
//                    {
//                        //Reference
//                        //https://docs.opencv.org/4.x/d6/d6e/group__imgproc__draw.html#ga07d2f74cadcf8e305e810ce8eed13bc9
//                        cv::rectangle(frame, cv::Rect(img_point[i].x-2 , img_point[i].y-2 ,8,8), cv::Scalar(0, 255, 0),4);
//                    }
                }
                // Create a 3D virtual object (cube) with size that fits on the checkerboard
                float cubeSize = (float)0.2 / 2; // Modify the size according to the checkerboard square size

                std::string objfile = "/Users/shirish/CLionProjects/project4/object.obj";
                std::pair<std::vector<cv::Point3f>, std::vector<std::vector<unsigned int>>> objPointsAndFaceValues = read_the_points_and_face_values(objfile);

                std::vector<cv::Point3f> vertices = objPointsAndFaceValues.first;
                std::vector<std::vector<unsigned int>> faceVertices = objPointsAndFaceValues.second;

                // Project the virtual object points to the image
                std::vector<cv::Point2f> virtualObjectImagePoints;

                cv::projectPoints(vertices, rotation_values, translation_values, cm, dc, virtualObjectImagePoints);
                std::cout << "culprit number 2" << std::endl;
                // Draw lines connecting the projected virtual object points

                for(unsigned int i = 0; i<faceVertices.size(); i++)
                {
                    for(unsigned int j = 0; j<faceVertices[i].size() - 1; j++)
                    {
                        cv::line(frame,virtualObjectImagePoints[faceVertices[i][j]-1],virtualObjectImagePoints[faceVertices[i][j+1]-1],cv::Scalar(255,255,0), 1);
                    }
                }
                cv::imshow("Frame", frame);

            }
            else
            {
                std::cout << "No corners detected" << std::endl;
                cv::imshow("Frame", frame);
            }
        }
    }
    return 0;
}




