#include "OneInstance.h"

COneInstance::COneInstance()
{
	hMutex = NULL;
}

COneInstance::~COneInstance()
{
	Release();
}

bool COneInstance::Check(char *MutexName)
{
	return Create(MutexName);
}

bool COneInstance::Create(char *MutexName)
{
	bool IsAlreadyRunning = false;

	if(!hMutex){
		hMutex = CreateMutexA(NULL, FALSE, MutexName);

		UINT err = GetLastError();
		IsAlreadyRunning = (err == ERROR_ALREADY_EXISTS || err == ERROR_ACCESS_DENIED);
	}
	return hMutex && !IsAlreadyRunning;
}

void COneInstance::Release()
{
	if(hMutex)
		ReleaseMutex(hMutex);
}


