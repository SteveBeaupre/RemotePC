#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "Screenshot.h"
#include "QuadTree.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CClientScreenshotManager : IClientScreenshotManager {
public:
	CClientScreenshotManager();
	~CClientScreenshotManager();
private:
	CBitmapHelper BitmapHelper;
	CRawBuffer UncompressedBuffer, OpenGLBuffer, ConvertedBuffer;
private:
	void Convert4To8bpp(int w, int h, int bpp, int Pitch);
public:
	void Reset();
	void Decompress(BYTE *pCompressedBuffer, DWORD CompressedBufferSize, DecompressedScreenshotInfoStruct* pInfo);
};
