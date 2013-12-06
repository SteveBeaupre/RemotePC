#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#pragma comment(lib, "FreeImage.lib")
#include "FreeImage.h"
//----------------------------------------------------------------------//
#include "VortezSDK.h"
//----------------------------------------------------------------------//

struct ScreenshotInfoStruct {
	int Width;
	int Height;
	int NumPixels;
	int BitsPerPixel;
	int BytesPerPixel;
	int BufferSize;
};

struct CompressedScreenshotInfoStruct {
	int Width;
	int Height;
	int BitsPerPixel;
	int UncompressedSize;
};

struct ScreenshotStruct {
	ScreenshotInfoStruct Info;
	CRawBuffer Buffer;
};

struct ServerScreenshotManagerStruct {
	CRawBuffer Buffer;
};

class IScreenshot {
public:
	virtual void Reset() = 0;
	virtual int  Take(BOOL bShowCursor) = 0;
	virtual void CreateEmpty(int Width, int Height, int BitsPerPixel) = 0;
	
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetNumPixels() = 0;
	virtual int GetBitsPerPixel() = 0;
	virtual int GetBytesPerPixel() = 0;
	virtual int GetBufferSize() = 0;
	
	virtual BYTE* GetBuffer() = 0;
};

class IServerScreenshotManager {
public:
	virtual void Reset() = 0;
	virtual void SwapBuffers() = 0;

	virtual CRawBuffer* Take(BOOL bShowCursor) = 0;
	
	virtual void Pack() = 0;
	virtual void Compress() = 0;
};

class IClientScreenshotManager {
public:
	virtual void Reset() = 0;
	virtual CRawBuffer* Decompress(BYTE *pCompressedBuffer, DWORD CompressedBufferSize) = 0;
};
