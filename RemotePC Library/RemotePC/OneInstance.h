#pragma once 

#include "Windows.h"
#include "SafeKill.h"

class COneInstance
{
public:
	COneInstance();
	~COneInstance();
private:
	HANDLE hMutex;
	bool Create(char *MutexName);
	void Release();
public:
	bool Check(char *MutexName);
};
