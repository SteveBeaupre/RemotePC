#ifndef _CVORTEZ_SDK_H_
#define _CVORTEZ_SDK_H_
#ifdef __cplusplus

#ifndef RAD_STUDIO_XE
#include "Conversion.h"
#endif
#include "CRC32.h"
#include "Bits.h"
#include "BitArray.h"
#include "DriveIO.h"
#include "FileIO.h"
#include "FileSystem.h"
#include "Registry.h"
#include "RawBuffer.h"
#include "Encryptor.h"
#include "Thread.h"
#include "Timer.h"
#include "Crono.h"
#include "Window.h"
#include "RectCls.h"
#include "RNG.h"
#include "Lock.h"
#ifndef RAD_STUDIO_XE
#include "Vector.h"
#include "Matrix.h"
#include "Strings.h"
#include "StringsList.h"
//#include "LinkedList.h"
#endif
#include "WinVersion.h"

#include "Preproc.h"

#ifdef COMPILE_FOR_BORLAND
#ifdef COMPILE_CPPBLD_DLL
#ifdef __cplusplus
extern "C" {
#endif
#endif
_EXP_FUNC ABitArray*      _EXTERNAL_LINKAGE NewBitArray();
_EXP_FUNC ACompressor*    _EXTERNAL_LINKAGE NewCompressor();
_EXP_FUNC ACRC32*         _EXTERNAL_LINKAGE NewCRC32();
_EXP_FUNC ACrono*         _EXTERNAL_LINKAGE NewCrono();
_EXP_FUNC ADriveIO*       _EXTERNAL_LINKAGE NewDriveIO();
_EXP_FUNC AFileIO*        _EXTERNAL_LINKAGE NewFileIO();
_EXP_FUNC ATxtFileIO*     _EXTERNAL_LINKAGE NewTxtFileIO();
_EXP_FUNC AFileManager*   _EXTERNAL_LINKAGE NewFileManager();
_EXP_FUNC AFolderManager* _EXTERNAL_LINKAGE NewFolderManager();
_EXP_FUNC ADriveManager*  _EXTERNAL_LINKAGE NewDriveManager();
_EXP_FUNC AEncryptor*     _EXTERNAL_LINKAGE NewEncryptor();
_EXP_FUNC ARNG*           _EXTERNAL_LINKAGE NewRNG();
_EXP_FUNC ARawBuffer*     _EXTERNAL_LINKAGE NewRawBuffer();
_EXP_FUNC AThread*        _EXTERNAL_LINKAGE NewThread();
_EXP_FUNC ATimer*         _EXTERNAL_LINKAGE NewTimer();
_EXP_FUNC ATimer64*       _EXTERNAL_LINKAGE NewTimer64();
_EXP_FUNC AGameTimer*     _EXTERNAL_LINKAGE NewGameTimer();
//_EXP_FUNC ACycleTimer*    _EXTERNAL_LINKAGE NewCycleTimer();
_EXP_FUNC AWindow*        _EXTERNAL_LINKAGE NewWindow();
_EXP_FUNC AVector*        _EXTERNAL_LINKAGE NewVector();
_EXP_FUNC AMatrix*        _EXTERNAL_LINKAGE NewMatrix();
_EXP_FUNC AStrings*       _EXTERNAL_LINKAGE NewStrings();
_EXP_FUNC AStringsList*   _EXTERNAL_LINKAGE NewStringsList();
_EXP_FUNC AWinVersion*    _EXTERNAL_LINKAGE NewWinVersion();
_EXP_FUNC ALock*          _EXTERNAL_LINKAGE NewLock();
#ifdef COMPILE_CPPBLD_DLL
#ifdef __cplusplus
}
#endif
#endif
#endif
//----------------------------------------------------------------------//

#endif
#endif //_CVORTEZ_SDK_H_
