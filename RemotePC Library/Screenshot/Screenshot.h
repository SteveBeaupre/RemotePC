#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#include "IScreenshot.h"
//----------------------------------------------------------------------//

//#define _16_BITS_SCREENSHOT_

class CScreenshot : public IScreenshot, private ScreenshotStruct { 
public:
	CScreenshot();
	~CScreenshot();
private:
	int BMISize;
	CRawBuffer bmi;
	BITMAPINFO *lpbi;
private:
	CWinVersion WinVer;
	void DrawCursor(HDC hDC);
	void GetScreenSize(int *w, int *h);

	void ResetInfos();
	void ResetBuffer();

	void FillBMIHeader(int w, int h, int bpp);
	void GenPalette(bool Grayscale);
	void CreateGrayscalePalette(int ncols);
	void CreateBGR233Palette(int ncols);
	HWND GetScreenInfo(int *x, int *y, int *w, int *h);
public:
	void Reset();
	void Take(ScrFormat Format);
	void CreateEmpty(ScrFormat Format, int Width, int Height, int BitsPerPixel);

	int GetWidth(){return Info.Width;}
	int GetHeight(){return Info.Height;}
	int GetNumPixels(){return Info.NumPixels;}
	int GetBitsPerPixel(){return Info.BitsPerPixel;}
	int GetBytesPerPixel(){return Info.BytesPerPixel;}
	int GetBufferSize(){return Info.BufferSize;}

	ScrFormat GetFormat(){return Info.Format;}

	BYTE* GetBuffer(){return Buffer.GetBuffer();}
};
