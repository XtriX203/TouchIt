#include "Binarization.h"

/// <summary>
/// this function responsible for the binarization process and have all the needed resources for it
/// </summary>
/// <param name="frame">the frame to binarize</param>
/// (the square is the area where we take the color range for the binarization)
/// <param name="pointX">the x value of the top left square</param>
/// <param name="pointY">the x value of the top left square</param>
/// <param name="pointLen">the height and width of the square</param>
/// <returns>the binarized frame</returns>
cv::Mat Binarization::binarize(cv::Mat frame, int pointX, int pointY, int pointLen)
{
    cv::Mat hsv = convertToHSV(frame);
    cv::Vec3b low = findLow(hsv, pointX, pointY, pointLen);
    cv::Vec3b high = findHigh(hsv, pointX, pointY, pointLen);

    cv::Mat binarized = mask(hsv, low, high);
    return binarized;
}

/// <summary>
/// convert from RGB to HSV
/// </summary>
/// <param name="frame">the original RGB frame</param>
/// <returns>the HSV frame</returns>
cv::Mat Binarization::convertToHSV(cv::Mat frame)
{
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_RGB2HSV);    //later we might change to manual convertion
    return hsv;
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
cv::Vec3b Binarization::findLow(cv::Mat hsv, int pointX, int pointY, int pointLen)
{
    cv::Vec3b low(179, 255, 255);   //will be returned

    for (int i = pointX; i < pointX + pointLen; i++)
    {
        for (int j = pointY; j < pointY + pointLen; j++)
        {
            cv::Vec3b curr = hsv.at<cv::Vec3b>(j, i);

            if (curr[0] < low[0])
            {
                low[0] = curr[0];
            }
            if (curr[1] < low[1])
            {
                low[1] = curr[1];
            }
            if (curr[2] < low[2])
            {
                low[2] = curr[2];
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
cv::Vec3b Binarization::findHigh(cv::Mat hsv, int pointX, int pointY, int pointLen)
{
    cv::Vec3b high(179, 255, 255);   //will be returned

    for (int i = pointX; i < pointX + pointLen; i++)
    {
        for (int j = pointY; j < pointY + pointLen; j++)
        {
            cv::Vec3b curr = hsv.at<cv::Vec3b>(j, i);

            if (curr[0] > high[0])
            {
                high[0] = curr[0];
            }
            if (curr[1] > high[1])
            {
                high[1] = curr[1];
            }
            if (curr[2] > high[2])
            {
                high[2] = curr[2];
            }
        }
    }
    return high;
}

cv::Mat Binarization::mask(cv::Mat hsv, cv::Vec3b low, cv::Vec3b high)
{
    cv::Mat bin = hsv.clone();  //will be the binarized frame
    cv::Size size = hsv.size();
    cv::Vec3b black = { 0,0,0 };
    cv::Vec3b white = { 255,255,255 };

    for (int i = 0; i < size.height; i++)
    {
        for (int j = 0; j < size.width; j++)
        {
            cv::Vec3b curr = bin.at<cv::Vec3b>(i, j);

            if (curr[0] > low[0] && curr[0] < high[0] &&
                curr[1] > low[1] && curr[1] < high[1] &&
                curr[2] > low[2] && curr[2] < high[2])
            {
                bin.at<cv::Vec3b>(i, j) = white;
            }
            else
            {
                bin.at<cv::Vec3b>(i, j) = black;
            }
        }
    }
    return bin;
}
