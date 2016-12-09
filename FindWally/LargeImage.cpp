#include "LargeImage.h"
#include "MatchImage.h"
#include <vector>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

void LargeImage::NNS_NormalisedCorrelation(Image * templateImage)
{
	mutex coutMu;

	auto worker = [](int start, int end, LargeImage* image, Image* templateImage, mutex& coutMu) {
		
		for (int i = start; i < end; i++)
		{
			int x = i % (image->width - templateImage->width);//calc x and y co-ords from index
			int y = i / (image->width - templateImage->width);


			MatchImage* match = new MatchImage(image,x,y,templateImage->width,templateImage->height);//create a match image for the location

			match->getScoreNormalisedCorrelation(templateImage);

			delete match;

		}
	};

	int threadCount = thread::hardware_concurrency();
	//int threadCount = 7;
	vector<thread> threads(threadCount);
	const int grainsize = ((width - templateImage->width) * (height - templateImage->height)) / threadCount;//operations given to each thread

	int worker_iter = 0;

	for (int i = 0; i < threadCount - 1; i++)//all threads apart from last one
	{
		threads[i] = thread(worker,worker_iter,worker_iter + grainsize, this,templateImage, std::ref(coutMu));
		worker_iter += grainsize;
	}
	threads.back() = thread(worker, worker_iter, (width - templateImage->width) * (height - templateImage->height), this,templateImage, std::ref(coutMu));//last grainsize section, + any leftovers

	for (auto&& i : threads)
	{
		i.join();
	}
}

void LargeImage::addMatch(MatchImage * match)
{
	matches.push_back(match);
}

LargeImage::~LargeImage()
{
	for (auto&& i : matches)
	{
		delete i;
	}
}
