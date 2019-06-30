#include "main.h"

// take as argument the filename with its path, e.g. /home/workdir/final_project/data/DSCN0408.jpg
int main(int argc, char* argv[]) {

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// Import image and preprocessing

	String filename = argv[1];
	cout << "Reading image " << filename << endl;
	Mat originalImg = imread(filename);

	cout << endl << "Preprocessing image... " << endl;
	Mat binaryImg = preprocessing(originalImg);

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// Plate detection - Canny edge detector + filter possible characters proportions

	cout << endl << "Plate detection... " << endl;
	Mat drawing = Mat::zeros(binaryImg.size(), CV_8UC3 );
	vector<PossibleChar> vecPossibleChar;
	vecPossibleChar = detectChar(binaryImg, drawing);

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// Plate detection - clustering characters + possible plates selection

	vector<Rect> selectedPlates;
	selectedPlates = selectPlates(vecPossibleChar, drawing);

	String title0 = "Original image with contoured plate";
	drawRects(originalImg, selectedPlates, title0, 3); // last variable is the thickness of the rectangle borders

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// Plate recognition (if only 1 plate is recognized) - preprocessing + CNN prediction
	if (selectedPlates.size()==1)
	{
		cout << endl << "Plate recognition... " << endl;
		Rect roi(selectedPlates[0]);
		recognizePlate(originalImg, roi);
		string plateID = readResult(); // reading in the results of CNN prediction
		cout << "License Plate ID: " << plateID << endl;
		putText(originalImg, plateID, Point(roi.x ,roi.y ), FONT_HERSHEY_COMPLEX_SMALL, 1.5,Scalar(0,0,255), 1, CV_AA); // text, coordinates, scale, BGR color, line thickness, Anti-alias
		showIm(originalImg, "Plate recognized");
		}
	else
	{
		return 0;
	}

	return 0;
	}




string readResult()
{
	ifstream inFile;
	string plateID;
	string x;
	inFile.open("result.txt");

	if (!inFile)
	{
		cout << "Unable to open file";
		exit(1); // terminate with error
	}

	while (inFile >> x)
	{
		plateID = plateID + x;
	}
    	inFile.close();

	system("rm result.txt");

	return plateID;
}
