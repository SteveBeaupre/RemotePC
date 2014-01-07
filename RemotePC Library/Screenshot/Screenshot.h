#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#include "IScreenshot.h"
//----------------------------------------------------------------------//

class CScreenshot : public IScreenshot, private ScreenshotStruct { 
public:
	CScreenshot();
	~CScreenshot();
private:
	int BMISize;
	CRawBuffer bmi;
	BITMAPINFO *lpbi;
private:
	CWinVersion   WinVer;
	CBitmapHelper BitmapHelper;

	void DrawCursor(HDC hDC);
	HWND GetScreenInfo(int *x, int *y, int *w, int *h);

	void ResetInfos();
	void ResetBuffer();

	void FillBMIHeader(int w, int h, int bpp);
	
	void Gen8BitsPalette(bool Grayscale);
	void Gen8bitsBGR233Palette();
	void Gen8bitsGrayscalePalette();
	void Gen4bitsGrayscalePalette();
	void Gen1bitsGrayscalePalette();
public:
	void Reset();
	void Take(ScrFormat Format);
	void CreateEmpty(ScrFormat Format, int Width, int Height, int BitsPerPixel);

	int GetWidth(){return Info.Width;}
	int GetHeight(){return Info.Height;}
	int GetNumPixels(){return Info.NumPixels;}
	int GetBitsPerPixel(){return Info.BitsPerPixel;}
	int GetBufferSize(){return Info.BufferSize;}

	ScrFormat GetFormat(){return Info.Format;}

	BYTE* GetBuffer(){return Buffer.GetBuffer();}
};
