#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "PPMLine.h"

class PPMImage
{
private:
	std::string magicNumber;
	unsigned int dimx, dimy, max;
	std::vector<PPMLine> lines;

public:
	PPMImage();
	PPMImage(std::string filename);
	PPMImage(const PPMImage& other);
	~PPMImage();
	void LoadFile(std::string filename);
	void DrawLine(int startX, int startY, int endX, int endY, RGBValue color);
	PPMImage& operator += (const PPMLine& line);
	PPMImage operator + (const PPMLine& line);
	PPMImage operator + (const PPMImage& other);
	PPMImage operator - (const PPMImage& other);
	PPMImage operator - ();
	bool operator == (const PPMImage& image);
	bool operator != (const PPMImage& image);
	friend std::ostream& operator << (std::ostream& out, const PPMImage& image);
};

std::ostream& operator << (std::ostream& os, const PPMImage& image);
