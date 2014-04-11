#pragma once
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CNetLog {
public:
	CNetLog();
	~CNetLog();
private:
	FILE *f;
	CRITICAL_SECTION cs;
	bool m_Disabled;
private:
	void Lock();
	void Unlock();
public:
	void Disable();
	bool IsOpened();
	bool Create(char *fname);
	void Add(const char *fmt, ...);
	void Close();
};
