#pragma once

#include "stdio.h"

void SetCaption(char *pAppName, char* out, int BufSize)
{
	ZeroMemory(out, BufSize);

	#ifdef _DEBUG
	sprintf(out, "%s (Built the %s at %s)", pAppName, __DATE__, __TIME__);
	#else
	sprintf(out, "%s", pAppName);
	#endif
}
