#include "graphics.h"

void showIm(Mat & im, String title)
{
	namedWindow(title,WINDOW_NORMAL);
        resizeWindow(title, 1080, 1920);
        imshow(title, im );
        waitKey(0);
        destroyWindow(title);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void drawRects(Mat & image, vector<Rect> & vecRect, String windowTitle, int thickness)
{
	Mat drawing = image.clone();
	namedWindow(windowTitle, WINDOW_NORMAL);
	resizeWindow(windowTitle, 1080, 1920);
	RNG rng(12345);
	for (size_t i = 0; i < vecRect.size(); i++)
	{
		Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
		rectangle(drawing, vecRect[i], color, thickness);
	}

	imshow(windowTitle, drawing);

	waitKey(0);
	destroyWindow(windowTitle);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void drawRectsCluster(Mat & image, vector<RectCluster> &  vecRectCluster, String windowTitle, int thickness)
{
	Mat drawing = image.clone();
	namedWindow(windowTitle, WINDOW_NORMAL);
	resizeWindow(windowTitle, 1080, 1920);
	RNG rng(12345);
	for (size_t i = 0; i < vecRectCluster.size(); i++)
	{
		Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
		rectangle(drawing, vecRectCluster[i].getBoundingRect(), color, thickness);
	}

	imshow(windowTitle, drawing);

	waitKey(0);
	destroyWindow(windowTitle);
}


//
void drawPossibleChar(Mat & image, vector<PossibleChar> &  vecPossibleChar, String windowTitle, int thickness)
{
	Mat drawing = image.clone();
	namedWindow(windowTitle, WINDOW_NORMAL);
	resizeWindow(windowTitle, 1080, 1920);
	RNG rng(12345);
	for (size_t i = 0; i < vecPossibleChar.size(); i++)
	{
		Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
		rectangle(drawing, vecPossibleChar[i].boundingRect, color, thickness);
	}

	imshow(windowTitle, drawing);

	waitKey(0);
	destroyWindow(windowTitle);
}
