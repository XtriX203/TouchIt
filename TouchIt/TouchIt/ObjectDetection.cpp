#include "ObjectDetection.h"

std::vector<int> T_B_L_R = { -1,-1,-1,-1 };


bool ObjectDetection::isWhite(cv::Vec3b p, cv::Mat frame)
{
	if (p == WHITE) {
		return true;
	}
	return false;
}



//check if the pixel is an extreme pixel an updates if so
void checkMax(cv::Point p)
{
	//set the initial values of the object extreme borders to the first point
	if (T_B_L_R[0] == -1)
	{
		T_B_L_R[0] = p.y;
		T_B_L_R[1] = p.y;
		T_B_L_R[2] = p.x;
		T_B_L_R[3] = p.x;
	}
	else
	{
		//if the pixel is topper then the stored top (in the frame the order of the lines starts from the top)
		if (T_B_L_R[0] > p.y)
		{
			T_B_L_R[0] = p.y;
		}
		//pixel is lower then bottom
		if (T_B_L_R[1] < p.y)
		{
			T_B_L_R[1] = p.y;
		}
		//pixel is extreme left
		if (T_B_L_R[2] > p.x)
		{
			T_B_L_R[2] = p.x;
		}
		//pixel is extreme right
		if (T_B_L_R[3] < p.x)
		{
			T_B_L_R[3] = p.x;
		}
	}
}

//constractor
ObjectDetection::ObjectDetection(cv::Mat binImg)
{
	this->bin = binImg;
	this->edge = cv::Mat(binImg.rows, binImg.cols, CV_8UC3);
	this->AlignmentEdge = cv::Mat(binImg.rows, binImg.cols, CV_8UC3);
	for (int i = 0; i < edge.rows; i++)
	{
		for (int j = 0; j < edge.cols; j++)
		{
			edge.at<cv::Vec3b>(i, j) = BLACK;
			AlignmentEdge.at<cv::Vec3b>(i, j) = BLACK;
		}
	}
}

//findes all borders and colors them (recursive)
void ObjectDetection::findBorder(std::vector<cv::Point> vec)
{
	//check if point is white on mat
	//checkMat(&vec, edge)
	if (!vec.empty())
	{
		for (int i = 0; i < vec.size(); i++)
		{
			edge.at<cv::Vec3b>(vec[i].x, vec[i].y) = WHITE; //some pixels are not colored and its make an infinite loop 
			//AlignmentEdge.at<cv::Vec3b>(vec[i].x, vec[i].y) = WHITE;//the visual edge detection
			checkMax(vec[i]);
			std::vector<cv::Point> nearBorders = checkBordersAround(vec[i]);
			findBorder(nearBorders);
			
		}
	}
}

//checks for border points near the current point
std::vector<cv::Point> ObjectDetection::checkBordersAround(cv::Point p)
{
	std::vector<cv::Point> ret;
	bool upRight = true, downRight = true, upLeft = true, downLeft = true;

	//right
	if (p.y + 1 < edge.cols)
	{
		cv::Point toCheck1 = cv::Point(p.x, p.y + 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck1), edge))
			if (isBorder(toCheck1))
				ret.push_back(toCheck1);
	}	
	else
	{
		upRight = false;
		downRight = false;
	}
	//left
	if (p.y - 1 > 0)
	{
		cv::Point toCheck2 = cv::Point(p.x, p.y - 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck2), edge))
			if (isBorder(toCheck2))
				ret.push_back(toCheck2);
	}
	else
	{
		upLeft = false;
		downLeft = false;
	}
	//down
	if (p.x + 1 < edge.rows)
	{
		cv::Point toCheck3 = cv::Point(p.x + 1, p.y);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck3), edge))
			if (isBorder(toCheck3))
				ret.push_back(toCheck3);
	}
	else
	{
		upRight = false;
		upLeft = false;
	}
	//up
	if (p.x - 1 > 0)
	{
		cv::Point toCheck4 = cv::Point(p.x-1, p.y);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck4), edge))
			if (isBorder(toCheck4))
				ret.push_back(toCheck4);
	}
	else
	{
		downLeft = false;
		downRight = false;
	}

	if (downRight)
	{
		cv::Point toCheck5 = cv::Point(p.x + 1, p.y - 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck5), edge))
			if (isBorder(toCheck5))
				ret.push_back(toCheck5);
	}
	if (upLeft)
	{
		cv::Point toCheck6 = cv::Point(p.x - 1, p.y - 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck6), edge))
			if (isBorder(toCheck6))
				ret.push_back(toCheck6);
	}
	if (upRight)
	{
		cv::Point toCheck7 = cv::Point(p.x+1, p.y + 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck7), edge))
			if (isBorder(toCheck7))
				ret.push_back(toCheck7);
	}
	if (downLeft)
	{
		cv::Point toCheck8 = cv::Point(p.x + 1, p.y - 1);
		if (!isWhite(edge.at<cv::Vec3b>(toCheck8), edge))
			if (isBorder(toCheck8))
				ret.push_back(toCheck8);
	}

	return ret;
}

///check if the given point is a border
bool ObjectDetection::isBorder(cv::Point p)
{
	int whiteCount = 0, blackCount = 0;
	
	//check if the current point is part of the object (is white)
	if (!isWhite(bin.at<cv::Vec3b>(p.x, p.y), bin))
	{
		return false;
	}
	
	//right
	if (p.y + 1 < bin.cols)
		isWhite(bin.at<cv::Vec3b>(p.x, p.y + 1), bin) ? whiteCount++ : blackCount++;
	
	//left
	if (p.y - 1 > 0)
		isWhite(bin.at<cv::Vec3b>(p.x, p.y-1), bin) ? whiteCount++ : blackCount++;

	//down
	if (p.x + 1 < bin.rows)
		isWhite(bin.at<cv::Vec3b>(p.x + 1, p.y), bin) ? whiteCount++ : blackCount++;

	//up
	if (p.x - 1 > 0)
		isWhite(bin.at<cv::Vec3b>(p.x - 1, p.y), bin) ? whiteCount++ : blackCount++;

	if (whiteCount > 0 && blackCount > 0)
		return true;
	return false;
}



cv::Mat ObjectDetection::Detect()
{	
	cv::Point pixel = FindFirstWhite(this->bin);
 	std::vector<cv::Point> vec;
	vec.push_back(pixel);
	findBorder(vec);
	return this->edge;
}

cv::Point ObjectDetection::FindFirstWhite(cv::Mat bin)
{
	for (int i = 0; i < bin.rows; ++i)//its the Y(down)
	{
		for (int j = 0; j < bin.cols; ++j)//its the X(right)
		{
			cv::Vec3b pixel = bin.at<cv::Vec3b>(i, j);
			if (pixel == WHITE)
			{
				return cv::Point(i , j); //test;
			}
		}
	}
	return cv::Point(0, 0);
}
