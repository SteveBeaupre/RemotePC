#pragma once

#include "Windows.h"
#include "stdio.h"

void ConvertUnicodeToChar(char *pOutputBuffer, int OutputBufferSize, LPCWSTR pwStr, int wStrLen = -1)
{
	ZeroMemory(pOutputBuffer, OutputBufferSize);
	WideCharToMultiByte(CP_UTF8, 0, pwStr, wStrLen, pOutputBuffer, OutputBufferSize, NULL, NULL);
}
