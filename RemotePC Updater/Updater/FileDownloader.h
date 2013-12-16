#ifndef _FILE_DOWNLOADER_H_
#define _FILE_DOWNLOADER_H_
#ifdef __cplusplus

#include "Windows.h"
#include "stdio.h"

#pragma comment(lib, "libcurl.lib")

#include <stdio.h>
#include "curl.h"

#include "VortezSDK.h"
#include "BuffersList.h"

class CFileDownloader {
public:
	CFileDownloader();
	~CFileDownloader();
private:
	CURL *curl;
	CBuffersList BuffersList;
	UINT MaxWrite;
public:
	bool IsInitialized();
	bool DownloadFile(LPCSTR url, CRawBuffer *pBuffer, UINT *pMaxSize = 0);
	void Cancel();
public:
	UINT GetMaxWrite();
	virtual void OnWrite(BYTE *pBuffer, UINT BufferSize);
	virtual void OnProgress(double DLTotal, double DLNow, double ULTotal, double ULNow){}
};



#endif
#endif //_FILE_DOWNLOADER_H_
