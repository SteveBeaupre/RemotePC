#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

typedef int (WINAPI* PFNCOMPRESS)(const char* source, char* dest, int inputSize);
typedef int (WINAPI* PFNDECOMPRESS)(const char* source, char* dest, int inputSize, int maxOutputSize);

class EXP_FUNC Clz4DllStub {
public:
	Clz4DllStub();
	~Clz4DllStub();
private:
	HINSTANCE hDll;
private:
	void PrintErrorMsg(const char *fmt, ...);
public:
	PFNCOMPRESS   Compress;
	PFNDECOMPRESS Decompress;
public:
	bool IsLoaded();
	bool Load(char *fname);
	void Free();
};
