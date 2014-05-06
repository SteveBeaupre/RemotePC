#include "OpenGLEmulator.h"

COpenGLEmulator::COpenGLEmulator()
{
	hWnd  = NULL;
	hdc   = NULL;
	hdcex = NULL;

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

bool COpenGLEmulator::ConvertTo32Bits(BYTE* pOutput, BYTE* pInput, ScrFormat Format, int bpp, int w, int h)
{
	int PixWritten = 0;

	static const int PitchDiv[6] = {4, 2, 1, 1, 1, 1};
	static const int NLoops[6] = {1, 1, 1, 1, 2, 8};

	int NumLoops = NLoops[Format];
	int Pitch = BitmapHelper.CalcPitchSize(w, h, bpp) / PitchDiv[Format];

	BYTE *Pix8  = pInput;
	BYTE *Pix32 = pOutput;

	BMI.Erase();
	BITMAPINFO* lpbi = BMI.Get();
	
	Palette.Fill(lpbi, Format, bpp);

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
						WORD Col = *pw;
						Pix32[0] = (BYTE)(((float)(Col & 0x001F) / 32.0f) * 255.0f);
						Pix32[1] = (BYTE)(((float)((Col >> 5) & 0x001F) / 32.0f) * 255.0f);
						Pix32[2] = (BYTE)(((float)((Col >> 10) & 0x001F) / 32.0f) * 255.0f);
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

	ConvertTo32Bits(ConvBuffer.GetBuffer(), pTex, Format, bpp, w, h);


	if(hbitmap)
		DeleteTexture();

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
	// Createa device context
	HDC hdcex = CreateCompatibleDC(hdc);

	// Select the bitmap and draw it (on the panel)
	SelectObject(hdcex, hbitmap);
	StretchBlt(hdc, l, (h+t)-1, w, -h, hdcex, 0, 0, Texture.Width, Texture.Height, SRCCOPY);

	//SelectObject(hdcex, OldBrush);
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

void COpenGLEmulator::RenderEmpty()
{
	if(!IsInitialized())
		return;

	Siz2 WndSize = CalcWndSize();
	HBRUSH NewBrush = CreateSolidBrush(RGB(0, 0, 0));

	SelectObject(hdc, NewBrush);
	Rectangle(hdc, 0, 0, WndSize.w, WndSize.h);
	DeleteObject(NewBrush);
}

//----------------------------------------------------------------------//

void COpenGLEmulator::Render()
{
	if(!IsInitialized())
		return;

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

