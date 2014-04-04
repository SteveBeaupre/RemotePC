#include "FakeOpenGL.h"

CFakeOpenGL::CFakeOpenGL()
{
	Shutdown();
}

CFakeOpenGL::~CFakeOpenGL()
{
	Shutdown();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CFakeOpenGL::SetStretchedFlag(bool Stretched)
{
	SettingsThreadLock.Lock();
	RenderSettings.Stretch = Stretched;
	SettingsThreadLock.Unlock();

	UpdateScrollBars(Stretched);
}

//----------------------------------------------------------------------//

void CFakeOpenGL::SetShowFPSFlag(bool ShowFPS)
{
	SettingsThreadLock.Lock();
	RenderSettings.ShowFPS = ShowFPS;
	SettingsThreadLock.Unlock();
}

//----------------------------------------------------------------------//

bool CFakeOpenGL::GetStretchedFlag()
{
	SettingsThreadLock.Lock();
	bool Stretched = RenderSettings.Stretch;
	SettingsThreadLock.Unlock();

	return Stretched;
}

//----------------------------------------------------------------------//

bool CFakeOpenGL::GetShowFPSFlag()
{
	SettingsThreadLock.Lock();
	bool ShowFPS = RenderSettings.ShowFPS;
	SettingsThreadLock.Unlock();

	return ShowFPS;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

HWND CFakeOpenGL::GetHWND()
{
	return hWnd;
}

Siz2 CFakeOpenGL::CalcWndSize()
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

bool CFakeOpenGL::Initialize(HWND h)
{
	// Save our windows handle
	hWnd = h;

	return true;
}

void CFakeOpenGL::Shutdown()
{
	hWnd = NULL;
	ZeroMemory(&Texture, sizeof(TextureStruct));
}

bool CFakeOpenGL::IsInitialized()
{
	return true;
}

TextureStruct* CFakeOpenGL::GetTexture()
{
	return &Texture;
}
