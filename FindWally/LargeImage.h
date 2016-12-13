#pragma once

#include "Image.h"
#include "MatchImage.h"
#include <string>
#include <vector>

class LargeImage : public Image {
public:
	LargeImage(int width, int height, std::string filename) : Image(width,height,filename) {}
	void NNS_NormalisedCorrelation(Image* templateImage, int keepTop);
	void NNS_SquaredDifference(Image * templateImage, int keepTop);
	void addMatchNNS(MatchImage* match, int topnum);
	void addMatchSSD(MatchImage* match, int topnum);
	~LargeImage();
private:
	std::vector<MatchImage*> matches;
};