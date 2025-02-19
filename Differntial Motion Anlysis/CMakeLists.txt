cmake_minimum_required(VERSION 3.24)
project(final_project)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(final_project main.cpp filters.cpp)

target_link_libraries(final_project ${OpenCV_LIBS})
target_link_libraries(final_project Qt6::Core Qt6::Widgets)
target_link_libraries(final_project ${OpenCV_LIBS})
target_link_libraries(final_project Qt6::Core Qt6::Widgets)