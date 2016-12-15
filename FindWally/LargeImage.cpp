#include "LargeImage.h"
#include "MatchImage.h"
#include <vector>
#include <iostream>
#include <mutex>
#include <thread>
#include <algorithm>
#include <cmath>

using namespace std;

void LargeImage::NNS_NormalisedCorrelation(Image * templateImage, int keepTop, int threadCount)
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
			lock_guard<mutex> lock(imageMutex);//lock the mutex for accessing the large image, unlocks when lock_guard leaves the scope
			image->addMatchNC(match,keepTop);

		}
	};

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
	
}

void LargeImage::NNS_SquaredDifference(Image * templateImage, int keepTop, int threadCount)
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

	vector<thread> threads(threadCount);//store the threads to be used
	//calculate how many total match images will be created
	int length = (width - templateImage->width) * (height - templateImage->height); 
	//calculate how many operations are given to each thread
	const int grainsize = length / threadCount;

	int worker_iter = 0;//start position of current thread
	for (int i = 0; i < threadCount - 1; i++)//all threads apart from last one
	{
		//start a thread, given its appropriate section
		threads[i] = thread(worker, worker_iter, worker_iter + grainsize, this, templateImage, std::ref(imageMutex), keepTop);
		worker_iter += grainsize;//move start position for the next thread
	}
	//start last thread
	//the last thread's end position is the last value, so that any leftover matches are also calculated
	//this is necessary if the number of matches is not divisible by the number of threads being used
	threads.back() = thread(worker, worker_iter, length, this, templateImage, std::ref(imageMutex), keepTop);//last grainsize section, + any leftovers

	for (auto&& i : threads)//wait for all threads to finish
	{
		i.join();
	}
	//sort matches
	//use custom function for comparison, so that the algorithm can sort by internal values of the MatchImages
	sort(matches.begin(), matches.end(), [](MatchImage* lhs, MatchImage* rhs) {return lhs->getScoreSquaredDifference() < rhs->getScoreSquaredDifference(); });
	
}

void LargeImage::addMatchNC(MatchImage * match, int topnum)
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

void LargeImage::printMatchesSquaredDifference()
{
	//print em
	cout << "Results:" << endl;
	for (int i = 0; i < matches.size(); i++)
	{
		cout << "#" << i + 1 << ":\t" << matches[i]->x << "," << matches[i]->y << "\t\t" << matches[i]->getScoreSquaredDifference() << endl;
	}
}

void LargeImage::printMatchesNormalisedCorrelation()
{
	//print em
	cout << "Results:" << endl;
	for (int i = 0; i < matches.size(); i++)
	{
		cout << "#" << i + 1 << ":\t" << matches[i]->x << "," << matches[i]->y << "\t\t" << matches[i]->getScoreNormalisedCorrelation() << endl;
	}
}

void LargeImage::drawMatches()
{
	for (MatchImage* i : matches)
	{
		for (int y = i->y; y < i->y + i->height; y++)
		{
			setValue(i->x, y, 0);
			setValue(i->x + i->width, y, 0);
		}
		for (int x = i->x; x < i->x + i->width; x++)
		{
			setValue(x, i->y, 0);
			setValue(x, i->y + i->height, 0);
		}
	}
}




LargeImage::~LargeImage()
{
	for (auto&& i : matches)//delete all matches stored from heap
	{
		delete i;
	}
}
