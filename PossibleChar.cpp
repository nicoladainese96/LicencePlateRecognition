#include "PossibleChar.h"

PossibleChar::PossibleChar(std::vector<cv::Point> _contour)
{
contour = _contour;

boundingRect = cv::boundingRect(contour);

intCenterX = (boundingRect.x + boundingRect.x + boundingRect.width) / 2;
intCenterY = (boundingRect.y + boundingRect.y + boundingRect.height) / 2;

area = boundingRect.width * boundingRect.height;

dblAspectRatio = (float)boundingRect.width / (float)boundingRect.height;
};

