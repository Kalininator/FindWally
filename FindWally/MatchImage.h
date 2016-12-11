#pragma once
#include "Image.h"
#include <map>
#include <string>
class MatchImage : Image
{
public:
	MatchImage(Image* source, int x, int y, Image* templateImage);
	double getScoreSquaredDifference();
	double getScoreNormalisedCorrelation();
	double getScoreNormalisedCorrelation(Image* templateImage, double templateImageMean);
	int x, y;
private:
	std::map<std::string, double> scores;
	
	Image* templateImage;
	Image* source;
};