#include "ObjectDetection.h"

std::vector<int> T_B_L_R = { -1,-1,-1,-1 };


bool isWhite(cv::Point p, cv::Mat bin)
{
	if (bin.at<cv::Vec3b>(p.x, p.y) == WHITE) {
		return true;
	}
	return false;
}

bool Move(std::vector<cv::Point>& vec, cv::Mat& bin)
{//do the same as Detect and return if pixel given is match the condition
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
	bin = binImg;
	edge = cv::Mat(binImg.rows, binImg.cols, CV_8UC3);

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



cv::Mat ObjectDetection::Detect(cv::Mat bin)
{
	cv::Mat ret = cv::Mat(bin.rows, bin.cols, CV_8UC3);
	cv::Point pixel = FindFirstWhite(bin);
	cv::Point up;
	pixel.y != 0 ? up = cv::Point(pixel.x, pixel.y - 1) : up = pixel;
	cv::Point down;
	cv::Point right;
	cv::Point left;
	int BCount = 0, WCount = 0;
	bool flag = true;
	std::vector<cv::Point> pixels;

	while (flag)
	{
		//added according to the sceme
		//if true set the up pixel as curr pixel and call the function
		if (Move(pixels, bin)) {
			for (int i = 0; i < 4; i++)
			{
				ret.at<cv::Vec3b>(pixels[i].x, pixels[i].y) = WHITE;
				checkMax(pixels[i]);
			}
		}
		pixel.y != bin.cols ? down = cv::Point(pixel.x, pixel.y + 1) : down = pixel;
		pixel.x != 0 ? left = cv::Point(pixel.x - 1, pixel.y) : left = pixel;
		pixel.x != bin.rows ? right = cv::Point(pixel.x + 1, pixel.y) : right = pixel;


		isWhite(up, bin) ? WCount++ : BCount++;
		isWhite(down, bin) ? WCount++ : BCount++;
		isWhite(right, bin) ? WCount++ : BCount++;
		isWhite(left, bin) ? WCount++ : BCount++;

		if (WCount > 0 && BCount > 0)
		{
			ret.at<cv::Vec3b>(pixel.x, pixel.y) = WHITE;
		}
		else
		{
			ret.at<cv::Vec3b>(pixel.x, pixel.y) = BLACK;
		}
	}

	return ret;
}

cv::Point ObjectDetection::FindFirstWhite(cv::Mat bin)
{
	for (int i = 0; i < bin.rows; i++)
	{
		for (int j = 0; j < bin.cols; j++)
		{
			cv::Vec3b pixel = bin.at<cv::Vec3b>(i, j);
			if (pixel == WHITE)
			{
				return cv::Point(i, j);
			}
		}
	}
	return cv::Point(0, 0);
}
