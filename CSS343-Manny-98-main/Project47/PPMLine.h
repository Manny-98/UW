#pragma once
#include <iostream>
#include <vector>
#include "RGBValue.h"

class PPMLine
{
	friend class PPMImage;
private:
	std::vector<RGBValue> pixels;
	int maxColorSize;
public:
	PPMLine(std::vector<RGBValue> pixels = {}, int maxColorSize = 0);
	bool operator == (const PPMLine& line);
	bool operator != (const PPMLine& line);

	friend std::ostream& operator << (std::ostream& os, const PPMLine& image);
};

std::ostream& operator << (std::ostream& os, const PPMLine& line);