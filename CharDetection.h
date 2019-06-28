#ifndef CHAR_DETECTION_H
#define CHAR_DETECTION_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "PossibleChar.h"
#include "graphics.h"

using namespace cv;
using namespace std;

// global constants ///////////////////////////////////////////////////////////////////////////////
// constants for checkIfPossibleChar, this checks one possible char only (does not compare to another char)
const int MIN_PIXEL_WIDTH = 2;
const int MIN_PIXEL_HEIGHT = 8;
const int MAX_PIXEL_HEIGHT = 75; //all these measures are not invariant under changes of resolution/pixel size -> work with standard resolution!
const double MIN_ASPECT_RATIO = 0.15;
const double MAX_ASPECT_RATIO = 1.0;
const int MIN_PIXEL_AREA = 80;

// canny edge detector variables
const int THRESH1 = 100;
const int THRESH2 = 200;
const int CANNY_FILTER_SIZE = 5;

vector<PossibleChar> detectChar(Mat & binaryImg, Mat drawing);

vector<PossibleChar> filterByHeightVar( vector<PossibleChar> & originalVec);

vector<PossibleChar> filterByAllignment( vector<PossibleChar> & originalVec);

bool checkIfPossibleChar(PossibleChar &possibleChar);

bool doIntersect(PossibleChar & possChar1, PossibleChar & possChar2);

#endif
