#include <iostream>
#include <cmath>
#include "Image.h"


double getScore(Image* scene, Image* sample)
{
	double score;
	//calc mean of scene and sample
	float mean_scene = scene->getTotal() / (scene->width * scene->height);
	float mean_sample = sample->getTotal() / (sample->width * sample->height);

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


int main()
{
	//1024,768
	//36,49

	Image* scene = new Image(1024, 768);
	Image* wally = new Image(36, 49);
	scene->fillFromFile("Cluttered_scene.txt");
	wally->fillFromFile("Wally_grey.txt");
	
	double best = -1;
	int bestx, besty;
	//for (int i = 0; i < scene->width - wally->width; i++)
	//{
	//	for (int j = 0; j < scene->height - wally->height; j++)
	//	{
	//		Image* scenesection = scene->getSection(i,j, 36, 49);
	//		double sc = getScore(scenesection, wally);
	//		if (sc > best)
	//		{
	//			best = sc;
	//			bestx = i;
	//			besty = j;
	//			std::cout << "score: " << sc << ",x:" << i << ",y:" << j << std::endl;
	//		}
	//		//scenesection->printMatrix();
	//		//std::cout << (*wally * *wally)->getTotal();

	//		//scenesection->printMatrix();

	//		delete scenesection;
	//		
	//	}
	//}

	/*Image* foundhim = scene->getSection(162,144, 36, 49);
	foundhim->generatePGM("foundhim.pgm");
	delete foundhim;*/

	std::cout << scene->getValue(2000, 2000);

	delete scene;
	delete wally;

	

	system("pause");
	return 0;
}