#pragma once
//----------------------------------------------------------------------//
#include "IOpenGL.h"
//----------------------------------------------------------------------//

//#define _16_BITS_SCREENSHOT_

class COpenGL : public IOpenGL {
public:
	COpenGL();
	~COpenGL();
private:
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
private:
	bool SetupPixelFormatDescriptor(HDC hdc);
	bool CheckExtension(char *extName);
	bool CheckVersion(int MajorVerson, int MinorVersion);
	void DrawFPS();
private:
	HWND hWnd;
	HDC   hDC;
	HGLRC hRC;

	TextureStruct Texture;
	RenderSettingsStruct RenderSettings;
	CLock SettingsThreadLock;
private:
	CGameTimer FPSTimer;
	CBitmapFonts FPSFont;
	int FontTexBaseID;
public:
	HWND GetHWND();
	Siz2 CalcWndSize();

	void SetVSync(int i);
	
	void UpdateScrollBars(bool Stretched);

	void SetStretchedFlag(bool Stretched);
	void SetShowFPSFlag(bool ShowFPS);
	bool GetStretchedFlag();
	bool GetShowFPSFlag();

	bool Initialize(HWND h);
	void Shutdown();
	bool IsInitialized();

	TextureStruct* GetTexture();

	void LoadFonts();
	void DeleteFonts();

	void CreateTexture();
	void LoadTexture(BYTE *pTex, UINT w, UINT h, ScrFormat Format);
	void DeleteTexture();

	void Set2DMode(int w, int h);

	void DrawQuad(float l, float t, float w, float h);

	void Render();
	void RenderEmpty();
};
