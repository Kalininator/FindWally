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
double getScoreSquaredDifference(Image* scene, Image* sample)
{
	double score = 0;
	
	for (int i = 0; i < sample->width; i++)
	{
		for (int j = 0; j < sample->height; j++)
		{
			double sc = abs(scene->getValue(i,j) - sample->getValue(i,j));
			score += sc;
		}
	}

	return score;
}


double getScore(Image* scene, Image* sample, float mean_sample)
{
	double score;
	//calc mean of scene
	float mean_scene = scene->getTotal() / (scene->width * scene->height);

	Image* I = *scene - mean_scene;
	//std::cout << scene->getTotal() << std::endl << scene->width << std::endl<< scene->height;
	Image* T = *sample - mean_sample;

	Image* IT = *I * *T;
	Image* II = *I * *I;
	Image* TT = *T * *T;
	//std::cout << IT->getTotal();
	//IT->printMatrix();
	double top = IT->getTotal();
	double bottom = (double)II->getTotal() * (double)TT->getTotal();
	double sqrtbottom = sqrt(bottom);
	//std::cout << IT->getTotal();
	score = (top/sqrtbottom);
	//std::cout << II->getTotal() * TT->getTotal();
	

	delete I;
	delete T;
	delete II;
	delete TT;
	delete IT;

	return score;
}



//void findBest(Image* scene, Image* wally) //395798
//{
//	clock_t startTime = clock();
//	double best = -1;
//	int bestx = 0;
//	int besty = 0;
//	float mean_sample = wally->getTotal() / (wally->width * wally->height);
//	std::mutex coutMu;
//	#pragma omp parallel for //96 seconds with
//	for (int i = 0; i < scene->width - wally->width; i++)
//	{
//		for (int j = 0; j < scene->height - wally->height; j++)
//		{
//			Image* scenesection = scene->getSection(i, j, 36, 49);
//			double sc = getScore(scenesection, wally,mean_sample);
//			if (sc > best)
//			{
//				best = sc;
//				bestx = i;
//				besty = j;
//				std::lock_guard<std::mutex> lock(coutMu);
//				std::cout << "score: " << sc << ",x:" << i << ",y:" << j << std::endl;
//			}
//			delete scenesection;
//		}
//	}
//	std::cout << "Time Taken: " << float(clock() - startTime)/(double)CLOCKS_PER_SEC << std::endl;
//
//	Image* result = scene->getSection(bestx,besty,wally->width,wally->height);
//	result->generatePGM("result.pgm");
//	delete result;
//
//	std::cout << "best match can be found in result.pgm";
//}


void find_Best(Image* scene, Image* wally)
{

}


void findBest(Image* scene, Image* wally)
{
	clock_t startTime = clock();
	double best = -1;
	float mean_sample = wally->getTotal() / (wally->width * wally->height);
	for (int i = 0; i < scene->width - wally->width; i++)
	{
		std::vector<std::future<double>> futures;
		std::vector<Image*> sections;
		for (int j = 0; j < scene->height - wally->height; j++)
		{
			sections.push_back(scene->getSection(i,j,wally->width,wally->height));
			futures.push_back(std::async(getScore,sections[j],wally,mean_sample));
		}

		for (int j = 0; j < scene->height - wally->height; j++)
		{
			double sc = futures[j].get();
			if (sc > best)
			{
				best = sc;
				std::cout << "score: " << sc << ",x:" << i << ",y:" << j << std::endl;
			}
			delete sections[j];
		}
	}
	std::cout << "Time Taken: " << float(clock() - startTime) / (double)CLOCKS_PER_SEC << std::endl;
}


void findBestSquaredDifference(Image* scene, Image* wally)
{
	clock_t startTime = clock();
	double best = 10000000000;
	int bestx = 0;
	int besty = 0;
	//float mean_sample = wally->getTotal() / (wally->width * wally->height);
	std::mutex coutMu;
#pragma omp parallel for //96 seconds with
	for (int i = 0; i < scene->width - wally->width; i++)
	{
		for (int j = 0; j < scene->height - wally->height; j++)
		{
			Image* scenesection = scene->getSection(i, j, 36, 49);
			double sc = getScoreSquaredDifference(scenesection, wally);
			sc = sc * sc;
			if (sc < best)
			{
				best = sc;
				bestx = i;
				besty = j;
				std::lock_guard<std::mutex> lock(coutMu);
				std::cout << "score: " << sc << ",x:" << i << ",y:" << j << std::endl;
			}
			delete scenesection;
		}
	}
	std::cout << "Time Taken: " << float(clock() - startTime) / (double) CLOCKS_PER_SEC << std::endl;

	Image* result = scene->getSection(bestx, besty, wally->width, wally->height);
	result->generatePGM("result.pgm");
	delete result;

	std::cout << "best match can be found in result.pgm";
}

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
	scene->NNS_NormalisedCorrelation(templateImage);
	std::cout << "Time Taken: " << float(clock() - startTime) / (double)CLOCKS_PER_SEC << std::endl;
	delete templateImage;
	delete scene;

	system("pause");
	return 0;
}
