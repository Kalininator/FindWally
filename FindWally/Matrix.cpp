#include "Matrix.h"
#include <iostream>
#include <string>
#include <fstream>


Matrix::Matrix() : Matrix(1, 1) {}

Matrix::Matrix(int xval, int yval)
{
	width = xval;
	height = yval;
	allocArray();
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			p[i][j] = 0;
		}
	}
}

void Matrix::fillFromFile(std::string filename)
{
	int i = 0;

	std::ifstream file(filename);

	while (file.good())
	{
		if (i >= width * height)
			break;

		double d;
		file >> d;

		setVal(i%width, i / width, (int)d);
		i++;
	}
	file.close();
}

void Matrix::setVal(int xpos, int ypos, int val)
{
	if (xpos >= width || ypos >= height)
		return;

	p[xpos][ypos] = val;
	
}

int Matrix::getVal(int x, int y)
{
	return p[x][y];
}

void Matrix::printMatrix()
{
	

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			std::cout << getVal(i, j);
			std::cout << ",";
		}
		std::cout << std::endl;
	}

}

Matrix::~Matrix()
{
	for (int i = 0; i < width; i++)
	{
		delete[] p[i];
	}
	delete[] p;
}
