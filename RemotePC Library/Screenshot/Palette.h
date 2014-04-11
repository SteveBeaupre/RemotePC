#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <Stdio.h>
//----------------------------------------------------------------------//
#include "IScreenshot.h"
//----------------------------------------------------------------------//

class CPalette {
public:
	CPalette();
private:
	BITMAPINFO *lpbi;
private:
	void Gen8BitsPalette(bool Grayscale);
	void Gen8bitsBGR233Palette();
	void Gen8bitsGrayscalePalette();
	void Gen4bitsPalette();
	void Gen1bitsPalette();
public:
	void Fill(BITMAPINFO *bmi, ScrFormat Format, int bpp);
};



