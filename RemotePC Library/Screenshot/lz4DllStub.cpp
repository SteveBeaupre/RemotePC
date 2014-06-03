#include "lz4DllStub.h"

Clz4DllStub::Clz4DllStub()
{
	hDll = NULL;
	Free();
}

Clz4DllStub::~Clz4DllStub()
{
	Free();
}

bool Clz4DllStub::IsLoaded()
{
	return hDll != NULL;
}

void Clz4DllStub::PrintErrorMsg(const char *fmt, ...)
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

bool Clz4DllStub::Load(char *fname)
{
	LPCSTR szCompress   = "_Compress";
	LPCSTR szDecompress = "_Decompress";

	if(IsLoaded())
		Free();

	hDll = LoadLibraryA(fname);

	if(hDll){
		Compress   = (PFNCOMPRESS)GetProcAddress(hDll, szCompress);
		Decompress = (PFNDECOMPRESS)GetProcAddress(hDll, szDecompress);

		LPCSTR FuncErrMsg = "\"%s\" dll: \"%s\" function not found.";

		if(!Compress){
			PrintErrorMsg(FuncErrMsg, fname, szCompress);
			return false;
		}
		if(!Decompress){
			PrintErrorMsg(FuncErrMsg, fname, szDecompress);
			return false;
		}

		return true;
	} else {
		PrintErrorMsg("Unable to load \"%s\".", fname);
	}

	return false;
}

void Clz4DllStub::Free()
{
	if(IsLoaded())
		FreeLibrary(hDll);

	hDll = NULL;

	Compress   = NULL;
	Decompress = NULL;
}



