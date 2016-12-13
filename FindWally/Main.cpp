#include <iostream>
#include "Image.h"
#include "LargeImage.h"
#include "MatchImage.h"
#include <cstring>

int main(int argc, char* argv[])
{
	bool squaredDifference = false;
	std::string sceneFile = "Cluttered_scene.txt";
	std::string templateFile = "Wally_grey.txt";
	////1024,768
	////36,49
	//if(squaredDifference)
	//	findBestSquaredDifference(scene, wally);
	//else
	//	findBest(scene, wally);

	LargeImage * scene = new LargeImage(1024, 768, sceneFile);
	Image* templateImage = new Image(36, 49, templateFile);
	clock_t startTime = clock();

	int topx = 10;
	bool NNS = false;
	if(NNS)
		scene->NNS_NormalisedCorrelation(templateImage,topx);
	else
		scene->NNS_SquaredDifference(templateImage,topx);

	std::cout << "Time Taken: " << float(clock() - startTime) / (double)CLOCKS_PER_SEC << std::endl;
	delete templateImage;
	delete scene;
	std::cin.get();
	return 0;
}
