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
	CWinVersion WinVer;
	void DrawCursor(HDC hDC);
	void GetScreenSize(int *w, int *h);

	void ResetInfos();
	void ResetBuffer();

	void GetScreenSizeAndPos(HWND hDesktopWnd, int *l, int *t, int *w, int *h);
public:
	void Reset();
	int  Take(BOOL bShowCursor);
	void CreateEmpty(int Width, int Height, int BitsPerPixel);

	int GetWidth(){return Info.Width;}
	int GetHeight(){return Info.Height;}
	int GetNumPixels(){return Info.NumPixels;}
	int GetBitsPerPixel(){return Info.BitsPerPixel;}
	int GetBytesPerPixel(){return Info.BytesPerPixel;}
	int GetBufferSize(){return Info.BufferSize;}

	BYTE* GetBuffer(){return Buffer.GetBuffer();}
};
