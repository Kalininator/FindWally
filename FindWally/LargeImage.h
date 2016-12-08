#pragma once

#include "Image.h"
#include "MatchImage.h"
#include <string>
#include <vector>

class LargeImage : public Image {
public:
	LargeImage(int width, int height, std::string filename) : Image(width,height,filename) {}
	void NNS_NormalisedCorrelation(Image* templateImage);
	void addMatch(MatchImage* match);
	~LargeImage();
private:
	std::vector<MatchImage*> matches;
};