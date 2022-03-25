#pragma once

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <Windows.h>
#include <iostream>
#include <vector>
#include <algorithm> //sort


struct myRGB
{
	BYTE r, g, b;
	myRGB(BYTE R, BYTE G, BYTE B)
	{
		r = R;
		g = G;
		b = B;
	}
};

class Midline
{
	friend class Screenshot;
private:
	int length;
public:
	Midline(Screenshot& sc);
	Midline(int sWidth, int sHeight, RGBQUAD* sPixels);
	~Midline();
	RGBQUAD* pixels = NULL;
};


class Screenshot
{
	friend class Capture;
	friend class Midline;
private:
	const bool DRAW = false; //Draws filtered image to console
	int length; //IMP: Needs to be defined as there is no-way to find length of *pixels just from pointer to 1st element
	int width, height;
	RGBQUAD* pixels = NULL; //IMP: Uses GetDIBits to obtain RGBQUADs => image is inverted. Visualise via drawScreenshot(false)

	bool isPoopYellow(RGBQUAD& pixel);
	bool isRGBEqual(RGBQUAD& r1, RGBQUAD& r2);

public:
	~Screenshot();

	void FreeMemory(); //Do not handle mem on destruction since we sometimes want mem region to persist

	bool triggerBot();

	Screenshot& operator=(const Screenshot& other); //Does not copy RGBQUAD* put points to it only!!!
	bool operator==(const Midline& line); //Faster then comparing whole screenshot. Takes 0.000ms
	bool operator!=(const Midline& line); // !^
	bool operator==(const Screenshot& other); //Slower than comparing only midline. Takes 0.004ms
	bool operator!=(const Screenshot& other); // !^

};


#endif // !SCREENSHOT_H