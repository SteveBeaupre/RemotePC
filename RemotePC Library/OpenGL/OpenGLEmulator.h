#pragma once
//----------------------------------------------------------------------//
#include "IOpenGL.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC COpenGLEmulator : public IOpenGL {
public:
	COpenGLEmulator();
	~COpenGLEmulator();
private:
	int BMISize;
	CRawBuffer bmi;
	BITMAPINFO *lpbi;
	CBitmapHelper BitmapHelper;
private:
	HWND hWnd;
	HDC  hdc, hdcex;
	HBITMAP hbitmap;
	void FillBMIHeader(int w, int h, int bpp);
private:
	TextureStruct Texture;
	RenderSettingsStruct RenderSettings;
	CLock SettingsThreadLock;
private:
	CGameTimer FPSTimer;
	void DrawFPS();
private:
	void Set2DMode(int w, int h){}
	void DrawQuad(OpenGLImgDim *dim);
	void DrawQuad(int l, int t, int w, int h);
	int  GetBitsPerPixels(ScrFormat Format);
	OpenGLImgDim CalcImgDimentions(Siz2 WndSize);
private:
	void GenPalette(ScrFormat Format, int bpp);
	void Gen8BitsPalette(bool Grayscale);
	void Gen8bitsBGR233Palette();
	void Gen8bitsGrayscalePalette();
	void Gen4bitsGrayscalePalette();
	void Gen1bitsGrayscalePalette();
	bool ConvertBuffers(BYTE* pOutput, BYTE* pInput, ScrFormat Format, int bpp, int w, int h);
public:
	HWND GetHWND();
	Siz2 CalcWndSize();

	void SetVSync(int i){}
	
	void UpdateScrollBars(bool Stretched);

	void SetStretchedFlag(bool Stretched);
	void SetShowFPSFlag(bool ShowFPS);
	bool GetStretchedFlag();
	bool GetShowFPSFlag();

	bool Initialize(HWND h);
	void Shutdown();
	bool IsInitialized();

	TextureStruct* GetTexture();

	void LoadFonts(){}
	void DeleteFonts(){}

	void CreateTexture(){}
	void DeleteTexture();
	void LoadTexture(BYTE *pTex, UINT w, UINT h, ScrFormat Format);

	void Render();
	void RenderEmpty();
};
