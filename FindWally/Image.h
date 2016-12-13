#pragma once
#include <string>

class Image
{
public:
	Image();
	Image(int width, int height);
	Image(int width, int height, std::string filename);
	void setValue(int x, int y, int val);
	int getValue(int x, int y);
	Image* operator-(int value);
	Image* operator*(Image& image);
	long getTotal();
	void printMatrix();
	void generatePGM(std::string filename);
	void fillFromFile(std::string filename);
	
	int width, height;
	
	~Image();
private:
	int **p;
protected:
	void allocArray();
};

