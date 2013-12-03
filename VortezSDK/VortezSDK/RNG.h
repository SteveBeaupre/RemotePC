#ifndef _CRNG_H_
#define _CRNG_H_
#ifdef __cplusplus

#include "Windows.h"

#include "Preproc.h"

#ifdef COMPILE_FOR_BORLAND
class ARNG {
public:
	virtual void _INTERNAL_LINKAGE InitializeGenerator(int Seed) = 0;
	virtual int  _INTERNAL_LINKAGE ExtractNumber() = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CRNG : public ARNG {
#else
class _EXP_FUNC CRNG {
#endif
public:
	CRNG();
private:
	int MT[624]; 
	int index;

	void GenerateNumbers();
public:
	void _INTERNAL_LINKAGE InitializeGenerator(int seed);
	int  _INTERNAL_LINKAGE ExtractNumber();

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_CRNG_H_






 
