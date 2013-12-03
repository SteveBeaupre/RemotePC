#ifndef _LOCK_H_
#define _LOCK_H_
#ifdef __cplusplus

#include "Preproc.h"

#include "Windows.h"
#include "stdio.h"

#ifdef COMPILE_FOR_BORLAND
class ALock {
public:
	virtual void  _INTERNAL_LINKAGE Lock() = 0;
	virtual void  _INTERNAL_LINKAGE Unlock() = 0;
	
	virtual void  _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CLock : public ALock {
#else
class _EXP_FUNC CLock {
#endif
public:
	CLock();
	~CLock();
private:
	CRITICAL_SECTION cs;
public:
	void  _INTERNAL_LINKAGE Lock();
	void  _INTERNAL_LINKAGE Unlock();

#ifdef COMPILE_FOR_BORLAND
	void  _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //--_LOCK_H_
