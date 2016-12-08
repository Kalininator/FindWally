#pragma once
#include "Image.h"
#include <map>
#include <string>
class MatchImage : Image
{
public:
	MatchImage(Image* source, int x, int y, int width, int height);
	double getScoreSquaredDifference(Image* templateImage);
	double getScoreNormalisedCorrelation(Image* templateImage);
	double getScoreNormalisedCorrelation(Image* templateImage, double templateImageMean);
private:
	std::map<std::string, double> scores;
	Image* source;
};