#pragma once

#include "stdio.h"
#include "VersionNumber.h"

void SetCaption(char *pAppName, char* out, int BufSize)
{
	ZeroMemory(out, BufSize);

	char VerNum[64];
	ZeroMemory(VerNum, 64);
	sprintf(VerNum, "%s", VERNUM);

	if(VerNum[4] == '0')
		memset(&VerNum[3], 0, 2);

	#ifdef _DEBUG
	sprintf(out, "%s v%s (Built the %s at %s)", pAppName, VerNum, __DATE__, __TIME__);
	#else
	sprintf(out, "%s v%s", pAppName, VerNum);
	#endif
}
