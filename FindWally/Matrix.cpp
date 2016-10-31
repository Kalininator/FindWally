#include "Matrix.h"

Matrix::Matrix() : Matrix(1, 1) {}

Matrix::Matrix(int xval, int yval)
{
	x = xval;
	y = yval;
	allocArray();
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			p[i][j] = 0;
		}
	}
}

void Matrix::setVal(int x, int y, int val)
{
	p[x][y] = val;
}

int Matrix::getVal(int x, int y)
{
	return p[x][y];
}

void Matrix::printMatrix()
{

}

Matrix::~Matrix()
{
	for (int i = 0; i < x; i++)
	{
		delete[] p[i];
	}
	delete[] p;
}
