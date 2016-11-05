#ifndef MATCHIMAGE_H
#define MATCHIMAGE_H

#include "Image.h"

class MatchImage : Image
{
public:
	MatchImage(int x, int y, std::string filename);
	int score;
	int x, y;
	~MatchImage();
private:

};

#endif