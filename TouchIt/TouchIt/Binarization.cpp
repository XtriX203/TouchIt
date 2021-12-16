#include "Binarization.h"
#define ANGLE 30
#define MAX_ANGLE 180

#define UNDEFINED -1
//for RGB
#define RED 0
#define GREEN 1
#define BLUE 2
//for HSV
#define HUE 0
#define SAT 1
#define VAL 2

#define RGB_MAX 255


/// <summary>
/// convert frame from RGB to HSV
/// </summary>
/// <param name="frame">the original RGB frame</param>
/// <returns>the HSV frame</returns>
cv::Mat Binarization::convertToHSV(cv::Mat frame)
{
	cv::Mat hsv = frame.clone();	//the HSV frame to return *** find a way to set the size of the mat without cloning the pic! ***
	cv::Size size = frame.size();	//the dimentions of the frame

	//manual RGB to HSV (for efficiency, every square of 2x2 is the same hsv color as the top right of the square)
	for (int i = 0; i < size.height; i += 2)
	{
		for (int j = 0; j < size.width; j += 2)
		{
			cv::Vec3b hsvColor;							//the hsv color of the pixel
			cv::Vec3b curr = frame.at<cv::Vec3b>(i, j);	//the current pixel

			hsvColor = pixelRGBToHSV(curr);	//convert pixel from RGB to HSV
			if (hsvColor[RED] == 0 && hsvColor[GREEN] == 0 && hsvColor[BLUE] == 0)	//if the function in the line above could not determine the dominant color
			{
				if (i != 0)	//set this pixels values as the pixel above
				{
					hsvColor = hsv.at<cv::Vec3b>(i - 1, j);
				}
				else if (j != 0)	//set this pixels values as the pixel to the left
				{
					hsvColor = hsv.at<cv::Vec3b>(i, j - 1);
				}
				else	//if it is the first pixel it will be black
				{
					hsvColor = cv::Vec3b(0, 0, 0);
				}
			}

			//set the hsv values for the 2x2 sqare
			hsv.at<cv::Vec3b>(i, j) = hsvColor;
			hsv.at<cv::Vec3b>(i, j + 1) = hsvColor;
			hsv.at<cv::Vec3b>(i + 1, j) = hsvColor;
			hsv.at<cv::Vec3b>(i + 1, j + 1) = hsvColor;
		}
	}
	return hsv;
}

/// <summary>
/// convert a single pixel from RGB to HSV
/// </summary>
/// <param name="pixel">a pixel with RGB values</param>
/// <returns>a pixel with HSV values</returns>
cv::Vec3b Binarization::pixelRGBToHSV(cv::Vec3b pixel)
{
	int hue = 0;					//H in HSV
	double sat = 0;					//S in HSV
	int val = 0;					//V in HSV

	int maxRGBIndex = UNDEFINED;	//the RGB mos dominant value
	int minRGBIndex = UNDEFINED;	//the RGB least dominant value
	cv::Vec3d doublePixel;  //pixel with range 0UNDEFINED for the HUE calculation

	//find the max RGB value (the dominant color)
	if (pixel[RED] > pixel[GREEN] && pixel[RED] > pixel[BLUE])	//max is red
	{
		maxRGBIndex = RED;
	}
	else if (pixel[GREEN] > pixel[RED] && pixel[GREEN] > pixel[BLUE])	//max is green
	{
		maxRGBIndex = GREEN;
	}
	else if (pixel[BLUE] > pixel[RED] && pixel[BLUE] > pixel[GREEN])	//max is blue
	{
		maxRGBIndex = BLUE;
	}
	//if there is no dominant color return black
	if (maxRGBIndex == UNDEFINED)
	{
		return cv::Vec3b(0, 0, 0);
	}

	//***set the value (V in HSV)
	val = pixel[maxRGBIndex];	//the value is equal to the highest value in RGB

	//find the min RGB value (the least dominant color)
	if (pixel[(maxRGBIndex + 1) % 2] < pixel[(maxRGBIndex + 2) % 2])
	{
		minRGBIndex = (maxRGBIndex + 1) % 2;
	}
	else
	{
		minRGBIndex = (maxRGBIndex + 2) % 2;
	}
	//***calculate the saturation
	sat = (int)std::round(255 * (pixel[maxRGBIndex] - pixel[minRGBIndex]) / pixel[maxRGBIndex]);

	//set double pixel values (3 values)
	for (int i = 0; i < 3; i++)
	{
		doublePixel[i] = (double)pixel[i] / RGB_MAX;
	}
	//***calculate the HUE (the hue calculation is based on the dominant color)
	switch (maxRGBIndex)
	{
	case RED:	//HUE is 0-30 or 150UNDEFINED79
		hue = ANGLE * (doublePixel[GREEN] - doublePixel[BLUE]) / (doublePixel[maxRGBIndex] - doublePixel[minRGBIndex]);
		if (hue < 0)	//in this case the HUE can be as low as -30 degrees so we need to circle back to 180 and decrease from there because the HUE values are cyclic values
		{
			hue = MAX_ANGLE - hue;
		}
		break;

	case GREEN:	//HUE is 30-90
		hue = ANGLE * (2 + doublePixel[BLUE] - doublePixel[RED]) / (doublePixel[GREEN] - doublePixel[minRGBIndex]);
		break;

	case BLUE:	//HUE is 90UNDEFINED50
		hue = ANGLE * (4 + doublePixel[RED] - doublePixel[GREEN]) / (doublePixel[BLUE] - doublePixel[minRGBIndex]);
		break;

	default:
		//will not happen, the funtion should return [0,0,0] if it could not determine the dominant color earlier
		break;
	}

	return cv::Vec3b(hue, sat, val);
}

