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
#include "BitmapHelper.h"
//----------------------------------------------------------------------//

enum ScrFormat {
	scrf_32 = 0, 
	scrf_16, 
	scrf_8c, 
	scrf_8g, 
	scrf_4, 
	scrf_1, 
};

struct ImageInfoStruct {
	int Width;
	int Height;
	int BitsPerPixel;
	ScrFormat Format;
};

struct ScreenshotInfoStruct : public ImageInfoStruct {
	int NumPixels;
	int BufferSize;
};

struct ScreenshotStruct {
	ScreenshotInfoStruct Info;
	CRawBuffer Buffer;
};

struct CompressedScreenshotInfoStruct : public ImageInfoStruct {
	int UncompressedSize;
};

struct DecompressedScreenshotInfoStruct : public ImageInfoStruct {
	CRawBuffer *pBuffer;
};

class IScreenshot {
public:
	virtual void Reset() = 0;
	virtual void Take() = 0;
	
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetNumPixels() = 0;
	virtual int GetBitsPerPixel() = 0;
	virtual int GetBufferSize() = 0;
	
	virtual ScrFormat GetFormat() = 0;
	
	virtual BYTE* GetBuffer() = 0;
};

class IServerScreenshotManager {
public:
	virtual void Reset() = 0;
	virtual void Take() = 0;
	virtual CRawBuffer* GetCompressedBuffer() = 0;
};

class IClientScreenshotManager {
public:
	virtual void Reset() = 0;
	virtual void Decompress(BYTE *pCompressedBuffer, DWORD CompressedBufferSize, DecompressedScreenshotInfoStruct* pInfo) = 0;
};
