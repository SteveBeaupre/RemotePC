#include "ServerScreenshotManager.h"

CServerScreenshotManager::CServerScreenshotManager()
{
	lz4DllStub.Load("lz4-r117.dll");
	SetFormat(scrf_32);
	Reset();
}

CServerScreenshotManager::~CServerScreenshotManager()
{
	Reset();
	lz4DllStub.Free();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::SetFormat(ScrFormat Format)
{
	ScreenshotFormat = Format;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::SwapBuffers()
{
	CScreenshot* pTmp = pBackBuffer;
	pBackBuffer  = pFrontBuffer;
	pFrontBuffer = pTmp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::Reset()
{
	pBackBuffer  = &Screenshot[0];
	pFrontBuffer = &Screenshot[1];

	pBackBuffer->Reset();
	pFrontBuffer->Reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::Take()
{
	pBackBuffer->Take(ScreenshotFormat);
	
	AdjustFrontBuffer();

	Pack();
	Compress();

	SwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::AdjustFrontBuffer()
{
	int bsize = pBackBuffer->GetBufferSize();
	int fsize = pFrontBuffer->GetBufferSize();

	if(fsize != bsize){
		int bWidth  = pBackBuffer->GetWidth();
		int bHeight = pBackBuffer->GetHeight();
		int bBPP    = pBackBuffer->GetBitsPerPixel();

		pFrontBuffer->CreateEmptyFrame(GetFormat(), bWidth, bHeight, bBPP);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::Pack()
{
	// Define the max. depth of the recursion
	static const BYTE MaxDepth = 4; 

	BYTE *pFront = pFrontBuffer->GetBuffer();
	BYTE *pBack  = pBackBuffer->GetBuffer();
	
	WORD w   = pBackBuffer->GetWidth();
	WORD h   = pBackBuffer->GetHeight();
	BYTE bpp = pBackBuffer->GetBitsPerPixel();

	// Generate the full tree
	CQuadTree QuadTree;

	QuadTree.SetScreenshotBuffers(pFront, pBack);

	int Pitch = BitmapHelper.CalcPitchSize(w, h, bpp);

	QuadTree.InitTree(Pitch, h, MaxDepth);
	
	QuadTree.GenTree();
	QuadTree.TrimTree();
	
	// Calculate the size needed for the uncompressed buffer
	int UncompressedSize = QuadTree.CalcOutputBufferSize();

	// Extract the data in the uncompressed buffer
	UncompressedBuffer.Allocate(UncompressedSize);
	BYTE *pUnc = UncompressedBuffer.GetBuffer();

	QuadTree.SetUncompressedBuffer(pUnc);
	QuadTree.ExtractTreeData();

	// we're done with the tree
	QuadTree.DelTree();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::Compress()
{
	// Store the uncompressed size and return 0 if empty
	int UncompressedSize = UncompressedBuffer.GetSize();
	if(UncompressedSize == 0)
		return;
	
	int CompressedHeaderSize = sizeof(CompressedScreenshotInfoStruct);
	int CompressedBufferSize = EstimateCompressedBufferSize(UncompressedSize) + CompressedHeaderSize;

	CompressedBuffer.Allocate(CompressedBufferSize);

	CompressedScreenshotInfoStruct CompressionHeader;
	CompressionHeader.Width  = pBackBuffer->GetWidth();
	CompressionHeader.Height = pBackBuffer->GetHeight();
	CompressionHeader.Format = pBackBuffer->GetFormat();
	CompressionHeader.BitsPerPixel = pBackBuffer->GetBitsPerPixel();
	CompressionHeader.UncompressedSize = UncompressedSize;

	memcpy(CompressedBuffer.GetBuffer(), &CompressionHeader, CompressedHeaderSize);

	BYTE *pIn  = UncompressedBuffer.GetBuffer();
	BYTE *pOut = CompressedBuffer.GetBuffer(CompressedHeaderSize);

	int CompressedDataSize = lz4DllStub.Compress((char*)pIn, (char*)pOut, UncompressedSize);

	CompressedBufferSize = CompressedHeaderSize + CompressedDataSize;
	CompressedBuffer.Resize(CompressedBufferSize);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CServerScreenshotManager::EstimateCompressedBufferSize(int UncompressedSize)
{
	return (int)((float)UncompressedSize * 1.1f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI ScreenshotThreadFunc(void *params)
{
	CServerScreenshotManager* pScreenshotManager = (CServerScreenshotManager*)params;

	pScreenshotManager->Take();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CServerScreenshotManager::IsScreenshotThreadedRunning()
{
	return ScreenshotTread.IsThreadRunning();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::WaitForScreenshotThreadToFinish()
{
	ScreenshotTread.WaitForThread();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::StartScreenshotThread()
{
	ScreenshotTread.StartThread(ScreenshotThreadFunc, this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CServerScreenshotManager::StopScreenshotThread()
{
	ScreenshotTread.StopThread();
}

