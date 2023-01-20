#pragma once

struct RGBValue {
	char red = 0;
	char green = 0;
	char blue = 0;

	bool operator==(const RGBValue& rhs) const {
		if (red == rhs.red && blue == rhs.blue && green == rhs.green) {
			return true;
		}
		return false;
	}
};