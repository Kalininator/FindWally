#include <iostream>
#include "Image.h"
#include "LargeImage.h"
#include "MatchImage.h"
#include <cstring>

using namespace std;
int main(int argc, char* argv[])
{
	bool normalisedCorrelation = false;
	string sceneFile = "Cluttered_scene.txt";
	string templateFile = "Wally_grey.txt";
	////1024,768
	////36,49
	LargeImage * scene = new LargeImage(1024, 768, sceneFile);
	Image* templateImage = new Image(36, 49, templateFile);
	clock_t startTime = clock();

	int topx = 5;
	if(normalisedCorrelation)
		scene->NNS_NormalisedCorrelation(templateImage,topx);
	else
		scene->NNS_SquaredDifference(templateImage,topx);

	cout << "Time Taken: " << float(clock() - startTime) / (double)CLOCKS_PER_SEC << endl;
	delete templateImage;
	delete scene;
	cin.get();
	return 0;
}
