#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "MsgIDs.h"
//----------------------------------------------------------------------//

#define MAX_LOGIN_INFO_LEN   32

struct LoginInfoStruct {
	BYTE AuthCode[MAX_LOGIN_INFO_LEN];
	char UserName[MAX_LOGIN_INFO_LEN];
	char Password[MAX_LOGIN_INFO_LEN];
};

struct LoginResultStruct {
	BOOL LogedIn;
	WORD ScrWidth;
	WORD ScrHeight;
};

class ILoginInfo : public LoginInfoStruct {
public:
	virtual void Reset() = 0;
	virtual bool CompareLoginInfo(LoginInfoStruct *pInfo) = 0;

	virtual char* GetName() = 0;
	virtual char* GetPassword() = 0;

	virtual void SetName(char *pName) = 0;
	virtual void SetPassword(char *pPassword) = 0;
	virtual void SetInfo(char *pName, char *pPassword) = 0;
};
