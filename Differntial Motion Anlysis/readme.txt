The operating I used is macos ventura, I used Clion and arduinoIDE as my IDE

Members worked in this project :

Shirish Kishore Kumar
Soorya Vijayaragavan


Instructions for running the executables:

Note : Once you download all the .cpp, .ino and .h files, create a new folder named 'final_project' and put all the files in that folder.
1)-> Then upload the 'excecute.ino' file to the arduino board using arduino IDE.
2)-> Before running the code change the port number in the code to the port number of your arduino board which can be
     found in the arduino IDE. change this in the main.cpp file in line 28. [i.e)"int serial_fd = configure_serial("/dev/cu.usbmodem14301");"]
3)-> Then open the folder in Clion and run the code.

Instructions for testing my code:

Once you run the code:
if you press 'g' or 'G' it will apply gray scale filter
if you press 'n' or 'N' it will apply normal filter
if you press 'b' or 'B' it will create background image
if you press 's' or 'S' it will perform saturated subtraction
if you press 'q' or 'Q' it will quit the program
if you press 't' or 'T' it will apply thresholding
if you press 'm' or 'M' it will apply magnitude
if you press 'x' or 'X' it will display all the process from the beginning and display the saturated difference image
if you press 'f' or 'F' it will turn on the external device like fan, light, etc using arduino

EXPECTED OUTPUT :

1)-> Gray scale filter
2)-> Normal filter
3)-> Background image
4)-> Saturated subtraction
5)-> Thresholding
6)-> Magnitude
7)-> Display all the process from the beginning and display the saturated difference image
8)-> Turn on the external device like fan, light, etc using arduino

Project Description:

For our final project we will be implementing automatic home light turn on/off using differential motion analysis. The reason we
chose to do this is because at present the best automatic light systems use motion sensors and the issue with these sensors is that
after some time even though humans are still in a room they turn off because the motion sensors require large motions to detect any
moving object and if they detect any large motions they use timer to keep the light on from the time the sensor has detected anything.
In order to solve this issue we will be using differential motion analysis, and this would be a perfect for this application since the
camera will be fixed in a single location, In order to implement this we first create a probabilistic model for each pixel over a day
so that we can capture illumination changes, if we find any changes over some particular threshold then we turn on the lights.
To turn on/off lights we will be using Arduino along with relays to handle the AC powered lights.

Project working/demonstration video link:
https://drive.google.com/file/d/16NDIudM8Ua6_pZQkBJLoruwPtST1YxrY/view?usp=sharing

Project presentation video link:
https://drive.google.com/file/d/1rTe_11pxBJOuIA8MkDHlKZyfd98YSyIF/view?usp=sharing

PPT link:
https://docs.google.com/presentation/d/134UI2AJnvoFeKeGRwh1aYWnZ7vMTJSdq8W-D7pk0k4E/edit?usp=sharing

Thank you :)

