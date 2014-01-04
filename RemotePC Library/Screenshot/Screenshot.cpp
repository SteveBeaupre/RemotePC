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

	// Generate a color palette if in bmi mode
	int bpp = 32;
	switch(Format)
	{
	case scrf_32: bpp = 32; break;
	case scrf_16: bpp = 16; break;
	case scrf_8c: 
	case scrf_8g: bpp = 8;  break;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	Info.Format = Format;
	Info.Width  = w;
	Info.Height = h;
	Info.NumPixels = w * h;
	Info.BitsPerPixel  = bpp;
	Info.BytesPerPixel = bpp / 8;
	Info.BufferSize = w * h * Info.BytesPerPixel;
	
	///////////////////////////////////////////////////////////////////////////////////////

	// Fill it
	FillBMIHeader(w, h, bpp);

	if(bpp == 8)
		GenPalette(Format == scrf_8g);

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

	// Ca;culate line width and number of padding bytes
	int LineWidth = w * (bpp / 8);
	int Padding = 4 - (LineWidth % 4);
	if(Padding == 4)
		Padding = 0;

	// Copy to buffer
	if(Padding == 0){
		memcpy(Buffer.GetBuffer(), pixels, Info.BufferSize);
	} else {
		int BufIndx = 0;
		int PixIndx = 0;

		for(int y = 0; y < h; y++){
			memcpy(Buffer.GetBuffer(BufIndx), &pixels[PixIndx], LineWidth);
			BufIndx += LineWidth;
			PixIndx += LineWidth + Padding;
		}
	}

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
	lpbi->bmiHeader.biSizeImage = w * h * (bpp / 8);
	lpbi->bmiHeader.biXPelsPerMeter = 1000000;
	lpbi->bmiHeader.biYPelsPerMeter = 1000000;
}

//----------------------------------------------------------------------//

void CScreenshot::GenPalette(bool Grayscale)
{
	// 8-bits color palette stuff...
	int ncols = 256;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	// Generate the palette
	switch(Grayscale)
	{
	case false: CreateBGR233Palette(ncols);    break;
	case true:  CreateGrayscalePalette(ncols); break;
	}
}

//----------------------------------------------------------------------//

void CScreenshot::CreateGrayscalePalette(int ncols)
{
	for(int i = 0; i < ncols; i++){
		lpbi->bmiColors[i].rgbRed   = i;
		lpbi->bmiColors[i].rgbGreen = i;
		lpbi->bmiColors[i].rgbBlue  = i;
	}
}

//----------------------------------------------------------------------//

void CScreenshot::CreateBGR233Palette(int ncols)
{
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
	Info.BytesPerPixel = Info.BitsPerPixel / 8;
	Info.BufferSize = Info.NumPixels * Info.BytesPerPixel;
	Buffer.Allocate(Info.BufferSize, TRUE);
}

