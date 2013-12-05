#include "ServerScreenshotManager.h"

CServerScreenshotManager::CServerScreenshotManager()
{
	Reset();
}

CServerScreenshotManager::~CServerScreenshotManager()
{
	Reset();
}

void CServerScreenshotManager::SwapBuffers()
{
	CScreenshot* pTmp = pBackBuffer;
	pBackBuffer  = pFrontBuffer;
	pFrontBuffer = pTmp;
}

void CServerScreenshotManager::Reset()
{
	pBackBuffer  = &Screenshot[0];
	pFrontBuffer = &Screenshot[1];

	pBackBuffer->Reset();
	pFrontBuffer->Reset();
}

int CServerScreenshotManager::Take(BOOL bShowCursor)
{
	pBackBuffer->Take(bShowCursor);

	int res = Compress();
	this->SwapBuffers();

	return res;
}

int CServerScreenshotManager::Pack()
{
	// Define the depth of the recursion process
	static const BYTE MaxDepth = 4; 

	BYTE *pFront = pFrontBuffer->GetBuffer();
	BYTE *pBack  = pBackBuffer->GetBuffer();
	
	WORD w   = pBackBuffer->GetWidth();
	WORD h   = pBackBuffer->GetHeight();
	BYTE bpp = pBackBuffer->GetBytesPerPixel();

	// Generate the full tree
	CQuadTree QuadTree;

	QuadTree.SetScreenshotBuffers(pFront, pBack);
	QuadTree.InitTree(w, h, bpp, MaxDepth);
	
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
	QuadTree.KillTree();

	return UncompressedSize;
}

int CServerScreenshotManager::EstimateCompressedBufferSize(int UncompressedSize)
{
	return (int)(((float)UncompressedSize * 1.1f) + 12.0f);
}

int CServerScreenshotManager::Compress()
{
	// Store the uncompressed size and return 0 if empty
	int UncompressedSize = UncompressedBuffer.GetSize();
	if(UncompressedSize == 0)
		return 0;
	
	int CompressedDataSize = 0;
	int CompressedBufferSize = EstimateCompressedBufferSize(UncompressedSize);

	CompressedBuffer.Allocate(UncompressedSize);

	BYTE *pOut = CompressedBuffer.GetBuffer();
	BYTE *pIn  = UncompressedBuffer.GetBuffer();

	int CompressedSize = FreeImage_ZLibCompress(pOut, CompressedBufferSize, pIn, UncompressedSize);

	return CompressedSize;
}

