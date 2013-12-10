#ifndef _KEYBOARD_HOOK_DLL_STUB_H_
#define _KEYBOARD_HOOK_DLL_STUB_H_
#ifdef __cplusplus

#include <Windows.h>
#include <stdio.h>

typedef BOOL (WINAPI* PFNINSTALLHOOK)(HWND ViewerWnd, void *pKeyEventFunc);
typedef void (WINAPI* PFNREMOVEHOOK)();


class CKbHookDllStub {
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

#endif
#endif //_KEYBOARD_HOOK_DLL_STUB_H_
