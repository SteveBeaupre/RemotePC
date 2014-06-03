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
#include "Thread.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

DWORD WINAPI ScreenshotThreadFunc(void *params);

class EXP_FUNC CServerScreenshotManager : IServerScreenshotManager {
public:
	CServerScreenshotManager();
	~CServerScreenshotManager();
private:
	CThread ScreenshotTread;
	CBitmapHelper BitmapHelper;
	Clz4DllStub lz4DllStub;
private:
	CScreenshot Screenshot[2];
	CScreenshot *pBackBuffer, *pFrontBuffer;

	CScreenshot* GetBackBuffer() {return pBackBuffer;}
	CScreenshot* GetFrontBuffer(){return pFrontBuffer;}

	CRawBuffer UncompressedBuffer, CompressedBuffer;

	ScrFormat ScreenshotFormat;

	void AdjustFrontBuffer();
	int  EstimateCompressedBufferSize(int UncompressedSize);
public:
	void Reset();
	void SwapBuffers();

	void Pack();
	void Compress();
	
	void Take();
	CRawBuffer* GetCompressedBuffer(){return &CompressedBuffer;}

	void SetFormat(ScrFormat Format);
	ScrFormat GetFormat(){return ScreenshotFormat;}

	bool IsScreenshotThreadedRunning();
	void WaitForScreenshotThreadToFinish();
	void StartScreenshotThread();
	void StopScreenshotThread();
};
