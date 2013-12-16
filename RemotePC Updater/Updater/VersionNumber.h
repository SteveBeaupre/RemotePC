#ifndef _VERSION_NUMBER_H_
#define _VERSION_NUMBER_H_
#ifdef __cplusplus

#include "Windows.h"
#include "stdio.h"


class CVersionNumber {
public:
	CVersionNumber();
private:
	int Hi, Md, Lo;
public:
	void Erase();

	void Get(int *h, int *m, int *l);
	int  GetHi();
	int  GetMd();
	int  GetLo();

	void Set(int h, int m, int l);
	void Scan(char *s);

	int  Compare(CVersionNumber *pVerNum);
};



#endif
#endif //_VERSION_NUMBER_H_
