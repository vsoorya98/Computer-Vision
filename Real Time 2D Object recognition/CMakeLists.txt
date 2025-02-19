cmake_minimum_required(VERSION 3.24)
project(CV)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(CV driver/main.cpp train/trainingData.cpp train/trainingData.h train/feature.cpp train/feature.h csv_util.cpp csv_util.h infer/Classifier.h infer/Classifier.cpp utils.h)
add_executable(CV_test driver/test.cpp train/trainingData.cpp train/trainingData.h train/feature.cpp train/feature.h csv_util.cpp csv_util.h)

target_link_libraries(CV ${OpenCV_LIBS})
target_link_libraries(CV Qt6::Core Qt6::Widgets)
target_link_libraries(CV_test ${OpenCV_LIBS})
target_link_libraries(CV_test Qt6::Core Qt6::Widgets)

