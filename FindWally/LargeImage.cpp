#include "LargeImage.h"

LargeImage::LargeImage(int x, int y, std::string filename) : Image(x, y, filename) {}


void LargeImage::NNS(MatchImage * matchImage)
{

	//add all matches to matches vector

}

LargeImage::~LargeImage()
{
	delete matrix;
}
