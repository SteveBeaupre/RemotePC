#include "OpenGL.h"

COpenGL::COpenGL()
{
	wglSwapIntervalEXT = NULL;

	hWnd = NULL;
	hRC  = NULL;
	hDC  = NULL;

	FontTexBaseID = 0;

	ZeroMemory(&Texture, sizeof(TextureStruct));
	
	RenderSettings.Stretch = false;
	RenderSettings.ShowFPS = true;
}

COpenGL::~COpenGL()
{
	Shutdown();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

HWND COpenGL::GetHWND()
{
	return hWnd;
}

//----------------------------------------------------------------------//

Siz2 COpenGL::CalcWndSize()
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
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void COpenGL::UpdateScrollBars(bool Stretched)
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

void COpenGL::SetStretchedFlag(bool Stretched)
{
	SettingsThreadLock.Lock();
	RenderSettings.Stretch = Stretched;
	SettingsThreadLock.Unlock();

	UpdateScrollBars(Stretched);
}

//----------------------------------------------------------------------//

void COpenGL::SetShowFPSFlag(bool ShowFPS)
{
	SettingsThreadLock.Lock();
	RenderSettings.ShowFPS = ShowFPS;
	SettingsThreadLock.Unlock();
}

//----------------------------------------------------------------------//

bool COpenGL::GetStretchedFlag()
{
	SettingsThreadLock.Lock();
	bool Stretched = RenderSettings.Stretch;
	SettingsThreadLock.Unlock();

	return Stretched;
}

//----------------------------------------------------------------------//

bool COpenGL::GetShowFPSFlag()
{
	SettingsThreadLock.Lock();
	bool ShowFPS = RenderSettings.ShowFPS;
	SettingsThreadLock.Unlock();

	return ShowFPS;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void COpenGL::SetVSync(int i)
{
	if(IsInitialized() && wglSwapIntervalEXT){
		wglSwapIntervalEXT(i);
	}
}

//----------------------------------------------------------------------//

bool COpenGL::CheckExtension(char *extName)
{
	// Get the list of supported extensions
	char *extList = (char*)glGetString(GL_EXTENSIONS);

	// Check if everything's fine...
	if(!extName || !extList)
		return false;

	// Loop throught all extensions
	while(*extList){
		
		// Find the length of the current extension
		UINT ExtLen = (int)strcspn(extList, " ");
		
		// If we've found the extension, return true
		if(strlen(extName) == ExtLen && strncmp(extName, extList, ExtLen) == 0)
			return true;

		// Move to the next extension
		extList += ExtLen + 1;
	}

	// Extention not found...
	return false;
}

//----------------------------------------------------------------------//

bool COpenGL::CheckVersion(int MajorVerson, int MinorVersion)
{
	// Get the list of supported extensions
	char *Ver = (char*)glGetString(GL_VERSION);

	// Tell if the given version number is supported
	return (Ver[0]-48 > MajorVerson || (Ver[0]-48 == MajorVerson && Ver[2]-48 >= MinorVersion));
}

//----------------------------------------------------------------------//

bool COpenGL::SetupPixelFormatDescriptor(HDC hdc)
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGBA pixel type
    pfd.cColorBits = 32;								// How many color bit?
    pfd.cDepthBits = 32;								// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We'll use no stencil buffer
    pfd.cAlphaBits = 8;	                                // 1 Alpha byte
	
	// This gets us a pixel format that best matches the one passed in from the device
    if((pixelformat = ChoosePixelFormat(hdc, &pfd)) == false){ 
		MessageBoxA(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	// This sets the pixel format that we extracted from above
	if(SetPixelFormat(hdc, pixelformat, &pfd) == false){
		MessageBoxA(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE; 
    } 
 
    return true;
}

//----------------------------------------------------------------------//

bool COpenGL::Initialize(HWND h)
{
	// Return false if we're already initialized
	if(IsInitialized())
		return false;

	// Save our windows handle
	hWnd = h;
	// Get the hDC with the GetDC api and the previous handle
	hDC = GetDC(hWnd);

	// This sets our pixel format/information
	if(!SetupPixelFormatDescriptor(hDC))
		return false;

	// This creates a rendering context from our hdc
	hRC = wglCreateContext(hDC);
	// Make the current hRC the current rendering context
	wglMakeCurrent(hDC, hRC);	                        

	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Black Background				
	glColor3f(1.0f, 1.0f, 1.0f);          // Current Color				        
	///////////////////////////////////////////////////////////////////////////////////////////
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_LINE_SMOOTH);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);			// Really Nice Perspective Calculations
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);				// Really Nice Perspective Calculations
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);			// Really Nice Perspective Calculations
	///////////////////////////////////////////////////////////////////////////////////////////
	glDisable(GL_LIGHTING);   // Disable depth testing
	glDisable(GL_DEPTH_TEST); // Disable lighting  	
	glEnable(GL_TEXTURE_2D);  // Enable texture mapping
	glDisable(GL_CULL_FACE);  // Disable face culling
	///////////////////////////////////////////////////////////////////////////////////////////
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Enable textures of any width to be loaded
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	// Enable vsync, if supported
	if(CheckExtension("WGL_EXT_swap_control")){
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT(1);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	LoadFonts();
	FPSTimer.Reset();

	return true;
}

//----------------------------------------------------------------------//

void COpenGL::Shutdown()
{
	if(IsInitialized()){
		DeleteTexture();
		DeleteFonts();
		if(hRC){wglDeleteContext(hRC);}
		if(hDC){ReleaseDC(hWnd, hDC);}
	}

	hWnd = NULL;
	hDC  = NULL;
	hRC  = NULL;
}

//----------------------------------------------------------------------//

bool COpenGL::IsInitialized()
{
	return hRC != NULL;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void COpenGL::LoadFonts()
{
	if(IsInitialized())
		FPSFont.Build(hDC, "Arial", &FontTexBaseID, 14, 15, 15, 255, 255, 0);
}

//----------------------------------------------------------------------//

void COpenGL::DeleteFonts()
{
	if(IsInitialized())
		FPSFont.Free();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

TextureStruct* COpenGL::GetTexture()
{
	return &Texture;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void COpenGL::CreateTexture()
{
	if(IsInitialized() && Texture.ID == 0){
		glGenTextures(1, &Texture.ID);
		glBindTexture(GL_TEXTURE_2D, Texture.ID);
		// Tell OpenGL the quality of our texture map.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

//----------------------------------------------------------------------//

void COpenGL::LoadTexture(BYTE *pTex, UINT w, UINT h, UINT bpp, UINT format)
{
	if(!IsInitialized())
		return;
	
	if(Texture.ID == 0){
		CreateTexture();
		goto LoadTexJmp;
	}
	
	if(Texture.ID != 0){
		glBindTexture(GL_TEXTURE_2D, Texture.ID);
		
		if(Texture.Width != w || Texture.Height != h || Texture.BPP != bpp || Texture.Format != format){
			
			DeleteTexture();
			CreateTexture();

			LoadTexJmp:
			Texture.Width = w;
			Texture.Height = h;
			Texture.BPP = bpp;
			Texture.Format = format;

			UpdateScrollBars(GetStretchedFlag());

			glTexImage2D(GL_TEXTURE_2D, 0, Texture.BPP, Texture.Width, Texture.Height, 0, Texture.Format, GL_UNSIGNED_BYTE, pTex);
		} else {
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, Texture.Width, Texture.Height, Texture.Format, GL_UNSIGNED_BYTE, pTex);
		}
	}
}

//----------------------------------------------------------------------//

void COpenGL::DeleteTexture()
{
	if(IsInitialized() && Texture.ID != 0){
		glDeleteTextures(1, &Texture.ID);
		ZeroMemory(&Texture, sizeof(TextureStruct));
	}
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void COpenGL::Set2DMode(int w, int h)
{
	if(!IsInitialized())
		return;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW); 
}

//----------------------------------------------------------------------//

void COpenGL::DrawQuad(float l, float t, float w, float h)
{
	if(!IsInitialized())
		return;

	glColor3f(1.0f, 1.0f, 1.0f);

	float r = l+w;
	float b = t+h;

	if(Texture.ID != 0){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture.ID);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(l, b);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(r, b);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(r, t);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(l, t);
		glEnd();
	} else {
		glDisable(GL_TEXTURE_2D);

		glColor3f(0.15f, 0.15f, 0.15f);
		glBegin(GL_QUADS);
			glVertex2f(l, b);
			glVertex2f(r, b);
			glVertex2f(r, t);
			glVertex2f(l, t);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

//----------------------------------------------------------------------//

void COpenGL::DrawFPS()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f, 0.0f, 0.0f, 0.666f);

	Siz2 s = CalcWndSize();

	float l = 0.0f;
	float r = 100.0f;
	float t = (float)s.h - 20.0f;
	float b = (float)s.h;

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glVertex2f(l, t);
		glVertex2f(r, t);
		glVertex2f(r, b);
		glVertex2f(l, b);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	glDisable(GL_BLEND);

	FPSFont.SetPosition(15, s.h-15);
	FPSFont.glPrintArg("FPS: %.2f", FPSTimer.GetFPS());
}

//----------------------------------------------------------------------//

void COpenGL::Render()
{
	if(!IsInitialized())
		return;

	glClearColor(0.15f, 0.15f, 0.15f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	Siz2 WndSize = CalcWndSize();

	if(!GetStretchedFlag()){
		float ww = (float)WndSize.w;
		float wh = (float)WndSize.h;
		float tw = (float)Texture.Width;
		float th = (float)Texture.Height;

		float l = tw < ww ? ((ww - tw) / 2.0f) : 0.0f;
		float t = th < wh ? (wh - th) - ((wh - th) / 2.0f) : wh - th;
		int vw = (int)(ww < tw ? tw : ww);
		int vh = (int)(wh < th ? th : wh);

		Set2DMode(vw, vh);
		DrawQuad(l, t, tw, th);
	} else {
		Set2DMode(WndSize.w, WndSize.h);
		DrawQuad(0.0f, 0.0f, (float)WndSize.w, (float)WndSize.h);
	}

	if(GetShowFPSFlag()){
		FPSTimer.Tick();
		DrawFPS();
	}

	SwapBuffers(hDC);
}
