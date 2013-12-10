#include "KbHookDllStub.h"

CKbHookDllStub::CKbHookDllStub()
{
	hDll = NULL;
	Free();
}

CKbHookDllStub::~CKbHookDllStub()
{
	Free();
}

bool CKbHookDllStub::IsLoaded()
{
	return hDll != NULL;
}

void CKbHookDllStub::PrintErrorMsg(const char *fmt, ...)
{
	if(fmt == NULL)
		return;

	const int BufSize = 1024;

	char TextBuf[BufSize];
	char *pTxt = &TextBuf[0];
	ZeroMemory(pTxt, BufSize);

	va_list ap;
	va_start(ap, fmt);
	#ifdef RAD_STUDIO_XE
	vsnprintf(pTxt, BufSize, fmt, ap);
	#else
	vsprintf(pTxt, fmt, ap);
	#endif
	va_end(ap);

	MessageBoxA(NULL, pTxt, "Error!", 0);
}

bool CKbHookDllStub::Load(char *fname)
{
	LPCSTR szInstallHook = "_InstallHook";
	LPCSTR szRemoveHook  = "_RemoveHook";

	if(IsLoaded())
		Free();

	hDll = LoadLibraryA(fname);

	if(hDll){
		InstallHook = (PFNINSTALLHOOK)GetProcAddress(hDll, szInstallHook);
		RemoveHook  = (PFNREMOVEHOOK) GetProcAddress(hDll, szRemoveHook);

		LPCSTR FuncErrMsg = "\"%s\" dll: \"%s\" function not found.";

		if(!InstallHook){
			PrintErrorMsg(FuncErrMsg, fname, szInstallHook);
			return false;
		}
		if(!RemoveHook){
			PrintErrorMsg(FuncErrMsg, fname, szRemoveHook);
			return false;
		}

		return true;
	} else {
		PrintErrorMsg("Unable to load \"%s\".", fname);
	}

	return false;
}

void CKbHookDllStub::Free()
{
	if(IsLoaded()){
		if(RemoveHook)
			RemoveHook();
		FreeLibrary(hDll);
	}

	hDll = NULL;

	InstallHook = NULL;
	RemoveHook  = NULL;
}



