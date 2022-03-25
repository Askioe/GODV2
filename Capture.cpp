

#include "Capture.h"

Capture::Capture(char* windowName)
{
	WindowName = windowName;
}

Capture::~Capture()
{
}

bool Capture::isWindowRunning()
{
	HWND hwnd = FindWindowA(0, WindowName);
	return (hwnd != NULL);
}

int Capture::getWidth() {
	HWND hwnd = FindWindowA(0, WindowName);
	if (hwnd == NULL) return 0;

	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	return rekt.right - rekt.left;
}

int Capture::getHeight() {
	HWND hwnd = FindWindowA(0, WindowName);
	if (hwnd == NULL) return 0;

	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	return rekt.bottom - rekt.top;
}

void Capture::switchToWindow() {
	HWND hwnd = FindWindowA(0, WindowName);
	if (hwnd != NULL) SwitchToThisWindow(hwnd, false);
}

/* Captures a screenshot and saves it into screeny which is passed by reference
   Return type T/F indicates whether method was successful or not. */
bool Capture::screenshotGDI(Screenshot& screeny)
{
	HWND hwnd = FindWindowA(0, WindowName);
	if (hwnd == NULL)
	{
		std::cout << "ERROR: Grand Theft Auto V HWND not found!" << std::endl;
		return false;
	}

	int sWidth = GetSystemMetrics(SM_CXSCREEN);
	int sHeight = GetSystemMetrics(SM_CYSCREEN);

	HDC hdc = GetDC(0);
	HDC captureDC = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, sWidth, sHeight);
	HGDIOBJ hOld = SelectObject(captureDC, hBmp);

	if (!BitBlt(captureDC, 0, 0, sWidth, sHeight, hdc, 0, 0, SRCCOPY | CAPTUREBLT))
	{
		std::cout << "ERROR: bit-block transfer failed!" << std::endl;
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}

	SelectObject(captureDC, hBmp);

	BITMAPINFO bmpInfo = { 0 };
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	if (!GetDIBits(hdc, hBmp, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS)) //get bmpInfo
	{
		std::cout << "ERROR: Failed to get Bitmap Info." << std::endl;
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	screeny.FreeMemory();

	int pixNo = bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biHeight;
	screeny.pixels = new RGBQUAD[pixNo];
	if (!screeny.pixels)
	{
		std::cout << "ERROR: Failed allocating RGBQUAD[" << pixNo << "]" << std::endl;
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}

	if (!GetDIBits(hdc, hBmp, 0, bmpInfo.bmiHeader.biHeight, (LPVOID)screeny.pixels, &bmpInfo, DIB_RGB_COLORS))
	{
		std::cout << "ERROR: Getting the bitmap buffer!" << std::endl;
		screeny.FreeMemory();
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}

	screeny.width = bmpInfo.bmiHeader.biWidth;
	screeny.height = bmpInfo.bmiHeader.biHeight;
	screeny.length = pixNo;

	release(hwnd, hdc, captureDC, hBmp);
	return true;
}

// Memory management for screenshot function above
void Capture::release(HWND& hwnd, HDC& hdc, HDC& captureDC, HBITMAP& hBmp)
{
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBmp);
	DeleteDC(captureDC);
	DeleteDC(hdc);
}

// Pauses until next frame is found 
bool Capture::captureNewFrame(Screenshot& currentFrame)
{
	if (!currentFrame.pixels)
		return screenshotGDI(currentFrame);

	Midline prevFrameMidline(currentFrame);
	bool newFrameLoaded = false;
	while (!newFrameLoaded) //Wait till new frame loads!
	{
		bool captured = screenshotGDI(currentFrame);
		if (!captured)
			return false;

		if (currentFrame != prevFrameMidline)
		{
			newFrameLoaded = true; //not necessary
			return true;
		}
		else
			Sleep(10);
	}

	return true;
}