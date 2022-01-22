#include "ObjectDetection.h"

std::vector<int> T_B_L_R = { -1,-1,-1,-1 };


bool ObjectDetection::isWhite(cv::Vec3b p, cv::Mat frame)
{
	if (p == WHITE) {
		return true;
	}
	return false;
}

void ObjectDetection::resetMaxHolder()
{
	T_B_L_R[0] = -1;
	T_B_L_R[1] = -1;
	T_B_L_R[2] = -1;
	T_B_L_R[3] = -1;
}

void ObjectDetection::colorRect(cv::Mat& img)
{
	for (int i = T_B_L_R[0]; i <= T_B_L_R[1]; i++)
	{
		for (int j = T_B_L_R[2]; j <= T_B_L_R[3]; j++)
		{
			img.at<cv::Vec3b>(i, j) = BLACK;
		}
	}
}

//check if the pixel is an extreme pixel an updates if so
void checkMax(cv::Point p)
{
	//set the initial values of the object extreme borders to the first point
	if (T_B_L_R[0] == -1)
	{
		T_B_L_R[0] = p.x;
		T_B_L_R[1] = p.x;
		T_B_L_R[2] = p.y;
		T_B_L_R[3] = p.y;
	}
	else
	{
		//if the pixel is topper then the stored top (in the frame the order of the lines starts from the top)
		if (T_B_L_R[0] > p.x)
		{
			T_B_L_R[0] = p.x;
		}
		//pixel is lower then bottom
		if (T_B_L_R[1] < p.x)
		{
			T_B_L_R[1] = p.x;
		}
		//pixel is extreme left
		if (T_B_L_R[2] > p.y)
		{
			T_B_L_R[2] = p.y;
		}
		//pixel is extreme right
		if (T_B_L_R[3] < p.y)
		{
			T_B_L_R[3] = p.y;
		}
	}
}

//constractor
ObjectDetection::ObjectDetection(cv::Mat binImg)
{
	this->_bin = binImg;
	this->_edge = cv::Mat(binImg.rows, binImg.cols, CV_8UC3);
	//this->AlignmentEdge = cv::Mat(binImg.rows, binImg.cols, CV_8UC3);
	for (int i = 0; i < _edge.rows; i++)
	{
		for (int j = 0; j < _edge.cols; j++)
		{
			_edge.at<cv::Vec3b>(i, j) = BLACK;
			//AlignmentEdge.at<cv::Vec3b>(i, j) = BLACK;
		}
	}
	this->_bigestShape = cv::Rect(0, 0, 0, 0);
}

//findes all borders and colors them 
cv::Rect ObjectDetection::findBorder(std::queue<cv::Point>& queue)
{
	//check if point is white on mat
	//checkMat(&vec, edge)
	cv::Point curr;
	while (!queue.empty())
	{
		curr = queue.front();
		queue.pop();

		_edge.at<cv::Vec3b>(curr.x, curr.y) = WHITE; //some pixels are not colored and its make an infinite loop 
		//AlignmentEdge.at<cv::Vec3b>(vec[i].x, vec[i].y) = WHITE;//the visual edge detection
		checkMax(curr);
		checkBordersAround(curr, queue);
	}
	cv::Rect rect = cv::Rect(cv::Point(T_B_L_R[2], T_B_L_R[0]), cv::Point(T_B_L_R[3], T_B_L_R[1]));
	//cv::rectangle(_edge, rect, cv::Scalar(255, 255, 0), 1);
	return rect;
}

