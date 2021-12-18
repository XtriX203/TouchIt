#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#define WHITE cv::Vec3b(255,255,255)
#define BLACK cv::Vec3b(0,0,0)

 
class ObjectDetection
{
private:
	cv::Mat bin;	//the binarized image
	//vector that stores the extreme top,bottom,left and right values of the object
	cv::Mat edge;

	cv::Point FindFirstWhite(cv::Mat binImg);
	void findBorder(std::vector<cv::Point> vec);
	std::vector<cv::Point> checkBordersAround(cv::Point p);
	bool isBorder(cv::Point p);
	bool isWhite(cv::Vec3b p, cv::Mat frame);
public:
	ObjectDetection(cv::Mat bin);
	cv::Mat Detect();
	
};

