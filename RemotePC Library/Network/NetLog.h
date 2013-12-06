#ifndef _NET_LOG_H_
#define _NET_LOG_H_
#ifdef __cplusplus
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//

class CNetLog {
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
	void Log(const char *fmt, ...);
	void Close();
};

#endif
#endif //--_NET_LOG_H_
