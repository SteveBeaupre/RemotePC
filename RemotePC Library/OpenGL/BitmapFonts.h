#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#include <gl\gl.h>
#include <gl\glu.h>
//----------------------------------------------------------------------//

class CBitmapFonts
{
public:
	CBitmapFonts();
public:   
	struct CBitmapFontSettings {
		int Left, Top;			
		BYTE R,G,B;
	};
private:
	int  base;
	int  Left, Top;
	int  R, G, B;
public:   
	void Build(HDC hDC, char *FontName, int *pbase, int FontSize, int l, int t, int r, int g, int b);
	void Build(HDC hDC, char *FontName, int *pbase, int FontSize, CBitmapFontSettings *pSettings);
    void Free();
public:   
    void glPrintArg(const char *fmt, ...);
	void glPrintTxt(const char *text);
public:   
	void SetPosition(int L, int T);
	void IncPosition(int L, int T);
	void SetColor(int lR, int lG, int lB);
	void SetSettings(CBitmapFontSettings *pSettings);
};
