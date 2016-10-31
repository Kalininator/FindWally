#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
public:
	Matrix();
	Matrix(int x, int y);
	void setVal(int x, int y, int val);
	int getVal(int x, int y);
	void printMatrix();
	~Matrix();
private:
	int x, y;
	int **p;

	void allocArray()
	{
		p = new int*[x];
		for (int i = 0; i < x; i++)
		{
			p[i] = new int[y];
		}
	}
};

#endif