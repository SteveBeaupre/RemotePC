#ifndef _ENCRYPTOR_H_
#define _ENCRYPTOR_H_
#ifdef __cplusplus

#include "Windows.h"
#include "stdio.h"

#include "crc32.h"
#include "RNG.h"
#include "RawBuffer.h"

#include "Preproc.h"

#ifdef COMPILE_FOR_BORLAND
class AEncryptor {
public:
	virtual void _INTERNAL_LINKAGE Initialize(void *pKey, bool StrongEncryption = false) = 0;

	virtual void _INTERNAL_LINKAGE Encrypt(BYTE *pBuffer, DWORD BufferSize) = 0;
	virtual void _INTERNAL_LINKAGE Decrypt(BYTE *pBuffer, DWORD BufferSize) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CEncryptor  : public AEncryptor {
#else
class _EXP_FUNC CEncryptor {
#endif
public:
	CEncryptor();
	~CEncryptor();
private:
	int  NumKeyChars;
	bool UseStrongEncryption;

	CRNG KeyRNG;
	CRNG *pKeyCharRNG;

	void GenKey(BYTE *pKey, DWORD KeySize, CRNG *rng);
	void XOR(BYTE *pBlock, BYTE *pKey, DWORD BlockSize);
public:
	void _INTERNAL_LINKAGE Initialize(void *pKey, UINT KeyLen, bool StrongEncryption = false);

	void _INTERNAL_LINKAGE Encrypt(void *pBuffer, DWORD BufferSize);
	void _INTERNAL_LINKAGE Decrypt(void *pBuffer, DWORD BufferSize);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_ENCRYPTOR_H_
