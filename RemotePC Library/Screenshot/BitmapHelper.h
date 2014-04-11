#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//

class CBitmapHelper {
public:
	CBitmapHelper();
private:
	int w, h, bpp, Alignment;
	void SetValues(int Width, int Height, int BitsPerPixels, int Align = 4);
private:
	int CalcPaddingSize(int RowSize);
	int CalcRowSize();
	int CalcPitchSize();
	int CalcBufferSize();
public:
	int CalcPaddingSize(int Width, int Height, int BitsPerPixels, int Align = 4);
	int CalcRowSize(int Width, int Height, int BitsPerPixels, int Align = 4);
	int CalcPitchSize(int Width, int Height, int BitsPerPixels, int Align = 4);
	int CalcBufferSize(int Width, int Height, int BitsPerPixels, int Align = 4);
};