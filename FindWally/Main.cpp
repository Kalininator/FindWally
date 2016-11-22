#include <iostream>
#include <cmath>
#include "Image.h"
#include <thread>
#include <vector>
#include <mutex>

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
	//calc mean of scene and sample
	float mean_scene = scene->getTotal() / (scene->width * scene->height);

	Image* I = *scene + -mean_scene;
	//std::cout << scene->getTotal() << std::endl << scene->width << std::endl<< scene->height;
	Image* T = *sample + -mean_sample;

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



void findBest(Image* scene, Image* wally) //395798
{
	clock_t startTime = clock();
	double best = -1;
	int bestx = 0;
	int besty = 0;
	float mean_sample = wally->getTotal() / (wally->width * wally->height);
	#pragma omp parallel for //96 seconds with
	for (int i = 0; i < scene->width - wally->width; i++)
	{
		for (int j = 0; j < scene->height - wally->height; j++)
		{
			Image* scenesection = scene->getSection(i, j, 36, 49);
			double sc = getScore(scenesection, wally,mean_sample);
			if (sc > best)
			{
				best = sc;
				bestx = i;
				besty = j;
				std::cout << "score: " << sc << ",x:" << i << ",y:" << j << std::endl;
			}
			delete scenesection;
		}
	}
	std::cout << "Time Taken: " << float(clock() - startTime)/1000;

	Image* result = scene->getSection(bestx,besty,wally->width,wally->height);
	result->generatePGM("result.pgm");
	delete result;

	std::cout << "best match can be found in result.pgm";
}

void findBestSquaredDifference(Image* scene, Image* wally)
{
	clock_t startTime = clock();
	double best = 10000000000;
	int bestx = 0;
	int besty = 0;
	//float mean_sample = wally->getTotal() / (wally->width * wally->height);
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
				std::cout << "score: " << sc << ",x:" << i << ",y:" << j << std::endl;
			}
			delete scenesection;
		}
	}
	std::cout << "Time Taken: " << float(clock() - startTime) / 1000;

	Image* result = scene->getSection(bestx, besty, wally->width, wally->height);
	result->generatePGM("result.pgm");
	delete result;

	std::cout << "best match can be found in result.pgm";
}



int main(int argc, char* argv[])
{
	bool squaredDifference = false;
	

	if (argc > 1)//get any command-line arguments
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i] == "-s") //squared difference mode
				squaredDifference = true;
		}
	}

	//1024,768
	//36,49

	Image* scene = new Image(1024, 768);
	Image* wally = new Image(36, 49);
	scene->fillFromFile("Cluttered_scene.txt");
	wally->fillFromFile("Wally_grey.txt");

	if(squaredDifference)
		findBestSquaredDifference(scene, wally);
	else
		findBest(scene, wally);
	
	


	delete scene;
	delete wally;

	

	system("pause");
	return 0;
}
