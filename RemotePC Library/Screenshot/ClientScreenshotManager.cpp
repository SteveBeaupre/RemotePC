#include "ClientScreenshotManager.h"

CClientScreenshotManager::CClientScreenshotManager()
{
	Reset();
}

CClientScreenshotManager::~CClientScreenshotManager()
{
	Reset();
}

void CClientScreenshotManager::Reset()
{
	UncompressedBuffer.Free();
	OpenGLBuffer.Free();
}

void CClientScreenshotManager::Decompress(BYTE *pCompressedBuffer, DWORD CompressedBufferSize, DecompressedScreenshotInfoStruct* pInfo)
{
	int CompressedHeaderSize = sizeof(CompressedScreenshotInfoStruct);

	CompressedScreenshotInfoStruct CompressionHeader;
	memcpy(&CompressionHeader, pCompressedBuffer, CompressedHeaderSize);

	ScrFormat Format;
	int w, h, bpp, UncSize;
	w = CompressionHeader.Width;
	h = CompressionHeader.Height;
	bpp = CompressionHeader.BitsPerPixel;
	UncSize = CompressionHeader.UncompressedSize;
	Format = CompressionHeader.Format;

	UncompressedBuffer.Allocate(UncSize);
	DWORD CompressedDataSize = CompressedBufferSize - CompressedHeaderSize;
	
	{
		BYTE *pIn  = &pCompressedBuffer[CompressedHeaderSize];
		BYTE *pOut = UncompressedBuffer.GetBuffer();

		// Decompress the data
		int Res = FreeImage_ZLibUncompress(pOut, UncSize, pIn, CompressedDataSize);
		
		// Check for error
		if(Res == 0){
			pInfo->Width  = 0;
			pInfo->Height = 0;
			pInfo->BitsPerPixel = 0;
			pInfo->Format = scrf_32;
			pInfo->pBuffer = NULL;
			return;
		}
	}

	// Re-allocate the opengl buffer if the size has changed
	int BufSize = BitmapHelper.CalcBufferSize(w, h, bpp);
	if(OpenGLBuffer.GetBufferSize() != (UINT)BufSize)
		OpenGLBuffer.Allocate(BufSize); 
	
	// Used to read our unc. buffer data
	int Pitch = BitmapHelper.CalcPitchSize(w, h, bpp);

	// Start decompressing the images blocks
	int BufIndx = 0;
	while(BufIndx < UncSize){

		// Gather information about the current texture part
		BlockInfoStruct BlockInfo;
		memcpy(&BlockInfo, UncompressedBuffer.GetBuffer(BufIndx), sizeof(BlockInfoStruct));
		BufIndx += sizeof(BlockInfoStruct);

		int Offset = BlockInfo.BlockStart;

		// Copy and decode the data, line per line
		for(int CptLines = 0; CptLines < BlockInfo.BlockHeight; CptLines++){

			// Copy a line of data
			memcpy(OpenGLBuffer.GetBuffer(Offset), UncompressedBuffer.GetBuffer(BufIndx), BlockInfo.BlockWidth);		

			// Update the unc. buffer index
			Offset  += Pitch;
			BufIndx += BlockInfo.BlockWidth;
		}
	}
	
	// Convert the image if format is 4 bpp
	if(Format != scrf_4){
		pInfo->pBuffer = &OpenGLBuffer;
	} else {
		bpp = 8;
		Format = scrf_8g;

		Convert4To8bpp(w, h, bpp, Pitch);

		pInfo->pBuffer = &ConvertedBuffer;
	}

	pInfo->Width  = w;
	pInfo->Height = h;
	pInfo->BitsPerPixel = bpp;
	pInfo->Format = Format;
}

void CClientScreenshotManager::Convert4To8bpp(int w, int h, int bpp, int Pitch)
{
	int ConvPitch   = BitmapHelper.CalcPitchSize(w, h, bpp);
	int ConvBufSize = BitmapHelper.CalcBufferSize(w, h, bpp);

	ConvertedBuffer.Allocate(ConvBufSize, TRUE);

	BYTE *pIn  = NULL;
	BYTE *pOut = NULL;

	BYTE in = 0;
	BYTE out[2] = {0, 0};

	for(int y = 0; y < h; y++){

		pIn  = OpenGLBuffer.GetBuffer(Pitch * y);
		pOut = ConvertedBuffer.GetBuffer(ConvPitch * y);

		int InIndx  = 0;
		int OutIndx = 0;

		for(int x = 0; x < w; x++){

			in = pIn[InIndx];
			
			out[0] = ((in & 0xF0) | (in >> 4));
			out[1] = ((in & 0x0F) | (in << 4));

			memcpy(&pOut[OutIndx], &out[0], 2);

			InIndx++;
			OutIndx += 2;
		}
	}
}