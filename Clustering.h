#ifndef CLUSTERING_H
#define CLUSTERING_H


#include<opencv2/core/core.hpp>

#include "PossibleChar.h"
#include "RectCluster.h"
#include "graphics.h"

using namespace std;
using namespace cv;

const double ALPHA = 0.85; // sum of radii * alpha > distance of centers => merge clusters
const int MIN_COMPONENTS = 6; // min number of characters in a licence plate

std::vector<Rect> selectPlates(vector<PossibleChar> & vecPossibleChar, cv::Mat drawing);

std::vector<RectCluster> sortVecOfCluster(std::vector<RectCluster>& originalVec);

std::vector<PossibleChar> sortVecOfPossibleChar(std::vector<PossibleChar>& originalVec);

std::vector<RectCluster> mergeCluster(std::vector<RectCluster> VecOfCluster);

bool isInFrontOf(RectCluster& firstCluster, RectCluster& secondCluster);

double computeDistance(RectCluster& tailCluster, RectCluster& headCluster);

bool isCharInCluster(PossibleChar& possChar, RectCluster& cluster);

#endif
