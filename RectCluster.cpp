#include "RectCluster.h"

RectCluster::RectCluster(cv::Rect headRect, cv::Rect tailRect)
{
	xHead = (int)( headRect.x + headRect.width/2);
	yHead = (int)(headRect.y + headRect.height/2);
	dHead = headRect.height;

	xTail = (int)(tailRect.x + tailRect.width/2);
	yTail = (int)(tailRect.y + tailRect.height/2);
	dTail = tailRect.height;
};


void RectCluster::setHead(int x, int y, double d)
{
	xHead = x;
	yHead = y;
	dHead = d;
};


void RectCluster::setTail(int x, int y, double d)
{
	xTail = x;
	yTail = y;
	dTail = d;
};


cv::Rect RectCluster::getBoundingRect()
{
	int width = (int)(xTail + dTail - xHead + dHead);
	int height = (int)(std::max(yHead+dHead,yTail+dTail) - std::min(yHead - dHead, yTail - dTail));
	int xTopLeft = (int)(xHead-dHead);
	int yTopLeft = (int)(std::min(yHead-dHead,yTail-dTail));
	cv::Rect contourRect(xTopLeft, yTopLeft, width, height);
	return contourRect;
};


bool RectCluster::isPlate()
{
	cv::Rect contourRect = getBoundingRect();
	double ratio = (double)contourRect.height / contourRect.width;
	if (MIN_RATIO < ratio && ratio < MAX_RATIO)
		{return true;}
	else
		{return false;};

};
