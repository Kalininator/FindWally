#include "MatchImage.h"
#include <cmath>

MatchImage::MatchImage(Image * source, int x, int y, Image* templateImage)
{
	this->width = templateImage->width;
	this->height = templateImage->height;
	this->x = x;
	this->y = y;
	this->source = source;
	this->templateImage = templateImage;
	allocArray();

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			setValue(i, j, source->getValue(i + x, j + y));
		}
	}

}

double MatchImage::getScoreSquaredDifference()
{
	if (scores.find("SquaredDifference") == scores.end())
	{
		//if doesn't already have it calculated
		//calculate sum of squared Differences
		double total = 0;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				int difference = getValue(i, j) - templateImage->getValue(i,j);
				total += difference * difference;
			}
		}

		//divide total by width*height, sicne this will be the same for all match images, the scores will still be usable for comparison
		//total = total / (width*height);
		//store it in map

		scores["SquaredDifference"] = total;
	}
	//return it, now that it is definitely stored
	return scores["SquaredDifference"];
}

double MatchImage::getScoreNormalisedCorrelation()
{
	//no mean provided for template image

	//calc mean
	double mean = templateImage->getTotal() / (templateImage->width * templateImage->height);

	return getScoreNormalisedCorrelation(mean);
}

double MatchImage::getScoreNormalisedCorrelation(double templateImageMean)
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

