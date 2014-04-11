#include "BMI.h"

CBMI::CBMI()
{
	BMISize = sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD));
	bmi.Allocate(BMISize);
	lpbi = (BITMAPINFO*)bmi.GetBuffer();
}

void CBMI::Erase()
{
	bmi.Erase();
}

void CBMI::Fill(int w, int h, int bpp)
{
	Erase();
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbi->bmiHeader.biWidth = w;
	lpbi->bmiHeader.biHeight = h;
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = bpp;
	lpbi->bmiHeader.biCompression = BI_RGB;
	lpbi->bmiHeader.biSizeImage = BitmapHelper.CalcBufferSize(w, h, bpp);
	lpbi->bmiHeader.biXPelsPerMeter = 1000000;
	lpbi->bmiHeader.biYPelsPerMeter = 1000000;
}


