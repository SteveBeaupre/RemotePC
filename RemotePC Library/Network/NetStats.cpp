#include "NetStats.h"

CNetStats::CNetStats()
{
	//Check if a High resolution timer is available 
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&PerfCounterFrequency)){ 
		IsPerfCounterAvailable = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime); 
		TimeScale = 1.0f / PerfCounterFrequency;
	} else { 
		IsPerfCounterAvailable = false;
		CurrentTime = timeGetTime(); 
		TimeScale	= 0.001f;
    } 

	Reset();
}

void CNetStats::Reset()
{
	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Initialization
	LastTime = CurrentTime;
	
	//StatsLock.Lock();
	ZeroMemory(&Stats, sizeof(CNetStatsStruct));
	//StatsLock.Unlock();
}

void FormatTransferSize(char *pBuffer, int BufSize, __int64 n)
{
	const __int64 _1KB_ = 1024;
	const __int64 _1MB_ = _1KB_*_1KB_;
	const __int64 _1GB_ = _1KB_*_1MB_;
	const __int64 _1TB_ = _1KB_*_1GB_;

	ZeroMemory(pBuffer, BufSize);

	if(n < 0)
		n = 0;

	if(n >= 0 && n < _1KB_){
		sprintf(pBuffer, n <= 1 ? "%lld Byte" : "%lld Bytes", n);
	} else if(n >= _1KB_ && n < _1MB_){
		sprintf(pBuffer, "%2.2f KB", ((double)n / (double)_1KB_));
	} else if(n >= _1MB_ && n < _1GB_){
		sprintf(pBuffer, "%2.2f MB", ((double)n / (double)_1MB_));
	} else if(n >= _1GB_ && n < _1TB_){
		sprintf(pBuffer, "%2.2f GB", ((double)n / (double)_1GB_));
	}
}

float CNetStats::Tick()
{
	LastTime = CurrentTime;

	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Calculate the elapsed time
	float ElapsedTime = (CurrentTime - LastTime) * TimeScale;

	// Calculate the elapsed time for our speed calculation
	fElapsedTime += ElapsedTime;
	if(fElapsedTime >= 1.0f){

		ZeroMemory(&Stats.szTotalDownloaded[0],     NETSTATS_TEXT_BUF_SIZE);
		ZeroMemory(&Stats.szTotalUploaded[0],       NETSTATS_TEXT_BUF_SIZE);
		ZeroMemory(&Stats.szAverageDownloadKBPS[0], NETSTATS_TEXT_BUF_SIZE);
		ZeroMemory(&Stats.szAverageUploadKBPS[0],   NETSTATS_TEXT_BUF_SIZE);

		//StatsLock.Lock();

		__int64 DLSpeed = Stats.d - Stats.ld;
		__int64 ULSpeed = Stats.u - Stats.lu;

		Stats.ld = Stats.d;
		Stats.lu = Stats.u;

		Stats.ad = ((float)(DLSpeed * (1.0f/fElapsedTime)) / 1024.0f);
		Stats.au = ((float)(ULSpeed * (1.0f/fElapsedTime)) / 1024.0f);

		FormatTransferSize(&Stats.szTotalDownloaded[0], NETSTATS_TEXT_BUF_SIZE, Stats.d);
		FormatTransferSize(&Stats.szTotalUploaded[0],   NETSTATS_TEXT_BUF_SIZE, Stats.u);

		sprintf(&Stats.szAverageDownloadKBPS[0], "%2.2f Kbp\\s", Stats.ad);
		sprintf(&Stats.szAverageUploadKBPS[0],   "%2.2f Kbp\\s", Stats.au);

		//StatsLock.Unlock();

		fElapsedTime = 0.0f;
	}

	return ElapsedTime;
}

void CNetStats::UpdateStats()
{
	Tick();
}

void CNetStats::IncNumBytesDownloaded(__int64 NumBytesDownloaded)
{
	//StatsLock.Lock();
	Stats.d += NumBytesDownloaded;
	//StatsLock.Unlock();
}

void CNetStats::IncNumBytesUploaded(__int64 NumBytesUploaded)
{
	//StatsLock.Lock();
	Stats.u += NumBytesUploaded;
	//StatsLock.Unlock();
}

