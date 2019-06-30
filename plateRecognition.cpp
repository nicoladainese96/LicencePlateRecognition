#include "plateRecognition.h"
#include <iostream>

Mat preprocessPlate(Mat & plateImg)
{
	Mat refinedImg = plateImg.clone();
        cvtColor(refinedImg, refinedImg, CV_BGR2GRAY);

	adaptiveThreshold(refinedImg, refinedImg, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE1, ADAPTIVE_THRESH_WEIGHT1);
	showIm(refinedImg, "Thresholded image");

	return refinedImg;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
vector<Mat> cropROIs(Mat & plateImg, vector<PossibleChar> & charsAlligned)
{
	vector<Mat> charImgs;
	//Rect targetRect(0,0,32,32); // 32x32 pixel output
	for (size_t i = 0; i < charsAlligned.size(); i++)
	{
		Mat croppedImg = plateImg(charsAlligned[i].boundingRect);
		Mat squareCharImg = GetSquareImage(croppedImg);
		//Mat resizedCharImg ;
		//resize(squareCharImg, resizedCharImg, targetRect);
		charImgs.push_back(squareCharImg);
	}
	return charImgs;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void recognizePlate(Mat & originalImg, Rect & roi)
{
	Mat plateImg = originalImg(roi);
	//showIm(plateImg,"Zoom on plate");

	Mat refinedImg = preprocessPlate(plateImg);
	Mat plateDrawing = Mat::zeros(refinedImg.size(), CV_8UC3 );

	vector<PossibleChar> charsInPlate = detectChar(refinedImg, plateDrawing);
	vector<PossibleChar> charsFilteredByHeight = filterByHeightVar(charsInPlate);
	vector<PossibleChar> charsAlligned = filterByAllignment(charsFilteredByHeight);
	cout << "Characters detected in plate: " << charsAlligned.size() << endl;
	drawPossibleChar(plateImg, charsAlligned, "Characters detected in plate", 2);

	// reorder this with increasing x
	vector<PossibleChar> charsSorted = sortVecOfPossibleChar(charsAlligned);

	Mat inversThreshImg;
	adaptiveThreshold(refinedImg, inversThreshImg, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 7,0);
	vector<Mat> charImgs = cropROIs(inversThreshImg, charsSorted);

	for (size_t i = 0; i < charImgs.size(); i++)
	{
		//showIm(charImgs[i],"Character resized");
		ostringstream name;
		name << "ImChar" << i << ".png";
		imwrite(name.str(), charImgs[i]);

		string command;
		command = "python -c 'from CNN_PredictChar import predict; predict(\""+ name.str()+ "\")' >> result.txt";
		cout << "Command executed: " << command << endl;
		const char *command2 = command.c_str();
		system(command2);
	}

}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Mat GetSquareImage( Mat& img)
{

	Mat paddedImg = padImage(img);

	int width = paddedImg.cols;
	int height = paddedImg.rows;
	int target_width = 32;

 	int max_dim = ( width >= height ) ? width : height;
	Mat square = Mat::ones( target_width, target_width, paddedImg.type() );
	square = Scalar::all(255);

	float scale = ( ( float ) target_width ) / max_dim;
	Rect roi;
	if ( width >= height )
	{
        	roi.width = target_width;
        	roi.x = 0;
        	roi.height = height * scale;
        	roi.y = ( target_width - roi.height ) / 2;
	}
	else
	{
        	roi.y = 0;
        	roi.height = target_width;
        	roi.width = width * scale;
        	roi.x = ( target_width - roi.width ) / 2;
	}

	resize( paddedImg, square( roi ), roi.size() );

	return square;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Mat padImage(Mat & src)
{
	Mat dst;
	copyMakeBorder( src, dst, 2, 2, 2, 2, BORDER_CONSTANT, 255 ); //255 for white border
	return dst;
}
