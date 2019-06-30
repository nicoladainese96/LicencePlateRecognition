#include <iostream>
#include "Clustering.h"

vector<Rect> selectPlates(vector<PossibleChar> & vecPossibleChar, Mat drawing)
{
	vector<RectCluster> vecRectCluster;
	for (size_t i = 0; i < vecPossibleChar.size(); i++)
	{
		RectCluster rc(vecPossibleChar[i].boundingRect, vecPossibleChar[i].boundingRect);
		vecRectCluster.push_back(rc);
	};
	//cout << "vecRectCluster done - size: " << vecRectCluster.size() << endl;

	String windowTitle0 = "Canny contours with point clusters";
	drawRectsCluster(drawing, vecRectCluster, windowTitle0,1);

	vector<RectCluster> sortedVecCluster = sortVecOfCluster(vecRectCluster);
	//cout << "sortedVecCluster done - size: " << sortedVecCluster.size() << endl;

	vector<RectCluster> mergedVecCluster = mergeCluster(sortedVecCluster);
	//cout << "merdedVecCluster done - size:" << mergedVecCluster.size() << endl;

	String windowTitle1 = "Canny contours with plate-like clusters";
	drawRectsCluster(drawing, mergedVecCluster, windowTitle1,1);

	vector<RectCluster> platelikeVecCluster;
	for (size_t i = 0; i < mergedVecCluster.size(); i++)
	{
		if (mergedVecCluster[i].isPlate())
		{
			platelikeVecCluster.push_back(mergedVecCluster[i]);
		}
	}
	//cout << "platelikeVecCluster done - size: "<< platelikeVecCluster.size() << endl;

	vector<int> membersOfCluster;
	for (size_t i = 0; i < platelikeVecCluster.size(); i++)
	{
		int members = 0;
		for (size_t j = 0; j < vecPossibleChar.size(); j++)
		{
			if (isCharInCluster(vecPossibleChar[j], platelikeVecCluster[i]))
			{
				members += 1;
			}
		}
		membersOfCluster.push_back(members);
	}
	//cout << "membersOfCluster done" << endl;

	vector<Rect> selectedPlates;
	for (size_t i = 0; i < platelikeVecCluster.size(); i++)
	{
		if (membersOfCluster[i] >= MIN_COMPONENTS)
		{
			Rect contourRect = platelikeVecCluster[i].getBoundingRect();
			selectedPlates.push_back(contourRect);
		}
	}

	//cout << "Number of possible plates selected: " << selectedPlates.size() << endl;

	String windowTitle2 = "Canny contours with plates highlighted";
	drawRects(drawing, selectedPlates, windowTitle2,1);

	return selectedPlates;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
std::vector<RectCluster> sortVecOfCluster(std::vector<RectCluster>& originalVec)
{
	std::vector<RectCluster> sortedVec;
	int maxSortedX = 0;

	for(size_t i = 0; i < originalVec.size(); i++)
	{
		//cout << "Cycle " << i << "of sortVecOfCluster " << std::endl;
		if (sortedVec.size() == 0)
		{
			sortedVec.push_back(originalVec[i]);
			maxSortedX = originalVec[i].xHead;
		 }
		else if (originalVec[i].xHead > maxSortedX )
		{
			sortedVec.push_back(originalVec[i]);
			maxSortedX = originalVec[i].xHead;
		 }
		else
		{
			for (size_t j = 0; j < sortedVec.size(); j++ )
			{
				//cout << "debug here" << std::endl;
				if (sortedVec[j].xHead > originalVec[i].xHead)
				{
					sortedVec.insert(sortedVec.begin()+j, originalVec[i]);
					break;
				}
			}
		}
	}
	return sortedVec;
};

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
std::vector<PossibleChar> sortVecOfPossibleChar(std::vector<PossibleChar>& originalVec)
{
	std::vector<PossibleChar> sortedVec;
	int maxSortedX = 0;

	for(size_t i = 0; i < originalVec.size(); i++)
	{
		//cout << "Cycle " << i << "of sortVecOfCluster " << std::endl;
		if (sortedVec.size() == 0)
		{
			sortedVec.push_back(originalVec[i]);
			maxSortedX = originalVec[i].intCenterX;
		 }
		else if (originalVec[i].intCenterX > maxSortedX )
		{
			sortedVec.push_back(originalVec[i]);
			maxSortedX = originalVec[i].intCenterX;
		 }
		else
		{
			for (size_t j = 0; j < sortedVec.size(); j++ )
			{
				if (sortedVec[j].intCenterX > originalVec[i].intCenterX)
				{
					sortedVec.insert(sortedVec.begin()+j, originalVec[i]);
					break;
				}
			}
		}
	}
	return sortedVec;
};

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
std::vector<RectCluster> mergeCluster(std::vector<RectCluster> VecOfCluster)
{
	bool loopCondition = true;
	while(loopCondition)
	{
		for (size_t i = 0; i < VecOfCluster.size(); i++)
		{
			std::vector<double> distVec;
			for (size_t j = i+1; j < VecOfCluster.size(); j++)
			{
				double d = computeDistance(VecOfCluster[i], VecOfCluster[j]);
				distVec.push_back(d);
			}

			int j_star = std::min_element(distVec.begin(), distVec.end()) - distVec.begin() + i + 1;
			bool possibleToMerge = isInFrontOf(VecOfCluster[i], VecOfCluster[j_star]);
			if (possibleToMerge)
			{
				RectCluster vecToMerge = VecOfCluster[i];
				VecOfCluster[j_star].setHead(vecToMerge.xHead, vecToMerge.yHead, std::max(vecToMerge.dHead,VecOfCluster[i].dTail));
				VecOfCluster.erase(VecOfCluster.begin() + i);
				break;
			}

			if (i == VecOfCluster.size()-1)
			{
				loopCondition = false; //exit if all the for cycle in "i" is completed without merging
			}
		}

	}
	return VecOfCluster;
};

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool isInFrontOf(RectCluster& firstCluster, RectCluster& secondCluster)
{
	double headTailDistance = computeDistance(firstCluster,secondCluster);
	double sumOfDimensions = firstCluster.dTail + secondCluster.dHead;
	if (sumOfDimensions*ALPHA > headTailDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
};

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double computeDistance(RectCluster& tailCluster, RectCluster& headCluster)
{
	double d = std::sqrt(std::pow(headCluster.xHead-tailCluster.xTail,2.) + std::pow(headCluster.yHead-tailCluster.yTail,2.));
	return d;
};

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool isCharInCluster(PossibleChar& possChar, RectCluster& cluster)
{
	cv::Rect contourRect = cluster.getBoundingRect();
	cv::Rect charRect = possChar.boundingRect;
	if (charRect.x > contourRect.x  && charRect.x + charRect.width < contourRect.x + contourRect.width &&
	    charRect.y > contourRect.y  && charRect.y + charRect.height < contourRect.y + contourRect.height)
	{
		return true;
	}
	else
	{
		return false;
	}
};