//checks for border points near the current point
void ObjectDetection::checkBordersAround(cv::Point p, std::queue<cv::Point>& ret)
{

	bool upRight = true, downRight = true, upLeft = true, downLeft = true;

	//right
	if (p.y + 1 < _edge.cols)
	{
		cv::Point toCheck1 = cv::Point(p.x, p.y + 1);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck1.x, toCheck1.y), _edge))
			if (isBorder(toCheck1))
				ret.push(toCheck1);
	}
	else
	{
		upRight = false;
		downRight = false;
	}
	//left
	if (p.y - 1 >= 0)
	{
		cv::Point toCheck2 = cv::Point(p.x, p.y - 1);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck2.x, toCheck2.y), _edge))
			if (isBorder(toCheck2))
				ret.push(toCheck2);
	}
	else
	{
		upLeft = false;
		downLeft = false;
	}
	//down
	if (p.x + 1 < _edge.rows)
	{
		cv::Point toCheck3 = cv::Point(p.x + 1, p.y);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck3.x, toCheck3.y), _edge))
			if (isBorder(toCheck3))
				ret.push(toCheck3);
	}
	else
	{
		downRight = false;
		downLeft = false;
	}
	//up
	if (p.x - 1 >= 0)
	{
		cv::Point toCheck4 = cv::Point(p.x - 1, p.y);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck4.x, toCheck4.y), _edge))
			if (isBorder(toCheck4))
				ret.push(toCheck4);
	}
	else
	{
		upLeft = false;
		upRight = false;
	}

	if (downRight)
	{
		cv::Point toCheck5 = cv::Point(p.x + 1, p.y + 1);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck5.x, toCheck5.y), _edge))
			if (isBorder(toCheck5))
				ret.push(toCheck5);
	}
	if (downLeft)
	{
		cv::Point toCheck6 = cv::Point(p.x + 1, p.y - 1);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck6.x, toCheck6.y), _edge))
			if (isBorder(toCheck6))
				ret.push(toCheck6);
	}
	if (upRight)
	{
		cv::Point toCheck7 = cv::Point(p.x - 1, p.y + 1);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck7.x, toCheck7.y), _edge))
			if (isBorder(toCheck7))
				ret.push(toCheck7);
	}
	if (upLeft)
	{
		cv::Point toCheck8 = cv::Point(p.x - 1, p.y - 1);
		if (!isWhite(_edge.at<cv::Vec3b>(toCheck8.x, toCheck8.y), _edge))
			if (isBorder(toCheck8))
				ret.push(toCheck8);
	}

}

///check if the given point is a border
bool ObjectDetection::isBorder(cv::Point p)
{
	int whiteCount = 0, blackCount = 0;

	//check if the current point is part of the object (is white)
	if (!isWhite(_bin.at<cv::Vec3b>(p.x, p.y), _bin))
	{
		return false;
	}

	//right
	if (p.y + 1 < _bin.cols)
		isWhite(_bin.at<cv::Vec3b>(p.x, p.y + 1), _bin) ? whiteCount++ : blackCount++;

	//left
	if (p.y - 1 > 0)
		isWhite(_bin.at<cv::Vec3b>(p.x, p.y - 1), _bin) ? whiteCount++ : blackCount++;

	//down
	if (p.x + 1 < _bin.rows)
		isWhite(_bin.at<cv::Vec3b>(p.x + 1, p.y), _bin) ? whiteCount++ : blackCount++;

	//up
	if (p.x - 1 > 0)
		isWhite(_bin.at<cv::Vec3b>(p.x - 1, p.y), _bin) ? whiteCount++ : blackCount++;

	if (whiteCount > 0 && blackCount > 0)
		return true;
	return false;
}



cv::Mat ObjectDetection::detect()
{
	cv::Rect rect = cv::Rect(cv::Point(0, 0), cv::Point(0, 0));
	cv::Mat temp = this->_bin.clone();

	resetMaxHolder();
	while (/*rect.area() < this->bin.total() / 16*/ true)//if the object is smaller then sixth the image
	{
		cv::Point pixel = findFirstWhite(temp);
		if (pixel.x == -1)//while didnt found
		{
			return this->_edge;
		}
		std::queue<cv::Point> vec;
		vec.push(pixel);
		rect = findBorder(vec);
		if (rect.area() > this->_bigestShape.area())
		{
			this->_bigestShape = rect;
		}
		colorRect(temp);
		resetMaxHolder();
	}
	return this->_edge;
}

cv::Mat ObjectDetection::getHand()
{
	cv::Mat ret = cv::Mat(this->_bigestShape.size().height + 1, this->_bigestShape.size().width + 1, CV_8UC3);

	int top = this->_bigestShape.tl().y, bot = this->_bigestShape.br().y;
	int left = this->_bigestShape.tl().x, right = this->_bigestShape.br().x;

	for (int i = top; i <= bot; i++)
	{
		for (int j = left; j <= right; j++)
		{
			ret.at<cv::Vec3b>(i - top, j - left) = this->_edge.at<cv::Vec3b>(i, j);
		}
	}
	return ret;
}

cv::Point ObjectDetection::findFirstWhite(cv::Mat bin)
{
	for (int i = 0; i < bin.rows; ++i)//its the Y(down)
	{
		for (int j = 0; j < bin.cols; ++j)//its the X(right)
		{
			cv::Vec3b pixel = bin.at<cv::Vec3b>(i, j);
			if (pixel == WHITE)
			{
				if (!(i >= T_B_L_R[0] && i <= T_B_L_R[1] && j >= T_B_L_R[2] && j <= T_B_L_R[3]))
				{
					return cv::Point(i, j); //test;
				}
			}
		}
	}
	return cv::Point(-1, -1);
}
