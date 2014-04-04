#pragma once
//----------------------------------------------------------------------//
#include "IOpenGL.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC COpenGL : public IOpenGL {
public:
	COpenGL();
	~COpenGL();
private:
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
private:
	void InitOpenGL();
	bool SetupPixelFormatDescriptor(HDC hdc);
	bool CheckExtension(char *extName);
	bool CheckVersion(int MajorVerson, int MinorVersion);
	void SetFormat(ScrFormat Format, int *pGLType, int *pGLFormat, int *pGLIntFormat);
	void SetVertex(float x, float y);
	void SetVertex(float x, float y, float tx, float ty);
	void DrawFPS();
	OpenGLImgDim CalcImgDimentions(Siz2 WndSize);
private:
	HDC   hDC;
	HGLRC hRC;
	HWND hWnd;
private:
	TextureStruct Texture;
	RenderSettingsStruct RenderSettings;
	CLock SettingsThreadLock;
private:
	CGameTimer FPSTimer;
	CBitmapFonts FPSFont;
	int FontTexBaseID;
public:
	bool Initialize(HWND h);
	void Shutdown();
	bool IsInitialized();

	HWND GetHWND();
	Siz2 CalcWndSize();

	void SetVSync(int i);
	
	void UpdateScrollBars(bool Stretched);

	void SetStretchedFlag(bool Stretched);
	void SetShowFPSFlag(bool ShowFPS);
	bool GetStretchedFlag();
	bool GetShowFPSFlag();

	TextureStruct* GetTexture();

	void LoadFonts();
	void DeleteFonts();

	void CreateTexture();
	void DeleteTexture();
	void LoadTexture(BYTE *pTex, UINT w, UINT h, ScrFormat Format);

	void Set2DMode(int w, int h);

	void DrawQuad(float l, float t, float w, float h);

	void Render();
	void RenderEmpty();
};
