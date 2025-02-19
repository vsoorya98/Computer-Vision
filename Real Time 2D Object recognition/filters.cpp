/*
 * Created by SHIRISH KISHORE KUMAR & SOORYA
 * In this program file we use the opencv connectedcomponentwithstats function to get the regions
 * Then we assign different shades of pink to different regions
 */

#include <opencv2/opencv.hpp>
#include <numeric>

using namespace cv;
using namespace std;


cv::Mat adding_color_to_the_foreground(cv::Mat& src, cv::Mat& lrg, cv::Mat& rg, cv::Mat& center, std::vector<int>& fg_objects)
{
    cv::Mat dst;
    // Reference
    // https://docs.opencv.org/3.4/d3/dc0/group__imgproc__shape.html
    int numRegions = connectedComponentsWithStats(src, lrg, rg, center);

    // Assigning a color to each region
    std::vector<cv::Vec3b> region_color(numRegions);
    // Setting the background color to black
    region_color[0] = cv::Vec3b(0, 0, 0);
    // Assigning different colors for different foreground objects
    cv::Vec3b foregroundColor(255, 80, 255);
    for (int i = 1; i < numRegions; i++) {
        float r = (float)i / (float)numRegions * 150.0f;
        float g = (float)i / (float)numRegions * 20.0f;
        float b = (float)i / (float)numRegions * 200.0f;
        region_color[i] = cv::Vec3b(r, g, b);
    }

    // Sort regions by size in descending order
    std::vector<int> sorted_regions(numRegions - 1);
    std::iota(sorted_regions.begin(), sorted_regions.end(), 1);
    std::sort(sorted_regions.begin(), sorted_regions.end(), [&](int i, int j) {return rg.at<int>(i, cv::CC_STAT_AREA) > rg.at<int>(j, cv::CC_STAT_AREA);});

    // Choose the top regions based on their size and add their labels to the fg_objects vector
    fg_objects.clear();
    int maxRegions = 20; // maximum number of regions to display
    maxRegions = (maxRegions < sorted_regions.size()) ? maxRegions : sorted_regions.size();
    int minRegionSize = 100; // minimum region area to display
    for (int i = 0; i < maxRegions; i++) {
        int label = sorted_regions[i];
        if (rg.at<int>(label, cv::CC_STAT_AREA) > minRegionSize)
        {
            fg_objects.push_back(label);
        }
    }

    // Color the regions in the output image based on their labels
    dst = cv::Mat::zeros(lrg.size(), CV_8UC3);
    for (int i = 0; i < lrg.rows; i++) {
        for (int j = 0; j < lrg.cols; j++) {
            int label = lrg.at<int>(i, j);
            dst.at<cv::Vec3b>(i, j) = region_color[label];
        }
    }

    return dst;
}
