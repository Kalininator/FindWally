#include <iostream>
#include <cmath>
#include "Image.h"
#include "LargeImage.h"
#include "MatchImage.h"
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <cstring>




void displayUsageMessage()
{
	std::cout << "[-s] -> use sum of squared differences";
	exit(0);
}


int main(int argc, char* argv[])
{
	bool squaredDifference = false;
	std::string sceneFile = "Cluttered_scene.txt";
	std::string templateFile = "Wally_grey.txt";

	//if (argc == 2)//get any command-line arguments
	//{
	//	if(std::strcmp(argv[1],"-s")==0)
	//		squaredDifference = true;	
	//	if(std::strcmp(argv[1],"-h")==0)
	//		displayUsageMessage();
	//}

	////1024,768
	////36,49

	//Image* scene = new Image(1024, 768, sceneFile);
	//Image* wally = new Image(36, 49, templateFile);
	////scene->fillFromFile("Cluttered_scene.txt");
	////wally->fillFromFile("Wally_grey.txt");

	//if(squaredDifference)
	//	findBestSquaredDifference(scene, wally);
	//else
	//	findBest(scene, wally);
	//
	//


	//delete scene;
	//delete wally;

	LargeImage * scene = new LargeImage(1024, 768, sceneFile);
	Image* templateImage = new Image(36, 49, templateFile);
	clock_t startTime = clock();

	/*MatchImage* actual = new MatchImage(scene, 162, 144, templateImage);
	std::cout << actual->getScoreSquaredDifference();*/
	scene->NNS_NormalisedCorrelation(templateImage,30);
	//scene->NNS_SquaredDifference(templateImage,20);



	std::cout << "Time Taken: " << float(clock() - startTime) / (double)CLOCKS_PER_SEC << std::endl;
	delete templateImage;
	delete scene;
	std::cin.get();
	return 0;
}
