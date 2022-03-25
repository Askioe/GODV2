

#include "Screenshot.h"
#include "Time.h"

/*
		Screenshot Implementation Code
*/



Screenshot::~Screenshot()
{
}

void Screenshot::FreeMemory()
{
	if (pixels) {
		delete[] pixels;
		pixels = NULL;
	}
}

bool Screenshot::isPoopYellow(RGBQUAD& pixel)
{
	BYTE minR = 81;//175
	BYTE maxR = 100;//255
	BYTE minG = 43;//0
	BYTE maxG = 75;//75
	BYTE minB = 44;//0
	BYTE maxB = 50;//50
	if (pixel.rgbRed >= minR && pixel.rgbRed <= maxR &&
		pixel.rgbGreen >= minG && pixel.rgbGreen <= maxG &&
		pixel.rgbBlue >= minB && pixel.rgbBlue <= maxB)
	{
	return true;
	}
	else
		return false;
}


bool Screenshot::triggerBot()
{
	int centreWidth = width / 2;
	int centreHeight = height / 2;
	float percentWidth = (0.5f * (float)width) / 100.0f;
	float percentHeight = (0.5f * (float)height) / 100.0f;
	//

	int startingRow = (centreHeight - (int)percentHeight) * width;
	int endRow = (centreHeight + (int)percentHeight) * width;

	for (int row = startingRow; row < endRow; row += width)
	{
		for (int col = centreWidth - (int)percentWidth; col < centreWidth + (int)percentWidth; col++)
		{
			int index = row + col;
			if (isPoopYellow(pixels[index])) {
				return true;
			}
		}
	}


	return false;

}




bool Screenshot::isRGBEqual(RGBQUAD& r1, RGBQUAD& r2)
{
	if (r1.rgbRed == r2.rgbRed && r1.rgbGreen == r2.rgbGreen && r1.rgbBlue == r2.rgbBlue)
		return true;

	return false;
}

bool Screenshot::operator==(const Midline& line)
{
	int start = (height / 2) * width;
	int mIndex = 0;
	for (int sIndex = start; sIndex < start + width; sIndex++)
	{
		if (!isRGBEqual(line.pixels[mIndex], this->pixels[sIndex]))
			return false;

		mIndex++;
	}

	return true;
}
bool Screenshot::operator!=(const Midline& line)
{
	return !(*this == line);
}


Screenshot& Screenshot::operator=(const Screenshot& other)
{
	height = other.height;
	width = other.width;
	length = other.length;
	pixels = other.pixels;
	return *this;
}

bool Screenshot::operator==(const Screenshot& other)
{
	for (int i = 0; i < length; i++)
	{
		if (!isRGBEqual(other.pixels[i], this->pixels[i]))
			return false;
	}
	return true;
}

bool Screenshot::operator!=(const Screenshot& other)
{
	return !(*this == other);
}


/*
		Midline Class Implementation Code
*/

Midline::Midline(Screenshot& sc)
{
	length = sc.width;
	pixels = new RGBQUAD[sc.width];

	int index = (sc.height / 2) * sc.width;
	int i = 0;

	for (int ii = index; ii < index + sc.width; ii++)
	{
		pixels[i] = sc.pixels[ii];
		i++;
	}
}

Midline::Midline(int sWidth, int sHeight, RGBQUAD* sPixels)
{
	length = sWidth;
	pixels = new RGBQUAD[sWidth];

	int index = (sHeight / 2) * sWidth;
	int i = 0;

	for (int ii = index; ii < index + sWidth; ii++)
	{
		pixels[i] = sPixels[ii];
		i++;
	}
}

Midline::~Midline()
{
	delete[] pixels;
}
