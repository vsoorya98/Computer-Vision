The operating I used is macos ventura and I used Clion as my IDE

Members worked in this project :

Shirish Kishore Kumar
Soorya Vijayaragavan


Instructions for running the executables:

Note : Once you download all the .cpp and .h files, create three new folders named driver, infer, train
       place the main.cpp and test.cpp in the driver folder
       place the Classifier.cpp and .h in the infer folder
       place the feature.cpp and .h, trainingData.cpp and .h in the train folder
       place the rest of the files the main folder, also name the main folder as 'CV' or you would have to make changes in the CMakeLists.txt file (if you dont follow the above steps )


Instructions for testing my code:

we have two main modes Training and inference mode

When you want to enter the object name press the respective objects label number to store the objects name into the CSV file
When the user presses 'S' or 's' it saves the frame
It also asks for the image name to be saved
When the user presses the 'Q' or 'q' the program quits
Press 'B' or 'b' for Thresholding
Press 'M' or 'm' for Morphological processing
Press 'C' or 'c' for applying color to the regions
Press 'N' or 'n' for training mode
Press the labels respective number to select that object and name it
Press 'W' or 'w' to save the csv file once you have labeled all the parts
Press 'I' or 'i' for inference mode

EXPECTED OUTPUT :

When you press 'n/N' it goes into training mode , select objects by entering its label number once you are done save the csv file by presssing 'W/w'
when you press 'I/i' it goes for inteference mode, it should display the objects that are similar the object placed before


Note: I am using 3 time travel days