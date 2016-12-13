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
	mutex imageMutex;
	//create a function to be used by threads
	auto worker = [](int start, int end, LargeImage* image, Image* templateImage, mutex& imageMutex,double templateImageMean, int keepTop) {
		
		for (int i = start; i < end; i++)
		{
			int x = i % (image->width - templateImage->width);//calc x and y co-ords from index
			int y = i / (image->width - templateImage->width);


			MatchImage* match = new MatchImage(image,x,y,templateImage);//create a match image for the location

			match->getScoreNormalisedCorrelation(templateImageMean);
			lock_guard<mutex> lock(imageMutex);
			image->addMatchNNS(match,keepTop);

		}
	};

	int threadCount = thread::hardware_concurrency();
	vector<thread> threads(threadCount);
	int length = (width - templateImage->width) * (height - templateImage->height);
	const int grainsize = length / threadCount;//operations given to each thread
	double templateImageMean = templateImage->getTotal() / (templateImage->width * templateImage->height);
	int worker_iter = 0;

	for (int i = 0; i < threadCount - 1; i++)//all threads apart from last one
	{
		threads[i] = thread(worker,worker_iter,worker_iter + grainsize, this,templateImage, std::ref(imageMutex),templateImageMean, keepTop);
		worker_iter += grainsize;
	}
	threads.back() = thread(worker, worker_iter, length, this,templateImage, std::ref(imageMutex),templateImageMean,keepTop);//last grainsize section, + any leftovers

	for (auto&& i : threads)
	{
		i.join();
	}

	//sort matches
	sort(matches.begin(), matches.end(), [](MatchImage* lhs, MatchImage* rhs) {return lhs->getScoreNormalisedCorrelation() > rhs->getScoreNormalisedCorrelation(); });
	//print em
	for (int i = 0; i < matches.size(); i++)
	{
		cout << "#" << i + 1 << ":\t" << matches[i]->x << "," << matches[i]->y << "\t\t" << matches[i]->getScoreNormalisedCorrelation() << endl;
	}
}

void LargeImage::NNS_SquaredDifference(Image * templateImage, int keepTop)
{
	mutex imageMutex;//used to lock access to the large image, so only one thread can access it at a time

	auto worker = [](int start, int end, LargeImage* image, Image* templateImage, mutex& imageMutex, int keepTop) {

		for (int i = start; i < end; i++)
		{
			int x = i % (image->width - templateImage->width);//calc x and y co-ords from index
			int y = i / (image->width - templateImage->width);


			MatchImage* match = new MatchImage(image, x, y, templateImage);//create a match image for the location

			match->getScoreSquaredDifference();
			lock_guard<mutex> lock(imageMutex);//lock the mutex for accessing the large image, unlocks when lock_guard leaves the scope
			image->addMatchSSD(match, keepTop);

		}
	};

	int threadCount = thread::hardware_concurrency();//numbe rof threads to be used, set by hardware capabilities
	vector<thread> threads(threadCount);//store the threads to be used
	int length = (width - templateImage->width) * (height - templateImage->height);
	const int grainsize = length / threadCount;//operations given to each thread

	int worker_iter = 0;

	for (int i = 0; i < threadCount - 1; i++)//all threads apart from last one
	{
		threads[i] = thread(worker, worker_iter, worker_iter + grainsize, this, templateImage, std::ref(imageMutex), keepTop);
		worker_iter += grainsize;
	}
	threads.back() = thread(worker, worker_iter, length, this, templateImage, std::ref(imageMutex), keepTop);//last grainsize section, + any leftovers

	for (auto&& i : threads)//wait for all threads to finish
	{
		i.join();
	}
	//sort matches
	//use custom function for comparison, so that the algorithm can sort by internal values of the MatchImages
	sort(matches.begin(), matches.end(), [](MatchImage* lhs, MatchImage* rhs) {return lhs->getScoreSquaredDifference() < rhs->getScoreSquaredDifference(); });
	//print em
	for (int i = 0; i < matches.size(); i++)
	{
		cout << "#" << i + 1 << ":\t"<< matches[i]->x << "," << matches[i]->y << "\t" << matches[i]->getScoreSquaredDifference() << endl;
	}
}

void LargeImage::addMatchNNS(MatchImage * match, int topnum)
{
	if (matches.size() < topnum) //check if top x values has x values yet
	{
		matches.push_back(match); //not enough values in matches, so just add the match
		return;
	}
	
	int lowestPosition = 0;//track index of lowest score in matches
	double lowest = matches[0]->getScoreNormalisedCorrelation();//track score at lowestPosition

	for (int i = 0; i < matches.size(); i++)//loop through matches and find lowest score
	{
		double d = matches[i]->getScoreNormalisedCorrelation();
		if (d < lowest)
		{
			lowestPosition = i;
			lowest = d;
		}
	}
	if (lowest < match->getScoreNormalisedCorrelation()) {//check if lowest score in matches is lower than the match being added
		delete matches[lowestPosition];//delete match at lowestPosition, so it can be replaced by the new match
		matches[lowestPosition] = match;
	}
	else {
		delete match;//match at lowestPosition is a better score, so discard the new match
	}
}

void LargeImage::addMatchSSD(MatchImage * match, int topnum)
{
	if (matches.size() < topnum)//check if top x values has x values yet
	{
		matches.push_back(match); //not enough values in matches, so just add the match
		return;
	}

	int lowestPosition = 0;//track index of lowest score in matches
	double lowest = matches[0]->getScoreSquaredDifference();//track score at lowestPosition

	for (int i = 0; i < matches.size(); i++)
	{
		double d = matches[i]->getScoreSquaredDifference();
		if (d > lowest)
		{
			lowestPosition = i;
			lowest = d;
		}
	}
	if (lowest > match->getScoreSquaredDifference()) {//check if lowest score in matches is lower than the match being added
		delete matches[lowestPosition];//delete match at lowestPosition, so it can be replaced by the new match
		matches[lowestPosition] = match;
	}
	else {
		delete match;//match at lowestPosition is a better score, so discard the new match
	}
}



LargeImage::~LargeImage()
{
	for (auto&& i : matches)//delete all matches stored from heap
	{
		delete i;
	}
}
