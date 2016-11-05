#include "MatchImage.h"
#include "Image.h"

MatchImage::MatchImage(int x, int y, std::string filename) : Image(x, y, filename)
{
}

MatchImage::~MatchImage()
{
	delete matrix;
}
