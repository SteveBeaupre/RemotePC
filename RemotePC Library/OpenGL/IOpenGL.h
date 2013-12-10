#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
//----------------------------------------------------------------------//
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <gl\gl.h>
#include <gl\glu.h>
#include "glext.h"
//----------------------------------------------------------------------//
#include "BitmapFonts.h"
#include "Timer.h"
#include "Lock.h"
#include "WinMsg.h"
//----------------------------------------------------------------------//

// VSync Extension
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

struct vec2 {float x,y;};
struct vec3 {float x,y,z;};
struct vec4 {float x,y,z,w;};

struct Col3 {float r,g,b;};
struct Col4 {float r,g,b,a;};

struct Pix3 {BYTE r,g,b;};
struct Pix4 {BYTE r,g,b,a;};

struct Pos2 {int x,y;};
struct Siz2 {int w,h;};

struct RenderSettingsStruct {
	bool Stretch;
	bool ShowFPS;
};

struct TextureStruct {
	UINT Width;
	UINT Height;
	UINT BPP;

	UINT ID;
	UINT Format;

	UINT BufferSize;
};

class IOpenGL {
public:
	virtual HWND GetHWND() = 0;
	virtual Siz2 CalcWndSize() = 0;

	virtual void SetVSync(int i) = 0;
	
	virtual void SetStretchedFlag(bool Stretched) = 0;
	virtual void SetShowFPSFlag(bool ShowFPS) = 0;
	virtual bool GetStretchedFlag() = 0;
	virtual bool GetShowFPSFlag() = 0;

	virtual bool Initialize(HWND h) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsInitialized() = 0;

	virtual TextureStruct* GetTexture() = 0;

	virtual void LoadFonts() = 0;
	virtual void DeleteFonts() = 0;

	virtual void CreateTexture() = 0;
	virtual void DeleteTexture() = 0;
	virtual void LoadTexture(BYTE *pTex, UINT w, UINT h, UINT bpp, UINT format) = 0;

	virtual void Set2DMode(int w, int h) = 0;
	
	virtual void DrawQuad(float l, float t, float w, float h) = 0;

	virtual void Render() = 0;
	virtual void RenderEmpty() = 0;
};
