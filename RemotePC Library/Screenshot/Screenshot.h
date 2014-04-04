#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#include "IScreenshot.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CScreenshot : public IScreenshot, private ScreenshotStruct {
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

	void DrawScreenCursor(HDC hDC);
	void TakeScreenshot(HWND hDesktopWnd, int x, int y, int w, int h);
	HWND GetScreenInfo(int *x, int *y, int *w, int *h);

	int  GetBitsPerPixels(ScrFormat Format);
	void FillScreenshotInfo(ScrFormat Format, int w, int h, int bpp);
	void FillBMIHeader(int w, int h, int bpp);
	
	void GenPalette(ScrFormat Format, int bpp);
	void Gen8BitsPalette(bool Grayscale);
	void Gen8bitsBGR233Palette();
	void Gen8bitsGrayscalePalette();
	void Gen4bitsGrayscalePalette();
	void Gen1bitsGrayscalePalette();
public:
	void Reset();
	void Take(ScrFormat Format);
	void CreateEmptyFrame(ScrFormat Format, int w, int h, int bpp);

	int GetWidth(){return Info.Width;}
	int GetHeight(){return Info.Height;}
	int GetNumPixels(){return Info.NumPixels;}
	int GetBitsPerPixel(){return Info.BitsPerPixel;}
	int GetBufferSize(){return Info.BufferSize;}

	ScrFormat GetFormat(){return Info.Format;}

	BYTE* GetBuffer(){return Buffer.GetBuffer();}
};
