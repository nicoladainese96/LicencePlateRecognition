#include "preprocessing.h"

Mat preprocessing(Mat & originalImg)
{
	showIm(originalImg, "Original image");

	Mat greyImg;
        cvtColor(originalImg, greyImg, CV_BGR2GRAY);	//convert to greyscale
	showIm(greyImg, "Greyscale image");

	//Mat eqImg;
        //equalizeHist(greyImg, eqImg);	//equalize image
	//showIm(eqImg, "Equalised image");

	BilateralFilter bf(greyImg, FILTER_SIZE, S_SPACE, S_RANGE);
        bf.doFilter();	//filter with bilateral filter to smooth without blurring contours
        Mat blurImg = bf.getResult();
	showIm(blurImg, "Blurred image");

	Mat binaryImg; //apply threshold to get binary image
	adaptiveThreshold(blurImg, binaryImg, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE, ADAPTIVE_THRESH_WEIGHT);
	showIm(binaryImg, "Thresholded image");
	return binaryImg;
}



