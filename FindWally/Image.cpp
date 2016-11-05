#include "Image.h"
#include <fstream>
#include <iostream>

Image::Image(int x, int y, std::string filename)
{
	width = x;
	height = y;
	matrix = new Matrix(width,height);
	(*matrix).fillFromFile(filename);
}

int Image::getPixel(int x, int y)
{
	return (*matrix).getVal(x, y);
}



void Image::setPixel(int x, int y, int val)
{
	(*matrix).setVal(x, y, val);
}

void Image::generatePGM(std::string filename)
{
	
	unsigned char* image;

	image = (unsigned char *) new unsigned char[width*height];

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			*(image + (j*width) + i) = (unsigned char)getPixel(i, j);
		}
	}


	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	file << "P5" << "\n";
	file << width << " " << height << "\n";
	file << "255" << "\n";

	file.write(reinterpret_cast<char *>(image), (width*height)*sizeof(unsigned char));

	if (file.fail())
	{
		std::cout << "can't open file";

	}

	file.close();

	delete[] image;
}

void Image::printImage()
{
	(*matrix).printMatrix();
}

int Image::getScore(int x, int y, Image * match)
{
	int score = 0;

	

	for (int i = x; i < x + match->width; i++)
	{
		for (int j = y; j < y + match->height; j++)
		{
			//for each pixel in image
			score += (abs(nearestNeighbourAverage(i + x,j + y, 1) - match->getPixel(i-x,j-y)));

		}
	}

	return score;
}

int Image::nearestNeighbourAverage(int x, int y, int k)
{
	int ttl = 0;
	int count = 0;
	for (int i = x - k; i <= x + k; i ++)
	{
		for (int j = y - k; j <= y + k; j++)
		{
			
			if (i >= 0 && i < width && j >= 0 && j < height)
			{
				ttl += getPixel(i,j);
				count++;
			}

			
			
		}
	}

	if (count > 0)
	{
		return ttl / count;

	}
	return 0;
}

Image::~Image()
{
	delete matrix;
}
