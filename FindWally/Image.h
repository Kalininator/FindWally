#ifndef IMAGE_H
#define IMAGE_H
#include "Matrix.h"
class Image
{
public:
	Image(int x, int y, std::string filename);
	int getPixel(int x, int y);
	void setPixel(int x, int y, int val);
	void generatePGM(std::string filename);
	void printImage();
	int getScore(int x, int y, Image* match);
	int nearestNeighbourAverage(int x, int y, int k);
	int width;
	int height;
	~Image();
protected:
	Matrix* matrix;
	
};

#endif
