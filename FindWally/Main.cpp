#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "Image.h"
#include "MatchImage.h"
#include "LargeImage.h"

using namespace std;

double* getArrayFromFile(string filename, int width, int height)
{
	double* data = new double[width*height];

	int i = 0;

	ifstream file(filename);

	if (file.is_open())
	{
		while (file.good())
		{
			if (i >= width*height)
				break;
			file >> *(data + i);
			//cout << *(data + i);
			i++;
		}
		file.close();
	}
	else {
		cout << "error code ID10T";
	}

	return data;
}
Matrix* getMatrixFromFile(string filename, int width, int height)
{
	Matrix* matrix = new Matrix(width,height);

	int i = 0;

	ifstream file(filename);

	while (file.good())
	{
		if (i >= width * height)
			break;

		double d;
		file >> d;

		(*matrix).setVal(i%width, i / width, (int)d);
		i++;
	}
	file.close();

	return matrix;
}

int main()
{
	//1024,768
	//wally: 36,49

	/*double* input_data = 0;
	input_data = getArrayFromFile("Cluttered_scene.txt",1024,768);
	cout << *(input_data);
	delete[] input_data;*/

	//Matrix* matrix = getMatrixFromFile("Cluttered_scene.txt",1024,768);
	

	//delete(matrix);

	Image* wally = new Image(36,49,"Wally_grey.txt");
	Image* scene = new Image(1024, 768, "Cluttered_scene.txt");
	//(*wally).printImage();

	//(*wally).generatePGM("wally.pgm");

	//cout << scene->getScore(106,209,wally);

	//int lowestscore = 10000000;

	/*for (int n = 0; n < scene->width - wally->width; n++)
	{
		for (int m = 0; m < scene->height - wally->height; m++)
		{
			int score;
			score = scene->getScore(n,m,wally);
			if (score < lowestscore)
			{
				lowestscore = score;
				cout << "x:" << n << ",y:" << m << endl;
			}
		}
	}*/


	delete wally;
	delete scene;
	system("pause");

	return 0;
}
