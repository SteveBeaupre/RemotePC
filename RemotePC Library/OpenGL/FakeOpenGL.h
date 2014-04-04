#pragma once
//----------------------------------------------------------------------//
#include "IOpenGL.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CFakeOpenGL : public IOpenGL {
public:
	CFakeOpenGL();
	~CFakeOpenGL();
private:
	HWND hWnd;
	TextureStruct Texture;
	RenderSettingsStruct RenderSettings;
	CLock SettingsThreadLock;
public:
	HWND GetHWND();
	Siz2 CalcWndSize();

	void SetVSync(int i){}
	
	void UpdateScrollBars(bool Stretched){}

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
	void DeleteTexture(){}
	void LoadTexture(BYTE *pTex, UINT w, UINT h, ScrFormat Format){}

	void Set2DMode(int w, int h){}
	
	void DrawQuad(float l, float t, float w, float h){}

	void Render(){}
	void RenderEmpty(){}
};
