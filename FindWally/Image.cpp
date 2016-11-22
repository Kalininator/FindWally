#include "Image.h"
#include <iostream>
#include <fstream>

Image::Image() : Image(1, 1) {}

Image::Image(int width, int height)
{
	this->width = width;
	this->height = height;
	allocArray();
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			p[i][j] = 0;
		}
	}
}

Image::Image(int width, int height, std::string filename)
{
	this->width = width;
	this->height = height;
	allocArray();
	fillFromFile(filename);
}

void Image::setValue(int x, int y, int val)
{
	if (x >= width || y >= height)
		return;

	p[x][y] = val;
}

int Image::getValue(int x, int y)
{
	return p[x][y];
}

Image * Image::getSection(int x, int y, int width, int height)
{
	if (x + width >= this->width || y + height >= this->height)
	{
		return nullptr;
	}

	Image* m = new Image(width, height);
	
	for (int i = x; i < x + width; i++)
	{
		for (int j = y; j < y + height; j++)
		{
			
			m->setValue(i-x,j-y,getValue(i,j));
		}
	}
	return m;
}

Image* Image::operator+(Image & image)
{
	Image* m = new Image(this->width,this->height);
	if (image.width != this->width || image.height != this->height)//sizes are different, so return empty matrix
	{
		return m;
	}

	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			m->setValue(i, j,this->getValue(i,j) + image.getValue(i,j));
		}
	}

	return m;
}

Image * Image::operator+(int value)
{
	Image* m = new Image(this->width, this->height);

	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			m->setValue(i, j, this->getValue(i, j) + value);
		}
	}

	return m;
}

Image * Image::operator*(Image & image)
{
	if (image.width != this->width || image.height != this->height)//sizes are different, so return nullptr
	{
		return nullptr;
	}

	Image* m = new Image(this->width, this->height);

	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			//std::cout << this->getValue(i, j) << std::endl << image.getValue(i, j);
			int multiple = this->getValue(i, j) * image.getValue(i, j);
			m->setValue(i, j, multiple);
		}
	}

	return m;
}

long Image::getTotal()
{
	long total = 0;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			total += getValue(i, j);
		}
	}
	return total;
}

void Image::printMatrix()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			std::cout << getValue(i, j);
			std::cout << "\t";
		}
		std::cout << std::endl;
	}
}

void Image::generatePGM(std::string filename)
{
	unsigned char* image;

	image = (unsigned char *) new unsigned char[width*height];

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			*(image + (j*width) + i) = (unsigned char)getValue(i, j);
		}
	}


	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	file << "P5" << "\n";
	file << width << " " << height << "\n";
	file << "255" << "\n";

	file.write(reinterpret_cast<char *>(image), (width*height) * sizeof(unsigned char));

	if (file.fail())
	{
		std::cout << "can't open file";

	}

	file.close();

	delete[] image;
}

void Image::fillFromFile(std::string filename)
{
	int i = 0;

	std::ifstream file(filename);

	while (file.good())
	{
		if (i >= width * height)
			break;

		double d;
		file >> d;

		setValue(i%width, i / width, (int)d);
		i++;
	}
	file.close();
}

Image::~Image()
{
	for (int i = 0; i < width; i++)
	{
		delete[] p[i];
	}
	delete[] p;
}

void Image::allocArray()
{
	p = new int*[width];
	for (int i = 0; i < width; i++)
	{
		p[i] = new int[height];
	}
}
