#pragma once

#include "stdio.h"
#include "Version.h"

void SetCaption(char *pAppName, char* out, int BufSize)
{
	ZeroMemory(out, BufSize);

	#ifdef _DEBUG
	sprintf(out, "%s (Built the %s at %s)", pAppName, __DATE__, __TIME__);
	#else
	if(AnsiString(VERNUM) == "1.0.0"){
		sprintf(out, "%s v%s", pAppName, "1.0");
	} else {
		sprintf(out, "%s v%s", pAppName, VERNUM);
	}
	#endif
}
