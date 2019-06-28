#include "plateSegmentation.h"

Mat preprocessPlate(Mat & plateImg)
{
	Mat refinedImg = plateImg.clone();
//	for (int i = 0; i < refinedImg.rows; i++)
//	{
//		for (int j = 0; j < refinedImg.cols; j++)
//		{
//			if ( distanceFromBlack(refinedImg.at<Vec3b>(i,j)) > ALMOST_BLACK)
//			{
//				Vec3b & color = refinedImg.at<Vec3b>(i,j);
//				color[0] = 255;
//				color[1] = 255;
//				color[2] = 255;
//			}
//		}
//	}
//	showIm(refinedImg, "White mask applied");

        cvtColor(refinedImg, refinedImg, CV_BGR2GRAY);

	adaptiveThreshold(refinedImg, refinedImg, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE1, ADAPTIVE_THRESH_WEIGHT1);
	showIm(refinedImg, "Thresholded image");

	return refinedImg;
}


double distanceFromBlack(Vec3b v)
{
	double d = std::sqrt(std::pow(v[0],2.) + std::pow(v[1],2.) + std::pow(v[2],2.));
	return d;
}
