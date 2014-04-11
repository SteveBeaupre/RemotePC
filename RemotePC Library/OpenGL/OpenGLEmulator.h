#pragma once
//----------------------------------------------------------------------//
#include "IOpenGL.h"
//----------------------------------------------------------------------//
#include "Screenshot.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC COpenGLEmulator : public IOpenGL {
public:
	COpenGLEmulator();
	~COpenGLEmulator();
private:
	CBMI BMI;
	CPalette Palette;
	CBitmapHelper BitmapHelper;
private:
	HWND hWnd;
	HDC  hdc, hdcex;
	HBITMAP hbitmap;
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
	OpenGLImgDim CalcImgDimentions(Siz2 WndSize);
private:
	bool ConvertTo32Bits(BYTE* pOutput, BYTE* pInput, ScrFormat Format, int bpp, int w, int h);
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
