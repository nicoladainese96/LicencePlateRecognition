#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filter.h"

using namespace cv;

// constructor
Filter::Filter(Mat input_img, int size) {

		input_image = input_img;
		if (size % 2 == 0)
			size++;
		filter_size = size;
		}

// for base class do nothing (in derived classes it performs the corresponding filter)
void Filter::doFilter() {

		// it just returns a copy of the input image
		result_image = input_image.clone();

		}

// get output of the filter
Mat Filter::getResult() {

		return result_image;
		}

//set window size (it needs to be odd)
void Filter::setSize(int size) {

		if (size % 2 == 0)
			size++;
		filter_size = size;
		}

//get window size 
int Filter::getSize() {

		return filter_size;
		}



	// Write your code to implement the Gaussian, median and bilateral filters

// implement GaussianFilter constructor
GaussianFilter::GaussianFilter(Mat input_img, int size, double sigma):
	Filter(input_img, size)
	{
	std = sigma;
	}

// rewrite doFilter() for GaussianFilter
void GaussianFilter::doFilter(){
	GaussianBlur(input_image, result_image, Size(filter_size, filter_size), std, std); //uses only class attributes
	}

// implement MedianFilter
MedianFilter::MedianFilter(Mat input_img, int size):
	Filter(input_img, size){}
// medianBlur(in,out,size)
void MedianFilter::doFilter(){
	medianBlur(input_image, result_image, filter_size);
	}

// implement BilateralFilter
BilateralFilter::BilateralFilter(Mat input_img, int size, double s_space, double s_range):
	Filter(input_img, size)
	{
	sigma_space = s_space;
	sigma_range = s_range;
	}

void BilateralFilter::doFilter(){
	bilateralFilter(input_image, result_image, filter_size, sigma_range, sigma_space);
	}
