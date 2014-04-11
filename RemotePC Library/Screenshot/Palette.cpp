#include "Palette.h"

CPalette::CPalette()
{
	lpbi = NULL;
}

void CPalette::Fill(BITMAPINFO *bmi, ScrFormat Format, int bpp)
{
	lpbi = bmi;

	switch(bpp)
	{
	case 1: Gen1bitsPalette(); break;
	case 4: Gen4bitsPalette(); break;
	case 8: Gen8BitsPalette(Format == scrf_8g); break;
	}
}

//----------------------------------------------------------------------//

void CPalette::Gen8BitsPalette(bool Grayscale)
{
	static const int ncols = 256;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	// Generate the palette
	switch(Grayscale)
	{
	case false: Gen8bitsBGR233Palette();    break;
	case true:  Gen8bitsGrayscalePalette(); break;
	}
}

//----------------------------------------------------------------------//

void CPalette::Gen8bitsGrayscalePalette()
{
	static const int ncols = 256;
	
	for(int i = 0; i < ncols; i++){
		lpbi->bmiColors[i].rgbRed   = i;
		lpbi->bmiColors[i].rgbGreen = i;
		lpbi->bmiColors[i].rgbBlue  = i;
	}
}

//----------------------------------------------------------------------//

void CPalette::Gen8bitsBGR233Palette()
{
	static const int ncols = 256;

	static const BYTE _2_bits_index[4] = {0x00, 0x55, 0xAA, 0xFF};
	static const BYTE _3_bits_index[8] = {0x00, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF};

	for(int i = 0; i < ncols; i++){

		int ri = (i & 0x07);
		int gi = (i & 0x38) >> 3;
		int bi = (i & 0xC0) >> 6;

		lpbi->bmiColors[i].rgbRed   = _3_bits_index[ri];
		lpbi->bmiColors[i].rgbGreen = _3_bits_index[gi];
		lpbi->bmiColors[i].rgbBlue  = _2_bits_index[bi];
	}
}

//---------------------------------------------------------------------------

void CPalette::Gen4bitsPalette()
{
	static const int ncols = 16;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;
	
	static const BYTE _4_bits_index[ncols] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                                              0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

	for(int i = 0; i < ncols; i++){
		BYTE Col = _4_bits_index[i];
		lpbi->bmiColors[i].rgbRed   = Col;
		lpbi->bmiColors[i].rgbGreen = Col;
		lpbi->bmiColors[i].rgbBlue  = Col;
	}
}

//---------------------------------------------------------------------------

void CPalette::Gen1bitsPalette()
{
	static const int ncols = 2;
	lpbi->bmiHeader.biClrUsed = ncols;
	lpbi->bmiHeader.biClrImportant = ncols;

	for(int i = 0; i < ncols; i++){
		BYTE Col = i == 0 ? 0 : 255;
		lpbi->bmiColors[i].rgbRed   = Col;
		lpbi->bmiColors[i].rgbGreen = Col;
		lpbi->bmiColors[i].rgbBlue  = Col;
	}
}

