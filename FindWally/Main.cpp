#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "Matrix.h"

using namespace std;

Matrix* getMatrixFromFile(string filename)
{
	ifstream file;
	file.open(filename, ios::in);

	int width,height;

	string line;
	//get first line with dimensions
	getline(file,line);
	istringstream str (line);

	getline(str,line,' ');
	width = stoi(line);
	getline(str,line,' ');
	height = stoi(line);

	Matrix* matrix = new Matrix(width,height);
	int county = 0;
	while(getline(file,line))
	{
		istringstream stream (line);
		int countx= 0;
		string val;
		while(getline(stream,val,' '))
		{
			(*matrix).setVal(countx,county,stoi(val));
			countx ++;
		}
		county ++;
	}

	file.close();

	return matrix;
}

int main()
{
	Matrix* matrix = getMatrixFromFile("sample.txt");

	matrix -> printMatrix();

	delete matrix;
	
	return 0;
}
