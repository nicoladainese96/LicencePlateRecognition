#ifndef PLATE_SEGMENTATION_H
#define PLATE_SEGMENTATION_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "graphics.h"
#include "PossibleChar.h"
#include "CharDetection.h"
#include "Clustering.h"


using namespace std;
using namespace cv;

const int ALMOST_BLACK = 200; //euclidean distance from (0,0,0) under which we consider a color almost black
const int ADAPTIVE_THRESH_BLOCK_SIZE1 = 19;
const int ADAPTIVE_THRESH_WEIGHT1 = 10;

Mat preprocessPlate(Mat & plateImg);

vector<Mat> cropROIs(Mat & plateImg, vector<PossibleChar> & charsAlligned);

void recognizePlate(Mat & originalImg, Rect & roi);

Mat GetSquareImage( Mat& img);

Mat padImage(Mat & src);

#endif
