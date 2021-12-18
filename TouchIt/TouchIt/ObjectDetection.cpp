#include "ObjectDetection.h"

std::vector<int> T_B_L_R = { -1,-1,-1,-1 };


bool isWhite(cv::Point p, cv::Mat bin)
{
	if (bin.at<cv::Vec3b>(p.x, p.y) == WHITE) {
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

}

//findes all borders and colors them (recursive)
void ObjectDetection::findBorder(std::vector<cv::Point2i> vec)
{
	//check if point is white on mat
	//checkMat(&vec, edge)
	if (!vec.empty())
	{
		for (int i = 0; i < vec.size(); i++)
		{
			edge.at<cv::Vec3b>(vec[i]) = WHITE;
			checkMax(vec[i]);
			std::vector<cv::Point2i> nearBorders = checkBordersAround(vec[i]);
			findBorder(nearBorders);
		}
	}
}

//checks for border points near the current point
std::vector<cv::Point2i> ObjectDetection::checkBordersAround(cv::Point2i p)
{
	std::vector<cv::Point2i> ret;
	bool upRight = true, downRight = true, upLeft = true, downLeft = true;

	//right
	if (p.x + 1 < edge.cols)
	{
		cv::Point2i toCheck = cv::Point2i(p.x + 1, p.y);
		if (isBorder(toCheck))
			ret.push_back(toCheck);
	}
	else
	{
		upRight = false;
		downRight = false;
	}
	//left
	if (p.x - 1 > 0)
	{
		cv::Point2i toCheck = cv::Point2i(p.x - 1, p.y);
		if (isBorder(toCheck))
			ret.push_back(toCheck);
	}
	else
	{
		upLeft = false;
		downLeft = false;
	}
	//up
	if (p.y + 1 < edge.rows)
	{
		cv::Point2i toCheck = cv::Point2i(p.x, p.y + 1);
		if (isBorder(toCheck))
			ret.push_back(toCheck);
	}
	else
	{
		upRight = false;
		upLeft = false;
	}
	//down
	if (p.y - 1 > 0)
	{
		cv::Point2i toCheck = cv::Point2i(p.x, p.y - 1);
		if (isBorder(toCheck))
			if (!isWhite(toCheck, edge))
				ret.push_back(toCheck);
	}
	else
	{
		downLeft = false;
		downRight = false;
	}

	if (upRight)
	{
		cv::Point2i toCheck = cv::Point2i(p.x + 1, p.y + 1);
		if (isBorder(toCheck))
			if (!isWhite(toCheck, edge))
				ret.push_back(toCheck);
	}
	if (upLeft)
	{
		cv::Point2i toCheck = cv::Point2i(p.x - 1, p.y + 1);
		if (isBorder(toCheck))
			if (!isWhite(toCheck, edge))
				ret.push_back(toCheck);
	}
	if (downRight)
	{
		cv::Point2i toCheck = cv::Point2i(p.x + 1, p.y - 1);
		if (isBorder(toCheck))
			if (!isWhite(toCheck, edge))
				ret.push_back(toCheck);
	}
	if (downLeft)
	{
		cv::Point2i toCheck = cv::Point2i(p.x - 1, p.y - 1);
		if (isBorder(toCheck))
			if (!isWhite(toCheck, edge))
				ret.push_back(toCheck);
	}

	return ret;
}

///check if the given point is a border
bool ObjectDetection::isBorder(cv::Point2i p)
{
	int whiteCount = 0, blackCount = 0;
	
	//check if the furrent point is part of the object (is white)
	if (!isWhite(p, bin))
	{
		return false;
	}
	
	//right
	if (p.x + 1 < edge.cols)
		isWhite(cv::Point2i(p.x + 1, p.y), bin) ? whiteCount++ : blackCount++;
	
	//left
	if (p.x - 1 > 0)
		isWhite(cv::Point2i(p.x - 1, p.y), bin) ? whiteCount++ : blackCount++;

	//up
	if (p.y + 1 < edge.rows)
		isWhite(cv::Point2i(p.x, p.y + 1), bin) ? whiteCount++ : blackCount++;

	//down
	if (p.y - 1 > 0)
		isWhite(cv::Point2i(p.x, p.y - 1), bin) ? whiteCount++ : blackCount++;

	if (whiteCount > 0 && blackCount > 1)
		return true;
	return false;
}



cv::Mat ObjectDetection::Detect()
{	
	cv::Point2i pixel = FindFirstWhite(this->bin);
	std::vector<cv::Point2i> vec;
	vec.push_back(pixel);
	findBorder(vec);
	return this->edge;
}

cv::Point2i ObjectDetection::FindFirstWhite(cv::Mat bin)
{
	for (int i = 0; i < bin.rows; i++)
	{
		for (int j = 0; j < bin.cols; j++)
		{
			cv::Vec3b pixel = bin.at<cv::Vec3b>(i, j);
			if (pixel == WHITE)
			{
				return cv::Point2i(i, j);
			}
		}
	}
	return cv::Point2i(0, 0);
}
