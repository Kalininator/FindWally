#include "MatchImage.h"
#include <cmath>

MatchImage::MatchImage(Image * source, int x, int y, int width, int height)
{
	this->width = width;
	this->height = height;
	this->source = source;
	allocArray();

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			setValue(i, j, source->getValue(i + x, j + y));
		}
	}

}

double MatchImage::getScoreSquaredDifference(Image * templateImage)
{
	if (scores.find("SquaredDifference") == scores.end())
	{
		//if doesn't already have it calculated
		//calculate sum of squared Differences



		//store it in map

		scores["SquaredDifference"] = 0.0;
	}
	//return it, now that it is definitely stored
	return scores["SquaredDifference"];
}

double MatchImage::getScoreNormalisedCorrelation(Image * templateImage)
{
	//no mean provided for template image

	//calc mean
	double mean = templateImage->getTotal() / (templateImage->width * templateImage->height);

	return getScoreNormalisedCorrelation(templateImage,mean);
}

double MatchImage::getScoreNormalisedCorrelation(Image * templateImage, double templateImageMean)
{
	//mean already provided :) yay less calculation

	if (scores.find("NormalisedCorrelation") == scores.end())
	{
		//if doesn't already have it calculated
		//calculate normalised correlation

		double score;

		float mean_scene = getTotal() / (width * height);

		Image* I = *this - mean_scene;
		Image* T = *templateImage - templateImageMean;

		Image* IT = *I * *T;
		Image* II = *I * *I;
		Image* TT = *T * *T;

		double top = IT->getTotal();
		double bottom = (double)II->getTotal() * (double)TT->getTotal();
		double sqrtbottom = sqrt(bottom);
		score = (top / sqrtbottom);

		delete I;
		delete T;
		delete II;
		delete TT;
		delete IT;
		//store it in map

		scores["NormalisedCorrelation"] = score;
	}
	//return it, now that it is definitely stored
	return scores["NormalisedCorrelation"];
}