/// <summary>
/// find the lowest HSV values for the color range
/// </summary>
/// <param name="hsv">the HSV frame to work with</param>
/// (the square is the area where we take the color range for the binarization)
/// <param name="pointX">the x value of the top left square</param>
/// <param name="pointY">the x value of the top left square</param>
/// <param name="pointLen">the height and width of the square</param>
/// <returns>a pixel with the lowest HSV values in the square</returns>
cv::Vec3b Binarization::findLow(cv::Mat frame, int pointX, int pointY, int pointLen)
{
	cv::Vec3b low(255, 255, 255);	//initialize with the highest values. will be returned

	for (int i = pointX; i < pointX + pointLen; i++)
	{
		for (int j = pointY; j < pointY + pointLen; j++)
		{
			cv::Vec3b curr = frame.at<cv::Vec3b>(j, i);	//the current pixel

			if (curr[RED] < low[RED])	//check if the current RED is lower
			{
				low[RED] = curr[RED];
			}
			if (curr[GREEN] < low[GREEN])	//check if the current GREEN is lower
			{
				low[GREEN] = curr[GREEN];
			}
			if (curr[BLUE] < low[BLUE])	//check if the current BLUE is lower
			{
				low[BLUE] = curr[BLUE];
			}
		}
	}
	return low;
}

/// <summary>
/// find the highest HSV values for the color range
/// </summary>
/// <param name="hsv">the HSV frame to work with</param>
/// (the square is the area where we take the color range for the binarization)
/// <param name="pointX">the x value of the top left square</param>
/// <param name="pointY">the x value of the top left square</param>
/// <param name="pointLen">the height and width of the square</param>
/// <returns>a pixel with the highest HSV values in the square</returns>
cv::Vec3b Binarization::findHigh(cv::Mat frame, int pointX, int pointY, int pointLen)
{
	cv::Vec3b high(0, 0, 0);	
	
	/*i will not remove the code yet, i thing more testing is needed*/
	for (int i = pointX; i < pointX + pointLen; i++)
	{
		for (int j = pointY; j < pointY + pointLen; j++)
		{
			cv::Vec3b curr = frame.at<cv::Vec3b>(j, i);

			if (curr[RED] > high[RED])
			{
				high[RED] = curr[RED];
			}
			if (curr[GREEN] > high[GREEN])
			{
				high[GREEN] = curr[GREEN];
			}
			if (curr[BLUE] > high[BLUE])
			{
				high[BLUE] = curr[BLUE];
			}
		}
	}
	return high;
}

/// <summary>
/// binarize the frame
/// </summary>
/// <param name="hsv">the frame in HSV color space</param>
/// <param name="low">the lowest HSV values for the range</param>
/// <param name="high">the highest HSV values for the range</param>
/// <returns>the mask in RGB color space</returns> (later might be changed to binary color space)
cv::Mat Binarization::mask(cv::Mat frame, cv::Vec3b low, cv::Vec3b high)
{
	cv::Mat bin= cv::Mat(frame.size().height,frame.size().width, CV_8UC3);	//will be the binarized frame 
	
	cv::Size size = bin.size();	//the dimations of the frame
	cv::Vec3b black = { 0,0,0 };		//black RGB
	cv::Vec3b white = { 255,255,255 };	//white RGB

	for (int i = 0; i < size.height; i++)
	{
		for (int j = 0; j < size.width; j++)
		{
			cv::Vec3b curr = frame.at<cv::Vec3b>(i, j);	//the current pixel

			//check if the color is in the color range
			if (curr[RED] > low[RED] && curr[RED] < high[RED] &&
				curr[GREEN] > low[GREEN] && curr[GREEN] < high[GREEN] &&
				curr[BLUE] > low[BLUE] && curr[BLUE] < high[BLUE])
			{
				bin.at<cv::Vec3b>(i, j) = white;	//set pixel as withe
			}
			else	//if not in color range
			{
				bin.at<cv::Vec3b>(i, j) = black;	//set pixel as black
			}
		}
	}
	return bin;
}
