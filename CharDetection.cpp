#include "CharDetection.h"
#include <vector>
#include <iostream>
#include <numeric>
#include <cmath>

vector<PossibleChar> detectChar(Mat & binaryImg, Mat drawing)
{
	Mat canny_output;
    	Canny(binaryImg, canny_output, THRESH1, THRESH2, CANNY_FILTER_SIZE );	// Canny edge deteection
	showIm(canny_output, "Canny edge detection");

    	vector<vector<Point> > contours;
    	vector<Vec4i> hierarchy;
    	findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE ); //contours from edges of binary image (connected components)
	cout << "Number of contours founded - STAGE1: " << contours.size() << endl;
	vector<PossibleChar> vecPossibleChar;
	vector<vector<Point> > possContours;
	vector<Vec4i> possHierarchy;

	for( size_t i=0; i< contours.size(); i++)
	{
		PossibleChar possChar(contours[i]);
		if (checkIfPossibleChar(possChar))
		{
		vecPossibleChar.push_back(possChar);
		possContours.push_back(contours[i]);
		possHierarchy.push_back(hierarchy[i]);
		};
	}

	cout << "Number of contours founded - STAGE2: " << vecPossibleChar.size() << endl;
	// filter out all char that intersect (erease the one with smaller area, treated as an impurity)
	bool loopCondition = true;
	while(loopCondition)
	{
		for(size_t i = 0; i < vecPossibleChar.size(); i++)
		{
			int countIntersections = 0;
			for(size_t j = i+1; j < vecPossibleChar.size(); j++)
			{
				if(doIntersect(vecPossibleChar[i],vecPossibleChar[j]))
				{
					//cout << "Intersection happened: (i,j) = " << i << ", " << j << endl;
					if(vecPossibleChar[i].area > vecPossibleChar[j].area)
					{
						vecPossibleChar.erase(vecPossibleChar.begin() + j);
						possContours.erase(possContours.begin() + j);
						possHierarchy.erase(possHierarchy.begin() + j);
						countIntersections += 1;
						break;
					}
					else
					{
						vecPossibleChar.erase(vecPossibleChar.begin() + i);
						possContours.erase(possContours.begin() + i);
						possHierarchy.erase(possHierarchy.begin() + i);
						countIntersections += 1;
						break;
					}
				}
			}
			if(countIntersections > 0)
			{
				break;
			}
			if(i == vecPossibleChar.size()-1)
			{
				loopCondition = false; // if it arrives at the end of the "i" cycle, no intersection are left
			}

		}
	}


	cout << "Number of contours founded - STAGE3: " << possContours.size() << endl;
	String windowTitle = "Canny contours";
	namedWindow(windowTitle, WINDOW_NORMAL);
	resizeWindow(windowTitle, 1080, 1920);
    	//Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	RNG rng(12345);
    	for( size_t i = 0; i< possContours.size(); i++ )
    	{
        	Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        	drawContours( drawing, possContours, (int)i, color, 1, LINE_4, possHierarchy, 0 );
    	}
    	imshow("Canny contours", drawing);

	waitKey(0);
	destroyWindow(windowTitle);

	return vecPossibleChar;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
vector<PossibleChar> filterByHeightVar( vector<PossibleChar> & originalVec)
{
	vector<double> heights;
	for (size_t i = 0 ; i < originalVec.size(); i++)
	{
		double h = (double)originalVec[i].boundingRect.height;
		heights.push_back(h);
	}

	int N = (int) heights.size();
	double mean = std::accumulate(heights.begin(), heights.end(), 0.0) / N;
	cout << "Mean height in pixels: " << mean << endl;
	double var = 0;
	for(int  i = 0; i < N; i++ )
	{
  		var += (heights[i] - mean) * (heights[i] - mean);
	}
	var /= N;
	double sd = std::sqrt(var);
	cout << "Standard deviation of height in pixels: " << sd << endl;

	vector<PossibleChar> filteredVec;
	for (size_t i=0; i < originalVec.size(); i++)
	{
		if (abs(heights[i] - mean) < 2*sd)
		{
			filteredVec.push_back(originalVec[i]);
		}
		else
		{
			continue;
		}
	}

	return filteredVec;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
vector<PossibleChar> filterByAllignment( vector<PossibleChar> & originalVec)
{
	vector<int> yCenters;
	for (size_t i = 0 ; i < originalVec.size(); i++)
	{
		int y = originalVec[i].intCenterY;
		yCenters.push_back(y);
	}

	int N = (int) yCenters.size();
	double mean = std::accumulate(yCenters.begin(), yCenters.end(), 0.0) / N;
	cout << "Mean yCenter in pixels: " << mean << endl;
	double var = 0;
	for(int  i = 0; i < N; i++ )
	{
  		var += (yCenters[i] - mean) * (yCenters[i] - mean);
	}
	var /= N;
	double sd = std::sqrt(var);
	cout << "Standard deviation of yCenter in pixels: " << sd << endl;

	vector<PossibleChar> filteredVec;
	for (size_t i=0; i < originalVec.size(); i++)
	{
		if (abs(yCenters[i] - mean) < 2*sd)
		{
			filteredVec.push_back(originalVec[i]);
		}
		else
		{
			continue;
		}
	}

	return filteredVec;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool checkIfPossibleChar(PossibleChar &possibleChar)
{
	// this function is a 'first pass' that does a rough check on a contour to see if it could be a char,
	// note that we are not (yet) comparing the char to other chars to look for a group
	if (possibleChar.boundingRect.area() > MIN_PIXEL_AREA && possibleChar.boundingRect.height < MAX_PIXEL_HEIGHT &&
	possibleChar.boundingRect.width > MIN_PIXEL_WIDTH && possibleChar.boundingRect.height > MIN_PIXEL_HEIGHT &&
        MIN_ASPECT_RATIO < possibleChar.dblAspectRatio && possibleChar.dblAspectRatio < MAX_ASPECT_RATIO)
	{
        	return(true);
	}
	else
	{
        	return(false);
	}
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool doIntersect(PossibleChar & possChar1, PossibleChar & possChar2)
{
	cv::Rect contourRect = possChar1.boundingRect; //keep fix the first
	cv::Rect charRect = possChar2.boundingRect; // check for the four vertices if there is one of them inside contourRect
	vector<Point> vertices;
	vertices.push_back(Point(charRect.x, charRect.y));
	vertices.push_back(Point(charRect.x+charRect.width, charRect.y));
	vertices.push_back(Point(charRect.x, charRect.y+charRect.height));
	vertices.push_back(Point(charRect.x+charRect.width, charRect.y+charRect.height));

	int counts = 0;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x > contourRect.x  && vertices[i].x < contourRect.x + contourRect.width &&
		    vertices[i].y > contourRect.y  && vertices[i].y < contourRect.y + contourRect.height)
		{
			counts += 1;
		}
		else
		{
			continue;
		}
	}
	if (counts > 0)
	{
		return true;
	}
	else
	{
		if(possChar2.intCenterX > contourRect.x && possChar2.intCenterX < contourRect.x + contourRect.width &&
		   possChar2.intCenterY > contourRect.y && possChar2.intCenterY < contourRect.y + contourRect.height)
		{
			return true;
		}
		else
		{
		return false;
		}
	}
}
