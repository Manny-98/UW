#include "PPMImage.h"

PPMImage::PPMImage() {
	magicNumber = "P6";
	dimx = 0;
	dimy = 0;
	max = 0;
}

PPMImage::PPMImage(std::string filename) {
	magicNumber = "P6";
	dimx = 0;
	dimy = 0;
	max = 0;
	LoadFile(filename);
}

PPMImage::PPMImage(const PPMImage& other) {
	magicNumber = other.magicNumber;
	dimx = other.dimx;
	dimy = other.dimy;
	max = other.max;
	lines = other.lines;
}

PPMImage::~PPMImage() {

}

void PPMImage::LoadFile(std::string filename) {
	lines.clear();
	std::ifstream ifs(filename, std::ios_base::in | std::ios_base::binary);
	if (ifs.fail()) {
		 throw std::invalid_argument("");
	}
	ifs >> magicNumber >> dimx >> dimy >> max;
	lines.resize(dimy);
	for (auto j = 0; j < dimy; ++j) {
		std::vector<RGBValue> pixels(dimx);
		for (auto i = 0; i < dimx; ++i) {
			pixels[i].blue = ifs.get(); // (max + ifs.get()) % max;
			pixels[i].red = ifs.get();// (max + ifs.get()) % max;
			pixels[i].green = ifs.get();// (max + ifs.get()) % max;
		}
		lines[j] = PPMLine(pixels, max);
	}
	ifs.close();
}

void PPMImage::DrawLine(int startX, int startY, int endX, int endY, RGBValue color) {
	if (startX < 0 || startX >= dimx || startY < 0 || startY >= dimy
		|| endX < 0 || endX >= dimx || endY < 0 || endY >= dimy) {
		 throw std::invalid_argument("");
	}
	if (startX > endX) {
		int temp = startX;
		startX = endX;
		endX = temp;
		temp = startY;
		startY = endY;
		endY = temp;
	}
	double lengthX = std::abs(endX - startX);
	double lengthY = std::abs(endY - startY);
	double incrementX = lengthX / std::max(lengthX, lengthY);
	double incrementY = lengthY / std::max(lengthX, lengthY);
	double currentX = startX;
	double currentY = startY;
	while (currentX < endX) {
		lines[(int)currentY].pixels[(int)currentX] = color;
		currentX += incrementX;
		currentY += (startY < endY) ? incrementY : -incrementY;
	}
}

PPMImage& PPMImage::operator += (const PPMLine& line) {
	/*if (line.maxColorSize != max) {
		throw std::invalid_argument("");
	}*/
	/*for (auto j = 0; j < dimy; ++j) {
		for (auto i = 0; i < dimx; ++i) {
			if (lines[j].pixels[i].red == 0 && lines[j].pixels[i].green == 0 && lines[j].pixels[i].blue == 0)
				 throw std::invalid_argument("");
		}
	}*/
	dimy++;
	lines.push_back(line);
	return *this;
}

PPMImage PPMImage::operator + (const PPMLine& line) {
	/*if (line.maxColorSize != max) {
		 throw std::invalid_argument("");
	}*/
	/*for (auto j = 0; j < dimy; ++j) {
		for (auto i = 0; i < dimx; ++i) {
			if (lines[j].pixels[i].red == 0 && lines[j].pixels[i].green == 0 && lines[j].pixels[i].blue == 0) 
				throw std::invalid_argument("");
		}
	}*/
	PPMImage copy(*this);
	copy.dimy++;
	copy.lines.push_back(line);
	return copy;
}

