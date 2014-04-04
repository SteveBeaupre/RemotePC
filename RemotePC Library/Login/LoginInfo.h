#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "ILoginInfo.h"
#include "VersionNumber.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CLoginInfo : public ILoginInfo {
public:
	CLoginInfo();
private:
	void GenAuthCode();
public:
	void Reset();
	LoginResults CompareLoginInfo(LoginInfoStruct *pInfo);

	char* GetName();
	char* GetPassword();

	void SetName(char *pName);
	void SetPassword(char *pPassword);
	void SetInfo(char *pName, char *pPassword);
};
