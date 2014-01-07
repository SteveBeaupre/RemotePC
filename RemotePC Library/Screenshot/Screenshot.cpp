#include "Screenshot.h"

CScreenshot::CScreenshot()
{
	// Default bmi header size
	BMISize = sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD));
	bmi.Allocate(BMISize);
	lpbi = (BITMAPINFO*)bmi.GetBuffer();

	Reset();
}

CScreenshot::~CScreenshot()
{
	Reset();
}

void CScreenshot::ResetInfos()
{
	ZeroMemory(&Info, sizeof(ScreenshotInfoStruct));
}

void CScreenshot::ResetBuffer()
{
	Buffer.Free();
}


void CScreenshot::Reset()
{
	ResetInfos();
	ResetBuffer();
	bmi.Erase();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CScreenshot::DrawCursor(HDC hDC)
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

void CScreenshot::Take(ScrFormat Format)
{
	int x,y,w,h;
	HWND hDesktopWnd = GetScreenInfo(&x,&y,&w,&h);

	///////////////////////////////////////////////////////////////////////////////////////

	int bpp = 32;
	switch(Format)
	{
	case scrf_32: bpp = 32; break;
	case scrf_16: bpp = 16; break;
	case scrf_8c: 
	case scrf_8g: bpp = 8;  break;
	case scrf_4:  bpp = 4;  break;
	case scrf_1:  bpp = 1;  break;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	Info.Format = Format;
	Info.Width  = w;
	Info.Height = h;
	Info.NumPixels = w * h;
	Info.BitsPerPixel = bpp;
	Info.BufferSize = BitmapHelper.CalcBufferSize(w, h, bpp);
	
	///////////////////////////////////////////////////////////////////////////////////////

	// Fill the bmi header
	FillBMIHeader(w, h, bpp);

	// Gen low color depth bitmap's palette
	switch(bpp)
	{
	case 8: Gen8BitsPalette(Format == scrf_8g); break;
	case 4: Gen4bitsGrayscalePalette(); break;
	case 1: Gen1bitsGrayscalePalette(); break;
	}
		

	///////////////////////////////////////////////////////////////////////////////////////

	// Get the desktop windows dc and create one for our bitmap
	HDC hdc   = GetDC(hDesktopWnd);
	HDC hdcex = CreateCompatibleDC(hdc);

	// Create the original hbitmap
	HBITMAP hbmp = CreateCompatibleBitmap(hdc, w,h);

	// Convert the original bitmap and get it's pixels data
	BYTE *pixels = NULL;
	HBITMAP hbmpex = CreateDIBSection(hdcex, lpbi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);

	// Select the modified bitmap and blit the original into it
	SelectObject(hdcex, hbmpex);
	BitBlt(hdcex,0,0,w,h, hdc,x,y, SRCCOPY);
	
	// Draw the cursor...
	DrawCursor(hdcex);

	// Allocate the buffer
	Buffer.Allocate(Info.BufferSize, TRUE);
	memcpy(Buffer.GetBuffer(), pixels, Info.BufferSize);

	///////////////////////////////////////////////////////////////////////////////////////

	// Cleanup
	DeleteObject(hbmpex);
	DeleteObject(hbmp);
	DeleteDC(hdcex);
	ReleaseDC(hDesktopWnd, hdc);
}

//----------------------------------------------------------------------//

void CScreenshot::FillBMIHeader(int w, int h, int bpp)
{
	bmi.Erase();
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbi->bmiHeader.biWidth = w;
	lpbi->bmiHeader.biHeight = h;
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = bpp;
	lpbi->bmiHeader.biCompression = BI_RGB;
	lpbi->bmiHeader.biSizeImage = BitmapHelper.CalcBufferSize(w, h, bpp);
	lpbi->bmiHeader.biXPelsPerMeter = 1000000;
	lpbi->bmiHeader.biYPelsPerMeter = 1000000;
}

//----------------------------------------------------------------------//

void CScreenshot::Gen8BitsPalette(bool Grayscale)
{
	static const int ncols = 256;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	// Generate the palette
	switch(Grayscale)
	{
	case false: Gen8bitsBGR233Palette();    break;
	case true:  Gen8bitsGrayscalePalette(); break;
	}
}

//----------------------------------------------------------------------//

void CScreenshot::Gen8bitsGrayscalePalette()
{
	static const int ncols = 256;
	for(int i = 0; i < ncols; i++){
		lpbi->bmiColors[i].rgbRed   = i;
		lpbi->bmiColors[i].rgbGreen = i;
		lpbi->bmiColors[i].rgbBlue  = i;
	}
}

//----------------------------------------------------------------------//

void CScreenshot::Gen8bitsBGR233Palette()
{
	static const int ncols = 256;
	static const BYTE _2_bits_index[4] = {0x00, 0x55, 0xAA, 0xFF};
	static const BYTE _3_bits_index[8] = {0x00, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF};

	for(int i = 0; i < ncols; i++){

		int ri = (i & 0x07);
		int gi = (i & 0x38) >> 3;
		int bi = (i & 0xC0) >> 6;

		lpbi->bmiColors[i].rgbRed   = _3_bits_index[ri];
		lpbi->bmiColors[i].rgbGreen = _3_bits_index[gi];
		lpbi->bmiColors[i].rgbBlue  = _2_bits_index[bi];
	}
}

//---------------------------------------------------------------------------

void CScreenshot::Gen4bitsGrayscalePalette()
{
	static const int ncols = 16;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	BYTE _4_bits_index[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

	for(int i = 0; i < ncols; i++){
		lpbi->bmiColors[i].rgbRed   = _4_bits_index[i];
		lpbi->bmiColors[i].rgbGreen = _4_bits_index[i];
		lpbi->bmiColors[i].rgbBlue  = _4_bits_index[i];
	}
}

//---------------------------------------------------------------------------

void CScreenshot::Gen1bitsGrayscalePalette()
{
	static const int ncols = 2;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	for(int i = 0; i < ncols; i++){
		lpbi->bmiColors[i].rgbRed   = i == 0 ? 0 : 255;
		lpbi->bmiColors[i].rgbGreen = i == 0 ? 0 : 255;
		lpbi->bmiColors[i].rgbBlue  = i == 0 ? 0 : 255;
	}
}

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

void CScreenshot::CreateEmpty(ScrFormat Format, int Width, int Height, int BitsPerPixel)
{
	Info.Format = Format;
	Info.Width  = Width;
	Info.Height = Height;
	Info.NumPixels = Width * Height;
	Info.BitsPerPixel = BitsPerPixel;
	Info.BufferSize = BitmapHelper.CalcBufferSize(Width, Height, BitsPerPixel);
	Buffer.Allocate(Info.BufferSize, TRUE);
}

