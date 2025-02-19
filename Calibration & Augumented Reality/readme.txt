The operating I used is macos ventura and I used Clion as my IDE

Members worked in this project :

Shirish Kishore Kumar
Soorya Vijayaragavan
Created on 03/22/2023

Instructions for running the executables for Calibration and Augumented Reality code:

Note: Once you download all the files (.cpp & .txt) place the "main.cpp" in a seperate folder named as "project4" along with the "CMakeLists.txt" file

Instructions for testing my "main.cpp" code:

In this program file we have implemented the camera calibration process using opencv findchessboardcorner function and cornersubpix functions
once we have found the corners we are performing calibration process using opencv calibratecamera module, then we calculate the current position of the camera using solvePNP function
then we display a object on the checkerboard with the help of projectPoints function
The keywords for implementing the above tasks:
Press "s" for the saving the vector of corners found in that particular live frame (need atleast five corner data to perform the calibration process)
Press "c" for the camera calibration process (need atleast five corner data to perform the calibration process)
Press "d" to display the rotation and translation data obtained using solvePnP function
Press "e" to display the outside 4 corner points on the checker board
Press "v" to display the virtual object on the checker board
Press "m" to detect multiple checker board and display the objects on them

EXPECTED OUTPUT :

Display a object on top of the checkerboard



Instructions for running the executables for Harris corner detection code:

Note: Once you download all the files (.cpp & .txt) place the "harris_corner.cpp" and "CMakeLists.txt" in a seperate folder named as "project4_harris"

Instructions for testing my "harris_corner.cpp" code:

Once you create a new folder named as "project4_harris" place the "harris_corner.cpp" and the CMakeLists file

Note: change the project name from "project4" to "project4_harris" whereever the project4 is present

This program uses opencv harris corner detector and displays the features in the live stream

EXPECTED OUTPUT :

Displays the features in the live stream


Note: I am using 2 time travel days