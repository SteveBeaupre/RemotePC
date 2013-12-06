#include "NetLog.h"

CNetLog::CNetLog()
{
	#ifndef _DEBUG
	m_Disabled = true;
	#else
	m_Disabled = false;
	#endif
	f = NULL;
	InitializeCriticalSection(&cs);
}

CNetLog::~CNetLog()
{
	Close();
	DeleteCriticalSection(&cs);
}

void CNetLog::Lock()
{
	EnterCriticalSection(&cs);
}

void CNetLog::Unlock()
{
	LeaveCriticalSection(&cs);
}

void CNetLog::Disable()
{
	EnterCriticalSection(&cs);
	m_Disabled = true;
	LeaveCriticalSection(&cs);
}

bool CNetLog::IsOpened()
{
	return f != NULL;
}

bool CNetLog::Create(char *fname)
{
	if(IsOpened() || m_Disabled)
		return false;

	f = fopen(fname, "wt+");

	return IsOpened();
}

void CNetLog::Log(const char *fmt, ...)
{
	if(!IsOpened() || m_Disabled || fmt == NULL)
		return;

	const int BufSize = 2048;

	char TextBuf[BufSize];
	char *pTxt = &TextBuf[0];
	ZeroMemory(pTxt, BufSize);

	va_list ap;
	va_start(ap, fmt);
	#ifdef RAD_STUDIO_XE
	vsnprintf(pTxt, BufSize, fmt, ap);
	#else
	vsprintf(pTxt, fmt, ap);
	#endif
	va_end(ap);

	Lock();
	fputs(pTxt, f);
	fflush(f);
	Unlock();
}

void CNetLog::Close()
{
	if(!IsOpened() || m_Disabled)
		return;

	Lock();
	fclose(f);
	f = NULL;
	Unlock();
}



