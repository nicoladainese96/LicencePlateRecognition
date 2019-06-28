#ifndef PREPROCESSING_H
#define PREPROCESSING_H


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "filter.h"
#include "graphics.h"

using namespace cv;
using namespace std;

const int ADAPTIVE_THRESH_BLOCK_SIZE = 19;
const int ADAPTIVE_THRESH_WEIGHT = 9;
const int S_SPACE = 80;
const  int S_RANGE = 80;
const int FILTER_SIZE = 7;


Mat preprocessing(Mat & originalImg);

#endif
