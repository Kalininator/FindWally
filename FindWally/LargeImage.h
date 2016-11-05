#ifndef LARGEIMAGE_H
#define LARGEIMAGE_H

#include <vector>
#include "Image.h"
#include "MatchImage.h"

class LargeImage : Image
{
public:
	LargeImage(int x, int y, std::string filename);
	
	void NNS(MatchImage* matchImage);
	~LargeImage();
private:
	std::vector<MatchImage> matches;
};

#endif
