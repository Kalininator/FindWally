#include "LargeImage.h"
#include "MatchImage.h"
#include <vector>
#include <iostream>
#include <mutex>
#include <thread>
#include <algorithm>

using namespace std;

void LargeImage::NNS_NormalisedCorrelation(Image * templateImage, int keepTop)
{
	mutex coutMu;
	auto worker = [](int start, int end, LargeImage* image, Image* templateImage, mutex& coutMu, int keepTop) {
		
		for (int i = start; i < end; i++)
		{
			int x = i % (image->width - templateImage->width);//calc x and y co-ords from index
			int y = i / (image->width - templateImage->width);


			MatchImage* match = new MatchImage(image,x,y,templateImage);//create a match image for the location

			match->getScoreNormalisedCorrelation();
			lock_guard<mutex> lock(coutMu);
			image->addMatchNNS(match,keepTop);

		}
	};

	int threadCount = thread::hardware_concurrency();
	//int threadCount = 7;
	vector<thread> threads(threadCount);
	const int grainsize = ((width - templateImage->width) * (height - templateImage->height)) / threadCount;//operations given to each thread

	int worker_iter = 0;

	for (int i = 0; i < threadCount - 1; i++)//all threads apart from last one
	{
		threads[i] = thread(worker,worker_iter,worker_iter + grainsize, this,templateImage, std::ref(coutMu), keepTop);
		worker_iter += grainsize;
	}
	threads.back() = thread(worker, worker_iter, (width - templateImage->width) * (height - templateImage->height), this,templateImage, std::ref(coutMu),keepTop);//last grainsize section, + any leftovers

	for (auto&& i : threads)
	{
		i.join();
	}

	//sort matches
	sort(matches.begin(), matches.end(), [](MatchImage* lhs, MatchImage* rhs) {return lhs->getScoreNormalisedCorrelation() > rhs->getScoreNormalisedCorrelation(); });
	//print em
	for (int i = 0; i < matches.size(); i++)
	{
		cout << matches[i]->x << "," << matches[i]->y << "\t" << matches[i]->getScoreNormalisedCorrelation() << endl;
	}
}



void LargeImage::NNS_SquaredDifference(Image * templateImage, int keepTop)
{
	mutex coutMu;

	auto worker = [](int start, int end, LargeImage* image, Image* templateImage, mutex& coutMu, int keepTop) {

		for (int i = start; i < end; i++)
		{
			int x = i % (image->width - templateImage->width);//calc x and y co-ords from index
			int y = i / (image->width - templateImage->width);


			MatchImage* match = new MatchImage(image, x, y, templateImage);//create a match image for the location

			match->getScoreSquaredDifference();
			lock_guard<mutex> lock(coutMu);
			image->addMatchSSD(match, keepTop);

		}
	};

	int threadCount = thread::hardware_concurrency();
	//int threadCount = 7;
	vector<thread> threads(threadCount);
	const int grainsize = ((width - templateImage->width) * (height - templateImage->height)) / threadCount;//operations given to each thread

	int worker_iter = 0;

	for (int i = 0; i < threadCount - 1; i++)//all threads apart from last one
	{
		threads[i] = thread(worker, worker_iter, worker_iter + grainsize, this, templateImage, std::ref(coutMu), keepTop);
		worker_iter += grainsize;
	}
	threads.back() = thread(worker, worker_iter, (width - templateImage->width) * (height - templateImage->height), this, templateImage, std::ref(coutMu), keepTop);//last grainsize section, + any leftovers

	for (auto&& i : threads)
	{
		i.join();
	}
	//sort matches
	sort(matches.begin(), matches.end(), [](MatchImage* lhs, MatchImage* rhs) {return lhs->getScoreSquaredDifference() < rhs->getScoreSquaredDifference(); });
	//print em
	for (int i = 0; i < matches.size(); i++)
	{
		cout << matches[i]->x << "," << matches[i]->y << "\t" << matches[i]->getScoreSquaredDifference() << endl;
	}
}

void LargeImage::addMatchNNS(MatchImage * match, int topnum)
{
	if (matches.size() < topnum)
	{
		matches.push_back(match);
		return;
	}
	
	int lowestPosition = 0;
	double lowest = matches[0]->getScoreNormalisedCorrelation();

	for (int i = 0; i < matches.size(); i++)
	{
		double d = matches[i]->getScoreNormalisedCorrelation();
		if (d < lowest)
		{
			lowestPosition = i;
			lowest = d;
		}
	}
	if (lowest < match->getScoreNormalisedCorrelation()) {
		delete matches[lowestPosition];
		matches[lowestPosition] = match;
	}
	else {
		delete match;
	}
}

void LargeImage::addMatchSSD(MatchImage * match, int topnum)
{
	if (matches.size() < topnum)
	{
		matches.push_back(match);
		return;
	}

	int lowestPosition = 0;
	double lowest = matches[0]->getScoreSquaredDifference();

	for (int i = 0; i < matches.size(); i++)
	{
		double d = matches[i]->getScoreSquaredDifference();
		if (d > lowest)
		{
			lowestPosition = i;
			lowest = d;
		}
	}
	if (lowest > match->getScoreSquaredDifference()) {
		delete matches[lowestPosition];
		matches[lowestPosition] = match;
	}
	else {
		delete match;
	}
}



LargeImage::~LargeImage()
{
	for (auto&& i : matches)
	{
		delete i;
	}
}
