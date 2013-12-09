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
	CLock SettingsThreadLock;
private:
	CGameTimer FPSTimer;
	CBitmapFonts FPSFont;
	int FontTexBaseID;

public:
	HWND GetHWND();
	Siz2 CalcWndSize();

	void SetVSync(int i);
	
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
	void LoadTexture(BYTE *pTex, UINT w, UINT h, UINT bpp, UINT format);
	void DeleteTexture();

	void Set2DMode();

	void DrawQuad();

	void Render();
};
