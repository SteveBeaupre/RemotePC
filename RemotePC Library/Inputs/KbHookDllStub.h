#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

typedef BOOL (WINAPI* PFNINSTALLHOOK)(HWND ViewerWnd, void *pKeyEventFunc);
typedef void (WINAPI* PFNREMOVEHOOK)();

class EXP_FUNC CKbHookDllStub {
public:
	CKbHookDllStub();
	~CKbHookDllStub();
private:
	HINSTANCE hDll;
private:
	void PrintErrorMsg(const char *fmt, ...);
public:
	PFNINSTALLHOOK InstallHook;
	PFNREMOVEHOOK  RemoveHook;
public:
	bool IsLoaded();
	bool Load(char *fname);
	void Free();
};
