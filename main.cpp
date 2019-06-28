#include "main.h"

// take as argument the filename with its path, e.g. /home/workdir/final_project/data/DSCN0408.jpg
int main(int argc, char* argv[]) {

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// Import image and preprocessing

	String filename = argv[1];
	cout << "Reading image " << filename << endl;
	Mat originalImg = imread(filename);

	Mat binaryImg = preprocessing(originalImg);

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// Plate detection - Canny edge detector + filter possible characters proportions

	Mat drawing = Mat::zeros(binaryImg.size(), CV_8UC3 );
	vector<PossibleChar> vecPossibleChar;
	vecPossibleChar = detectChar(binaryImg, drawing);

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// Plate detection - clustering characters + possible plates selection

	vector<Rect> selectedPlates;
	selectedPlates = selectPlates(vecPossibleChar, drawing);

	String title0 = "Original image with contoured plate";
	drawRects(originalImg, selectedPlates, title0, 3); // last variable is the thickness of the rectangle borders

	if (selectedPlates.size()==1)
	{
		Rect roi(selectedPlates[0]);
		Mat plateImg = originalImg(roi);
		showIm(plateImg,"Zoom on plate");
		Mat refinedImg = preprocessPlate(plateImg);
		Mat plateDrawing = Mat::zeros(refinedImg.size(), CV_8UC3 );
		vector<PossibleChar> charsInPlate = detectChar(refinedImg, plateDrawing);
		cout << "Possible chars in plate - STAGE1: " << charsInPlate.size() << endl;
		vector<PossibleChar> charsFilteredByHeight = filterByHeightVar(charsInPlate);
		cout << "Possible chars in plate - STAGE2: " << charsFilteredByHeight.size() << endl;
		vector<PossibleChar> charsAlligned = filterByAllignment(charsFilteredByHeight);
		cout << "Possible chars in plate - STAGE3: " << charsAlligned.size() << endl;
		drawPossibleChar(plateImg, charsAlligned, "Characters detected in plate", 2);
	}
	else
	{
		return 0;
	}

	return 0;
	}




