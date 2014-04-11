#include "Screenshot.h"

CScreenshot::CScreenshot()
{
	Reset();
}

//----------------------------------------------------------------------//

CScreenshot::~CScreenshot()
{
	Reset();
}

//----------------------------------------------------------------------//

void CScreenshot::Reset()
{
	BMI.Erase();
	Buffer.Free();
	ZeroMemory(&Info, sizeof(ScreenshotInfoStruct));
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

int GetBitsPerPixels(ScrFormat Format)
{
	switch(Format)
	{
	case scrf_32: return 32;
	case scrf_16: return 16;
	case scrf_8c: 
	case scrf_8g: return 8;
	case scrf_4:  return 4;
	case scrf_1:  return 1;
	}

	return 0;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CScreenshot::Take(ScrFormat Format)
{
	int x,y,w,h;
	HWND hDesktopWnd = GetScreenInfo(&x,&y,&w,&h);

	int bpp = GetBitsPerPixels(Format);
	
	FillScreenshotInfo(Format, w, h, bpp);
	BMI.Fill(w, h, bpp);
	Palette.Fill(BMI.Get(), Format, bpp);

	TakeScreenshot(hDesktopWnd, x,y,w,h);
}

//----------------------------------------------------------------------//

void CScreenshot::TakeScreenshot(HWND hDesktopWnd, int x, int y, int w, int h)
{
	// Get the desktop windows dc and create one for our bitmap
	HDC hdc   = GetDC(hDesktopWnd);
	HDC hdcex = CreateCompatibleDC(hdc);

	// Create the original hbitmap
	HBITMAP hbmp = CreateCompatibleBitmap(hdc, w,h);

	// Convert the original bitmap and get it's pixels data
	BYTE *pixels = NULL;
	HBITMAP hbmpex = CreateDIBSection(hdcex, BMI.Get(), DIB_RGB_COLORS, (void**)&pixels, NULL, 0);

	// Select the modified bitmap and blit the original into it
	SelectObject(hdcex, hbmpex);
	BitBlt(hdcex,0,0,w,h, hdc,x,y, SRCCOPY);
	
	// Draw the cursor...
	DrawScreenCursor(hdcex);

	// Allocate the buffer
	Buffer.Allocate(Info.BufferSize, TRUE);
	memcpy(Buffer.GetBuffer(), pixels, Info.BufferSize);

	// Cleanup
	DeleteObject(hbmpex);
	DeleteObject(hbmp);
	DeleteDC(hdcex);
	ReleaseDC(hDesktopWnd, hdc);
}

//----------------------------------------------------------------------//

void CScreenshot::DrawScreenCursor(HDC hDC)
{
#ifdef RAD_STUDIO_XE
	CURSORINFO CursorInfo;
	CursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&CursorInfo);

	static DWORD Version = WinVer.DetectWindowsVersion();
	//static HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);

	DWORD CursorWidth  = GetSystemMetrics(SM_CXCURSOR);
	DWORD CursorHeight = GetSystemMetrics(SM_CYCURSOR);

	POINT CursorPos;
	GetCursorPos(&CursorPos);
	
	// Needed for XP or older windows
	if(Version < _WIN_VISTA_){
		CursorPos.x -= CursorWidth  >> 2;
		CursorPos.y -= CursorHeight >> 2;
	}

	DrawIconEx(hDC, CursorPos.x, CursorPos.y, CursorInfo.hCursor, CursorWidth, CursorHeight, 0, NULL, DI_NORMAL);
#endif
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

HWND CScreenshot::GetScreenInfo(int *x, int *y, int *w, int *h)
{
	RECT r;
	HWND hDesktopWnd = GetDesktopWindow();
	GetWindowRect(hDesktopWnd, &r);

	*x = r.left;
	*y = r.top;
	*w = r.right  - r.left;
	*h = r.bottom - r.top;

	return hDesktopWnd;
}

//----------------------------------------------------------------------//

void CScreenshot::FillScreenshotInfo(ScrFormat Format, int w, int h, int bpp)
{
	Info.Format = Format;
	Info.Width  = w;
	Info.Height = h;
	Info.NumPixels = w * h;
	Info.BitsPerPixel = bpp;
	Info.BufferSize = BitmapHelper.CalcBufferSize(w, h, bpp);
}

//----------------------------------------------------------------------//

void CScreenshot::CreateEmptyFrame(ScrFormat Format, int w, int h, int bpp)
{
	FillScreenshotInfo(Format, w, h, bpp);
	Buffer.Allocate(Info.BufferSize, TRUE);
}

