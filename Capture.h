
#pragma once
#ifndef CAPTURE_H
#define CAPTURE_H

#include <Windows.h>
#include "Screenshot.h"

// Takes a screenshot and saves it inside a Screenshot class lal //
class Capture
{
	friend class Screenshot;
private:
	char* WindowName; //Handle name of window
	void release(HWND& hwnd, HDC& hdc, HDC& captureDC, HBITMAP& hBmp);

public:
	Capture(char* windowName);
	~Capture();

	bool isWindowRunning();
	bool screenshotGDI(Screenshot& screeny); //Obtains screenshot via win32. Outputs data into _LastFrame struct
	int getWidth();
	int getHeight();
	void switchToWindow();
	bool captureNewFrame(Screenshot& currentFrame);
};


#endif 