PPMImage PPMImage::operator + (const PPMImage& other) {
	PPMImage image;
	image.dimx = std::max(dimx, other.dimx);
	image.dimy = std::max(dimy, other.dimy);
	image.max = std::max(max, other.max);
	image.lines = std::vector<PPMLine>(image.dimy);
	for (auto j = 0; j < image.dimy; ++j) {
		std::vector<RGBValue> pixels(image.dimx);
		for (auto i = 0; i < image.dimx; ++i) {
			if (j < dimy && i < dimx && j < other.dimy && i < other.dimx) {
				pixels[i].red = (lines[j].pixels[i].red + other.lines[j].pixels[i].red) % image.max;
				pixels[i].green = (lines[j].pixels[i].green + other.lines[j].pixels[i].green) % image.max;
				pixels[i].blue = (lines[j].pixels[i].blue + other.lines[j].pixels[i].blue) % image.max;
			}
			else if (j < dimy && i < dimx) {
				pixels[i].red = lines[j].pixels[i].red;
				pixels[i].green = lines[j].pixels[i].green;
				pixels[i].blue = lines[j].pixels[i].blue;
			}
			else if (j < other.dimy && i < other.dimx) {
				pixels[i].red = other.lines[j].pixels[i].red;
				pixels[i].green = other.lines[j].pixels[i].green;
				pixels[i].blue = other.lines[j].pixels[i].blue;
			}
		}
		image.lines[j] = PPMLine(pixels, image.max);
	}
	return image;
}

PPMImage PPMImage::operator - (const PPMImage& other) {
	PPMImage image;
	image.dimx = std::max(dimx, other.dimx);
	image.dimy = std::max(dimy, other.dimy);
	image.max = std::max(max, other.max);
	image.lines = std::vector<PPMLine>(image.dimy);
	for (auto j = 0; j < image.dimy; ++j) {
		std::vector<RGBValue> pixels(image.dimx);
		for (auto i = 0; i < image.dimx; ++i) {
			if (j < dimy && i < dimx && j < other.dimy && i < other.dimx) {
				pixels[i].red = (lines[j].pixels[i].red - other.lines[j].pixels[i].red) % image.max;
				pixels[i].green = (lines[j].pixels[i].green - other.lines[j].pixels[i].green) % image.max;
				pixels[i].blue = (lines[j].pixels[i].blue - other.lines[j].pixels[i].blue) % image.max;
			}
			else if (j < dimy && i < dimx) {
				pixels[i].red = lines[j].pixels[i].red;
				pixels[i].green = lines[j].pixels[i].green;
				pixels[i].blue = lines[j].pixels[i].blue;
			}
			else if (j < other.dimy && i < other.dimx) {
				pixels[i].red = (0 - other.lines[j].pixels[i].red) % image.max;
				pixels[i].green = (0 - other.lines[j].pixels[i].green) % image.max;
				pixels[i].blue = (0 - other.lines[j].pixels[i].blue) % image.max;
			}
		}
		image.lines[j] = PPMLine(pixels, image.max);
	}
	return image;
}

PPMImage PPMImage::operator - () {
	PPMImage copy(*this);
	for (auto j = 0; j < dimy; ++j) {
		for (auto i = 0; i < dimx; ++i) {
			copy.lines[j].pixels[i].red = max - copy.lines[j].pixels[i].red;
			copy.lines[j].pixels[i].green = max - copy.lines[j].pixels[i].green;
			copy.lines[j].pixels[i].blue = max - copy.lines[j].pixels[i].blue;
		}
	}
	return copy;
}


bool PPMImage::operator == (const PPMImage& image) {
	if (magicNumber != image.magicNumber || dimx != image.dimx || dimy != image.dimy || max != image.max || lines.size() != image.lines.size()) {
		return false;
	}
	for (auto i = 0; i < lines.size(); ++i) {
		if (lines[i] != image.lines[i]) {
			return false;
		}
	}
	return true;
}

bool PPMImage::operator != (const PPMImage& image) {
	if (magicNumber != image.magicNumber || dimx != image.dimx || dimy != image.dimy || max != image.max || lines.size() != image.lines.size()) {
		return true;
	}
	for (auto i = 0; i < lines.size(); ++i) {
		if (lines[i] != image.lines[i]) {
			return true;
		}
	}
	return false;
}

std::ostream& operator << (std::ostream& os, const PPMImage& image) {
	os << image.magicNumber << std::endl << image.dimx << ' ' << image.dimy << std::endl << image.max << std::endl;
	for (auto j = 0; j < image.lines.size(); ++j) {
		os << image.lines[j];
	}
	return os;
}
