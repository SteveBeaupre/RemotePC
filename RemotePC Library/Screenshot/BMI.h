#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <Stdio.h>
//----------------------------------------------------------------------//
#include "IScreenshot.h"
//----------------------------------------------------------------------//
#include "RawBuffer.h"
//----------------------------------------------------------------------//

class CBMI {
public:
	CBMI();
private:
	int BMISize;
	CRawBuffer bmi;
	BITMAPINFO *lpbi;
	CBitmapHelper BitmapHelper;
public:
	BITMAPINFO* Get(){return lpbi;}
	
	void Erase();
	void Fill(int w, int h, int bpp);
};

