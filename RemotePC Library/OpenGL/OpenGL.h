#pragma once
//----------------------------------------------------------------------//
#include "IOpenGL.h"
//----------------------------------------------------------------------//

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
private:
	CGameTimer FPSTimer;
	CBitmapFonts FPSFont;
	int FontTexBaseID;

public:
	HWND GetHWND();
	Siz2 CalcWndSize();

	void SetVSync(int i);
	
	void SetSettings(bool Stretch, bool ShowFPS);
	RenderSettingsStruct* GetSettings();

	bool Initialize(HWND h);
	void Shutdown();
	bool IsInitialized();

	TextureStruct* GetTexture();

	void LoadFonts();
	void DeleteFonts();

	void CreateTexture();
	void LoadTexture(BYTE *pTex, UINT w, UINT h, UINT bpp, UINT format);
	void DeleteTexture();

	void Set2DMode();

	void DrawQuad();

	void Render();
};
