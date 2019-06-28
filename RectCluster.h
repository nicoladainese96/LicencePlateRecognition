#ifndef RECTCLUSTER_H
#define RECTCLUSTER_H

#include <opencv2/core.hpp>

const double MIN_RATIO = 0.2;
const double MAX_RATIO = 0.4;

class RectCluster {
public:
	int xHead;
	int yHead;
	double dHead;

	int xTail;
	int yTail;
	double dTail;

	RectCluster(cv::Rect headRect, cv::Rect tailRect);

	void setTail(int x, int y, double d);
	void setHead(int x, int y, double d);

	cv::Rect getBoundingRect();

	bool isPlate();
};

#endif
