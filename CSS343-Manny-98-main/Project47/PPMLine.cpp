#include "PPMLine.h"

PPMLine::PPMLine(std::vector<RGBValue> pixels, int maxColorSize) {
	this->pixels = pixels;
	this->maxColorSize = maxColorSize;
}

bool PPMLine::operator == (const PPMLine& line) {
	if (maxColorSize != line.maxColorSize || pixels.size() != line.pixels.size()) {
		return false;
	}
	for (auto i = 0; i < pixels.size(); ++i) {
		if (pixels[i].red != line.pixels[i].red || pixels[i].green != line.pixels[i].green || pixels[i].blue != line.pixels[i].blue) {
			return false;
		}
	}
	return true;
}

bool PPMLine::operator != (const PPMLine& line) {
	if (maxColorSize != line.maxColorSize || pixels.size() != line.pixels.size()) {
		return true;
	}
	for (auto i = 0; i < pixels.size(); ++i) {
		if (pixels[i].red != line.pixels[i].red || pixels[i].green != line.pixels[i].green || pixels[i].blue != line.pixels[i].blue) {
			return true;
		}
	}
	return false;
}

std::ostream& operator << (std::ostream& os, const PPMLine& line) {
	for (auto i = 0; i < line.pixels.size(); ++i) {
		os << line.pixels[i].red << line.pixels[i].green << line.pixels[i].blue; // red, green, blue
	}
	return os;
}