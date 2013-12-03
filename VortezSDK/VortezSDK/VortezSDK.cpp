#include "VortezSDK.h"

#ifndef DONT_COMPILE_AS_DLL
//-----------------------------------------------------------------------------
// Name : DllMain() (DllMain Entry Point)
//-----------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwreason,  LPVOID lpReserved )
{
	UNREFERENCED_PARAMETER(hInst);
	UNREFERENCED_PARAMETER(lpReserved);
    switch(fdwreason)
	{
    case DLL_PROCESS_ATTACH: break;
    case DLL_PROCESS_DETACH: break;
    case DLL_THREAD_ATTACH:  break;
    case DLL_THREAD_DETACH:  break;
    }
    return TRUE;
}
#endif

#ifdef COMPILE_FOR_BORLAND
_EXP_FUNC ABitArray* _EXTERNAL_LINKAGE NewBitArray()
{
   return new CBitArray();
}

_EXP_FUNC ACompressor* _EXTERNAL_LINKAGE NewCompressor()
{
   return new CCompressor();
}

_EXP_FUNC ACRC32* _EXTERNAL_LINKAGE NewCRC32()
{
   return new CCRC32();
}

_EXP_FUNC ACrono* _EXTERNAL_LINKAGE NewCrono()
{
   return new CCrono();
}

_EXP_FUNC ADriveIO* _EXTERNAL_LINKAGE NewDriveIO()
{
   return new CDriveIO();
}

_EXP_FUNC AFileIO* _EXTERNAL_LINKAGE NewFileIO()
{
   return new CFileIO();
}

_EXP_FUNC ATxtFileIO* _EXTERNAL_LINKAGE NewTxtFileIO()
{
   return new CTxtFileIO();
}

_EXP_FUNC AFileManager* _EXTERNAL_LINKAGE NewFileManager()
{
   return new CFileManager();
}

_EXP_FUNC AFolderManager* _EXTERNAL_LINKAGE NewFolderManager()
{
   return new CFolderManager();
}

_EXP_FUNC ADriveManager* _EXTERNAL_LINKAGE NewDriveManager()
{
   return new CDriveManager();
}

_EXP_FUNC AEncryptor* _EXTERNAL_LINKAGE NewEncryptor()
{
   return new CEncryptor();
}

_EXP_FUNC ARNG* _EXTERNAL_LINKAGE NewRNG()
{
   return new CRNG();
}

_EXP_FUNC ARawBuffer* _EXTERNAL_LINKAGE NewRawBuffer()
{
   return new CRawBuffer();
}

_EXP_FUNC AThread* _EXTERNAL_LINKAGE NewThread()
{
   return new CThread();
}

_EXP_FUNC ATimer* _EXTERNAL_LINKAGE NewTimer()
{
   return new CTimer();
}

_EXP_FUNC ATimer64* _EXTERNAL_LINKAGE NewTimer64()
{
   return new CTimer64();
}

_EXP_FUNC AGameTimer* _EXTERNAL_LINKAGE NewGameTimer()
{
   return new CGameTimer();
}

/*#ifndef RAD_STUDIO_XE
_EXP_FUNC ACycleTimer* _EXTERNAL_LINKAGE NewCycleTimer()
{
   return new CCycleTimer();
}
#endif*/

_EXP_FUNC AWindow* _EXTERNAL_LINKAGE NewWindow()
{
   return new CWindow();
}

_EXP_FUNC AVector* _EXTERNAL_LINKAGE NewVector()
{
   return new CVector();
}

_EXP_FUNC AMatrix* _EXTERNAL_LINKAGE NewMatrix()
{
   return new CMatrix();
}

_EXP_FUNC AStrings* _EXTERNAL_LINKAGE NewStrings()
{
   return new Strings();
}

_EXP_FUNC AStringsList* _EXTERNAL_LINKAGE NewStringsList()
{
   return new CStringsList();
}

_EXP_FUNC AWinVersion* _EXTERNAL_LINKAGE NewWinVersion()
{
   return new CWinVersion();
}

_EXP_FUNC ALock* _EXTERNAL_LINKAGE NewLock()
{
   return new CLock();
}

#endif