#ifndef GRAPHICS_H
#define GRAPHICS_H


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "RectCluster.h"
#include "PossibleChar.h"

using namespace cv;
using namespace std;

void showIm(Mat & im, String title = "Display window");

void drawRects(Mat & image, vector<Rect> & vecRect, String windowTitle, int thickness = 1);

void drawRectsCluster(Mat & image, vector<RectCluster> & vecRectCluster, String windowTitle, int thickness = 1);

void drawPossibleChar(Mat & image, vector<PossibleChar> & vecPossibleChar, String windowTitle, int thickness = 1);

#endif
