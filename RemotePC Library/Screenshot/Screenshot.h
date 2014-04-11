#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#include "IScreenshot.h"
//----------------------------------------------------------------------//
#include "BMI.h"
#include "Palette.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

int GetBitsPerPixels(ScrFormat Format);

class EXP_FUNC CScreenshot : public IScreenshot, private ScreenshotStruct {
public:
	CScreenshot();
	~CScreenshot();
private:
	CBMI BMI;
private:
	CWinVersion   WinVer;
	CBitmapHelper BitmapHelper;

	void DrawScreenCursor(HDC hDC);
	void TakeScreenshot(HWND hDesktopWnd, int x, int y, int w, int h);
	HWND GetScreenInfo(int *x, int *y, int *w, int *h);

	void FillScreenshotInfo(ScrFormat Format, int w, int h, int bpp);
	
	CPalette Palette;
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
