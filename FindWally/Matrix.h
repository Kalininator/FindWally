#ifndef MATRIX_H
#define MATRIX_H
#include <string>
class Matrix
{
public:
	Matrix();
	Matrix(int x, int y);
	void setVal(int x, int y, int val);
	int getVal(int x, int y);
	void printMatrix();
	void fillFromFile(std::string filename);
	~Matrix();
private:
	int width, height;
	int **p;

	void allocArray()
	{
		p = new int*[width];
		for (int i = 0; i < width; i++)
		{
			p[i] = new int[height];
		}
	}
};

#endif