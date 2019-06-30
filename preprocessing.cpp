#include "preprocessing.h"

Mat preprocessing(Mat & originalImg)
{
	showIm(originalImg, "Original image");
	resize(originalImg, originalImg, Size(800,600));
	Mat greyImg;
        cvtColor(originalImg, greyImg, CV_BGR2GRAY);	//convert to greyscale
	//showIm(greyImg, "Greyscale image");

	//Mat eqImg;
        //equalizeHist(greyImg, eqImg);	//equalize image
	//showIm(eqImg, "Equalised image");

	BilateralFilter bf(greyImg, FILTER_SIZE, S_SPACE, S_RANGE);
        bf.doFilter();	//filter with bilateral filter to smooth without blurring contours
        Mat blurImg = bf.getResult();
	//showIm(blurImg, "Blurred image");
	imwrite("blurImg.jpg", blurImg);

	Mat binaryImg; //apply threshold to get binary image
	adaptiveThreshold(blurImg, binaryImg, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE, ADAPTIVE_THRESH_WEIGHT);
	//showIm(binaryImg, "Thresholded image");
	imwrite("binaryImg.jpg", binaryImg);

	Mat canny_output;
        Canny(binaryImg, canny_output, 100, 200, 5 );   // Canny edge deteection
        //showIm(canny_output, "Canny edge detection");
        imwrite("canny_output1.jpg",canny_output);

	return binaryImg;
}



