#ifndef POSSIBLE_CHAR_H
#define POSSIBLE_CHAR_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class PossibleChar {
public:
	// member variables
	std::vector<cv::Point> contour;

	cv::Rect boundingRect;

	int intCenterX;
	int intCenterY;

	int area;

	double dblAspectRatio;

	PossibleChar(std::vector<cv::Point> _contour);

};

#endif // POSSIBLE_CHAR_H
