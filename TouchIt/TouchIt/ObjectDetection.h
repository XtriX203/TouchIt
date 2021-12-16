#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#define WHITE cv::Vec3b(255,255,255)
#define BLACK cv::Vec3b(0,0,0)

 
class ObjectDetection
{
public:
	cv::Mat Detect(cv::Mat bin);
	cv::Point FindFirstWhite(cv::Mat bin);
};