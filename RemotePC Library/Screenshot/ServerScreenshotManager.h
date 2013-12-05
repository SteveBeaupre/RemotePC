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

class CServerScreenshotManager : IServerScreenshotManager {
public:
	CServerScreenshotManager();
	~CServerScreenshotManager();
private:
	CScreenshot Screenshot[2];
	CScreenshot *pBackBuffer, *pFrontBuffer;

	CScreenshot* GetBackBuffer() {return pBackBuffer;}
	CScreenshot* GetFrontBuffer(){return pFrontBuffer;}

	CRawBuffer UncompressedBuffer, CompressedBuffer;
	int EstimateCompressedBufferSize(int UncompressedSize);
public:
	void Reset();
	void SwapBuffers();

	int Take(BOOL bShowCursor);
	int Pack();
	int Compress();
};
