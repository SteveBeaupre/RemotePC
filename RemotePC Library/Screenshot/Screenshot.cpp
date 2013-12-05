#include "Screenshot.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CScreenshot::CScreenshot()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
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
}

void CScreenshot::GetScreenSizeAndPos(HWND hDesktopWnd, int *l, int *t, int *w, int *h)
{
	RECT r;
	GetWindowRect(hDesktopWnd, &r);

	*l = r.right;
	*t = r.top;
	*w = r.right  - r.left;
	*h = r.bottom - r.top;
}
//-----------------------------------------------------------------------------
// Draw the cursor
//-----------------------------------------------------------------------------
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

int CScreenshot::Take(BOOL bShowCursor)
{
	CRawBuffer BitmapInfo;

	HWND hDesktopWnd = GetDesktopWindow();
	HDC hdc = GetDC(hDesktopWnd);

	int x,y,w,h;
	GetScreenSizeAndPos(hDesktopWnd, &x, &y, &w, &h);

	Info.Width  = w;
	Info.Height = h;
	Info.NumPixels = w * h;
	Info.BitsPerPixel  = 32;
	Info.BytesPerPixel = Info.BitsPerPixel / 8;
	Info.BufferSize    = Info.NumPixels * Info.BytesPerPixel;

/////////////////////////////////////////////////////////////////////////////////////////

	HDC     s_hdc   = CreateCompatibleDC(hdc);
	HBITMAP hbmp    = CreateCompatibleBitmap(hdc, w,h);
	HBITMAP ex_hbmp = (HBITMAP)SelectObject(s_hdc, hbmp);

	// Copy the screen image in our bitmap
	BitBlt(s_hdc, 0,0,w,h, hdc, x,y, SRCCOPY);

	if(bShowCursor)
		DrawCursor(s_hdc);
	
	ReleaseDC(hDesktopWnd, hdc);

/////////////////////////////////////////////////////////////////////////////////////////

	// Allocate the buffer
	Buffer.Allocate(Info.BufferSize, TRUE); 

	// Allocate a BITMAPINFO buffer 
	static const int BMISize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	BitmapInfo.Allocate(BMISize);
	
	LPBITMAPINFO lpbi = (LPBITMAPINFO)BitmapInfo.GetBuffer();
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	// De-select our hbmp
	SelectObject(s_hdc, ex_hbmp);

	// Get information about the screenshot image format
	GetDIBits(s_hdc, hbmp, 0, h, NULL, lpbi, DIB_RGB_COLORS);
	lpbi->bmiHeader.biCompression = BI_RGB;
	// Make sure it's gonna be extracted in 32 bits format
	lpbi->bmiHeader.biBitCount  = Info.BitsPerPixel;
	lpbi->bmiHeader.biSizeImage = Buffer.GetSize();

	// Extract the image in 32 bits format
	GetDIBits(s_hdc, hbmp, 0, h, Buffer.GetBuffer(), lpbi, DIB_RGB_COLORS);

	// Release the bitmap handles
	if(SelectObject(s_hdc, hbmp)){
		DeleteObject(hbmp);
		DeleteDC(s_hdc);
	}

	ReleaseDC(hDesktopWnd, hdc);

	return Buffer.GetSize();
}


