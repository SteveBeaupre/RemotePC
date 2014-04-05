#include "OpenGLEmulator.h"

COpenGLEmulator::COpenGLEmulator()
{
	hWnd  = NULL;
	hdc   = NULL;
	hdcex = NULL;

	// Allocate enough memory to hold a BITMAPINFO structure
	BMISize = sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD));
	bmi.Allocate(BMISize);
	lpbi = (BITMAPINFO*)bmi.GetBuffer();

	Shutdown();
}

COpenGLEmulator::~COpenGLEmulator()
{
	Shutdown();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void COpenGLEmulator::SetStretchedFlag(bool Stretched)
{
	SettingsThreadLock.Lock();
	RenderSettings.Stretch = Stretched;
	SettingsThreadLock.Unlock();

	UpdateScrollBars(Stretched);
}

//----------------------------------------------------------------------//

void COpenGLEmulator::SetShowFPSFlag(bool ShowFPS)
{
	SettingsThreadLock.Lock();
	RenderSettings.ShowFPS = ShowFPS;
	SettingsThreadLock.Unlock();
}

//----------------------------------------------------------------------//

bool COpenGLEmulator::GetStretchedFlag()
{
	SettingsThreadLock.Lock();
	bool Stretched = RenderSettings.Stretch;
	SettingsThreadLock.Unlock();

	return Stretched;
}

//----------------------------------------------------------------------//

bool COpenGLEmulator::GetShowFPSFlag()
{
	SettingsThreadLock.Lock();
	bool ShowFPS = RenderSettings.ShowFPS;
	SettingsThreadLock.Unlock();

	return ShowFPS;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

HWND COpenGLEmulator::GetHWND()
{
	return hWnd;
}

//----------------------------------------------------------------------//

Siz2 COpenGLEmulator::CalcWndSize()
{
	Siz2 s;
	ZeroMemory(&s, sizeof(Siz2));

	if(hWnd){
		RECT r;
		GetClientRect(hWnd, &r);

		s.w = r.right  - r.left;
		s.h = r.bottom - r.top;
	}

	return s;
}

//----------------------------------------------------------------------//

bool COpenGLEmulator::Initialize(HWND h)
{
	if(IsInitialized())
		Shutdown();

	hWnd = h;
	hdc  = GetDC(hWnd);

	ZeroMemory(&Texture, sizeof(TextureStruct));

	FPSTimer.Reset();

	return true;
}

//----------------------------------------------------------------------//

void COpenGLEmulator::Shutdown()
{
	if(IsInitialized()){

		if(hbitmap){
			DeleteObject(hbitmap);
			hbitmap = NULL;
		}

		if(hdc){
			ReleaseDC(hWnd, hdc);
			hdc = NULL;
		}

		hWnd = NULL;
	}

	DeleteTexture();
}

//----------------------------------------------------------------------//

bool COpenGLEmulator::IsInitialized()
{
	return hWnd != NULL;
}

//----------------------------------------------------------------------//

void COpenGLEmulator::UpdateScrollBars(bool Stretched)
{
	HWND h = hWnd;
	for(int i = 0; i < 3; i++)
		h = GetParent(h);

	if(h){
		switch(Stretched)
		{
		case true:  PostMessage(h, ON_UPDATE_SCROLLBARS_MSG, 0, 0); break;
		case false: PostMessage(h, ON_UPDATE_SCROLLBARS_MSG, Texture.Width, Texture.Height); break; 
		}
		
	}
}

//----------------------------------------------------------------------//

TextureStruct* COpenGLEmulator::GetTexture()
{
	return &Texture;
}

//----------------------------------------------------------------------//

int COpenGLEmulator::GetBitsPerPixels(ScrFormat Format)
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

void COpenGLEmulator::FillBMIHeader(int w, int h, int bpp)
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

void COpenGLEmulator::GenPalette(ScrFormat Format, int bpp)
{
	switch(bpp)
	{
	case 1: Gen1bitsGrayscalePalette(); break;
	case 4: Gen4bitsGrayscalePalette(); break;
	case 8: Gen8BitsPalette(Format == scrf_8g); break;
	}
}

//----------------------------------------------------------------------//

void COpenGLEmulator::Gen8BitsPalette(bool Grayscale)
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

void COpenGLEmulator::Gen8bitsGrayscalePalette()
{
	static const int ncols = 256;
	for(int i = 0; i < ncols; i++){
		lpbi->bmiColors[i].rgbRed   = i;
		lpbi->bmiColors[i].rgbGreen = i;
		lpbi->bmiColors[i].rgbBlue  = i;
	}
}

//----------------------------------------------------------------------//

void COpenGLEmulator::Gen8bitsBGR233Palette()
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

void COpenGLEmulator::Gen4bitsGrayscalePalette()
{
	static const int ncols = 16;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	BYTE _4_bits_index[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

	for(int i = 0; i < ncols; i++){
		BYTE Col = _4_bits_index[i];
		lpbi->bmiColors[i].rgbRed   = Col;
		lpbi->bmiColors[i].rgbGreen = Col;
		lpbi->bmiColors[i].rgbBlue  = Col;
	}
}

//---------------------------------------------------------------------------

void COpenGLEmulator::Gen1bitsGrayscalePalette()
{
	static const int ncols = 2;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	for(int i = 0; i < ncols; i++){
		BYTE Col = i == 0 ? 0 : 255;
		lpbi->bmiColors[i].rgbRed   = Col;
		lpbi->bmiColors[i].rgbGreen = Col;
		lpbi->bmiColors[i].rgbBlue  = Col;
	}
}

//----------------------------------------------------------------------//

bool COpenGLEmulator::ConvertBuffers(BYTE* pOutput, BYTE* pInput, ScrFormat Format, int bpp, int w, int h)
{
	int PixWritten = 0;

	static const int PitchDiv[6] = {4, 2, 1, 1, 1, 1};
	static const int NLoops[6] = {1, 1, 1, 1, 2, 8};

	int NumLoops = NLoops[Format];
	int Pitch = BitmapHelper.CalcPitchSize(w, h, bpp) / PitchDiv[Format];

	BYTE *Pix8  = pInput;
	BYTE *Pix32 = pOutput;

	GenPalette(Format, bpp);

	for(int y = 0; y < h; y++){

		int x = 0;
		for(int i = 0; i < Pitch; i++){

			for(int j = NumLoops-1; j >= 0; j--){

				if(x < w){

					if(PixWritten > w * h)
						return false;

					if(Format == scrf_32){
						memcpy(Pix32, Pix8, 4);
					} else if(Format == scrf_16){
						WORD *pw = (WORD*)Pix8;
						WORD w = *pw;
						Pix32[0] = (BYTE)(((float)(w & 0x001F) / 32.0f) * 255.0f);
						Pix32[1] = (BYTE)(((float)((w >> 5) & 0x001F) / 32.0f) * 255.0f);
						Pix32[2] = (BYTE)(((float)((w >> 10) & 0x001F) / 32.0f) * 255.0f);
					} else {
							int palIndx = 0;
							if(Format == scrf_8c || Format == scrf_8g){
								palIndx = *Pix8;
							} else if(Format == scrf_4){
								BYTE b = *Pix8;
								palIndx = j == 0 ? (b & 0x0F) : (b >> 4);
							} else if(Format == scrf_1){
								BYTE b = *Pix8;
								palIndx = (b >> j) & 0x01;
							}

							Pix32[0] = lpbi->bmiColors[palIndx].rgbRed;
							Pix32[1] = lpbi->bmiColors[palIndx].rgbGreen;
							Pix32[2] = lpbi->bmiColors[palIndx].rgbBlue;
					}

					Pix32 += 4;
					PixWritten++;
					x++;
				}
			}

			Pix8 += PitchDiv[Format];
		}
	}

	return true;
}

//----------------------------------------------------------------------//

void COpenGLEmulator::LoadTexture(BYTE *pTex, UINT w, UINT h, ScrFormat Format)
{
	if(!IsInitialized())
		return;
	
	if(hbitmap)
		DeleteTexture();

	Texture.Width  = w;
	Texture.Height = h;
	Texture.Format = Format;

	UpdateScrollBars(GetStretchedFlag());

	int bpp = GetBitsPerPixels(Format);
	int ConvBufSize = BitmapHelper.CalcBufferSize(w, h, 32);
	CRawBuffer ConvBuffer(ConvBufSize);

	// Convert the texture to 32 bits
	ConvertBuffers(ConvBuffer.GetBuffer(), pTex, Format, bpp, w, h);

	// Delete the previsous bitmap, if any
	if(hbitmap)
		DeleteTexture();
	// Create a 32-bit bitmap
	hbitmap = (HBITMAP)CreateBitmap(w, h, 1, 32, ConvBuffer.GetBuffer());
}

//----------------------------------------------------------------------//

void COpenGLEmulator::DeleteTexture()
{
	if(hbitmap){
		DeleteObject(hbitmap);
		hbitmap = NULL;
	}
	ZeroMemory(&Texture, sizeof(TextureStruct));
}

//----------------------------------------------------------------------//

void COpenGLEmulator::DrawFPS()
{
	char buf[64];
	ZeroMemory(buf, 64);
	sprintf(buf, "FPS: %.2f", FPSTimer.GetFPS());

	HDC hDC = GetDC(hWnd);
	TextOut(hDC, 15, 15, buf, strlen(buf));
	ReleaseDC(hWnd, hDC);
}

//----------------------------------------------------------------------//

void COpenGLEmulator::DrawQuad(OpenGLImgDim *dim)
{
	DrawQuad((int)dim->l, (int)dim->t, (int)dim->tw, (int)dim->th);
}

//----------------------------------------------------------------------//

void COpenGLEmulator::DrawQuad(int l, int t, int w, int h)
{
	Siz2 WndSize = CalcWndSize();

	// Createa device context
	HDC hdcex = CreateCompatibleDC(hdc);

	/*HBRUSH NewBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdcex, NewBrush);
	Rectangle(hdcex, 0, 0, WndSize.w, WndSize.h);

	BYTE Alpha = 0x26;
	SetPanelColor(hdc, 255, 0, 0);*/

	// Select the bitmap and draw it (on the panel)
	SelectObject(hdcex, hbitmap);

	StretchBlt(hdc, l, (h+t)-1, w, -h, hdcex, 0, 0, Texture.Width, Texture.Height, SRCCOPY);

	//SelectObject(hdcex, OldBrush);
	//DeleteObject(NewBrush);
	DeleteDC(hdcex);
}

//----------------------------------------------------------------------//

OpenGLImgDim COpenGLEmulator::CalcImgDimentions(Siz2 WndSize)
{
	OpenGLImgDim dim;
	dim.ww = (float)WndSize.w;
	dim.wh = (float)WndSize.h;
	dim.tw = (float)Texture.Width;
	dim.th = (float)Texture.Height;

	dim.l = dim.tw < dim.ww ? ((dim.ww - dim.tw) / 2.0f) : 0.0f;
	dim.t = dim.th < dim.wh ? (dim.wh - dim.th) - ((dim.wh - dim.th) / 2.0f) : dim.wh - dim.th;
	dim.vw = (int)(dim.ww < dim.tw ? dim.tw : dim.ww);
	dim.vh = (int)(dim.wh < dim.th ? dim.th : dim.wh);

	return dim;
}

//----------------------------------------------------------------------//

// Flicker... :(
void COpenGLEmulator::SetPanelColor(HDC hDC, BYTE r, BYTE g, BYTE b)
{
	Siz2 WndSize = CalcWndSize();
	HBRUSH NewBrush = CreateSolidBrush(RGB(r, g, b));

	SelectObject(hDC, NewBrush);
	Rectangle(hDC, 0, 0, WndSize.w, WndSize.h);
	DeleteObject(NewBrush);
}

//----------------------------------------------------------------------//

void COpenGLEmulator::RenderEmpty()
{
	if(!IsInitialized())
		return;

	SetPanelColor(hdc, 0, 0, 0);
}

//----------------------------------------------------------------------//

void COpenGLEmulator::Render()
{
	if(!IsInitialized())
		return;

	//BYTE Alpha = (BYTE)(0.15f * 255.0f);
	//SetPanelColor(Alpha, Alpha, Alpha);

	Siz2 WndSize = CalcWndSize();

	if(!GetStretchedFlag()){
		OpenGLImgDim dim = CalcImgDimentions(WndSize);
		DrawQuad(&dim);
	} else {
		DrawQuad(0, 0, WndSize.w, WndSize.h);
	}

	if(GetShowFPSFlag()){
		FPSTimer.Tick();
		DrawFPS();
	}
}

