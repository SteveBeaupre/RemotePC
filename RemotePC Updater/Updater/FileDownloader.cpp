#include "FileDownloader.h"

static size_t OnProgressFunction(void *UserData, double DLTotal, double DLNow, double ULTotal, double ULNow)
{
	// Make sure total download is not zero
	if(DLTotal == 0)
		return 0;

	if(UserData){

		CFileDownloader* pFileDownloader = (CFileDownloader*)UserData;
		UINT MaxWrite = pFileDownloader->GetMaxWrite();
		if(MaxWrite > 0 && DLNow >= MaxWrite)
			return 1;
		pFileDownloader->OnProgress(DLTotal, DLNow, ULTotal, ULNow);
	}

	return 0;
}

static size_t OnWriteFunction(BYTE *IncomingData, size_t ItemSize, size_t ItemCount, void *UserData)
{
	size_t DataSize = ItemSize * ItemCount;

	// Make sure DataSize is not zero and IncomingData is valid
	if(DataSize == 0 || IncomingData == NULL)
		return 0;

	// Call virtual function OnWrite()
	if(UserData)
		((CFileDownloader*)UserData)->OnWrite(IncomingData, DataSize);

	// Always return DataSize
	return DataSize;
}

CFileDownloader::CFileDownloader()
{
	MaxWrite = 0;
	// Initialize libcurl
	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS,       0);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA,        this);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,     this);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,    OnWriteFunction);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgressFunction);
	}
}

UINT CFileDownloader::GetMaxWrite()
{
	return MaxWrite;
}

CFileDownloader::~CFileDownloader()
{
	// Cleanup libcurl
	curl = NULL;
	if(curl)
		curl_easy_cleanup(curl);
}

bool CFileDownloader::IsInitialized()
{
	return (curl != NULL);
}

void CFileDownloader::OnWrite(BYTE *pBuffer, UINT BufferSize)
{
	// Store the data block in the linked list
	if(IsInitialized())
		BuffersList.Push(pBuffer, BufferSize);
}

void CFileDownloader::Cancel()
{
	MaxWrite = 1;
}

bool CFileDownloader::DownloadFile(LPCSTR url, CRawBuffer *pBuffer, UINT *pMaxSize)
{
	if(!IsInitialized())
		return false;

	if(pMaxSize){
		MaxWrite = *pMaxSize;
	} else {
		MaxWrite = 0;
	}
	int NumTry = 5;

	// Try to read multiple time on failure...
	while(1){
		// Erase any previous data 
		BuffersList.Clear();

		// Set the url of the file to download
		curl_easy_setopt(curl, CURLOPT_URL, url);
		// Download it (or try...)
		CURLcode res = curl_easy_perform(curl);

		// Assume that a file of 0 size is an error...
		UINT BufSize = BuffersList.GetTotalBuffersSizes();
		// Make sure it's not...
		if(res == CURLE_OK || CURLE_ABORTED_BY_CALLBACK){
			if(BufSize > 0){
				// Get the size of all the sub-buffers combined
				UINT BufSize = BuffersList.GetTotalBuffersSizes();
				// Allocate a buffer to hold them all
				pBuffer->Allocate(BufSize, TRUE);
				// Extract them in the buffer
				BuffersList.ExtractBuffers(pBuffer->GetBuffer());

				bool Success = MaxWrite == 0 ? (res == CURLE_OK) : (BufSize >= MaxWrite);
				if(pMaxSize)
					*pMaxSize = BufSize;

				return Success;
			}
		}

		// Something went wrong, start over...
		Sleep(500);
		NumTry--;
		if(NumTry <= 0)
			break;
	}

	if(pMaxSize)
		*pMaxSize = 0;

	return false;
}


