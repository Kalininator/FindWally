#pragma once
#include "Image.h"
#include <map>
#include <string>
class MatchImage : public Image
{
public:
	MatchImage(Image* source, int x, int y, Image* templateImage);
	double getScoreSquaredDifference();
	double getScoreNormalisedCorrelation();//standard algorithm
	double getScoreNormalisedCorrelation(double templateImageMean);//mean value of template image ahs already been calculated, so this method can be used to save processing power
	int x, y;//store position of the match inside it's parent scene
private:
	std::map<std::string, double> scores; //use a map to store scores, so it's easy to tell if a score has already been calculated
	
	Image* templateImage;
	Image* source;
};