#include "LoginInfo.h"

CLoginInfo::CLoginInfo()
{
	Reset();
}

void CLoginInfo::GenAuthCode()
{
	CRNG   rng;
	CCRC32 crc32;
	LPCSTR LastCompatibleVersion = LAST_COMPATIBLE_VERSION;

	UINT crc = crc32.Generate((void*)&LastCompatibleVersion[0], strlen(LastCompatibleVersion));
	rng.InitializeGenerator(crc);

	for(int i = 0; i < MAX_LOGIN_INFO_LEN; ++i)
		AuthCode[i] = (BYTE)rng.ExtractNumber();
}

LoginResults CLoginInfo::CompareLoginInfo(LoginInfoStruct *pInfo)
{
	bool IsPasswordValid = memcmp(&Password[0], &pInfo->Password[0], MAX_LOGIN_INFO_LEN) == 0;
	bool IsAuthenticationCodeValid = memcmp(&AuthCode[0], &pInfo->AuthCode[0], MAX_LOGIN_INFO_LEN) == 0;

	if(!IsPasswordValid)
		return InvalidPassword;

	if(!IsAuthenticationCodeValid)
		return InvalidAuthorizationCode;

	return NoErrors;
}

void CLoginInfo::Reset()
{
	GenAuthCode();
	ZeroMemory(&UserName[0], MAX_LOGIN_INFO_LEN);
	ZeroMemory(&Password[0], MAX_LOGIN_INFO_LEN);
}

char* CLoginInfo::GetName()
{
	return (char*)&UserName[0];
}

char* CLoginInfo::GetPassword()
{
	return (char*)&Password[0];
}

void CLoginInfo::SetName(char *pName)
{
	int len = strlen(pName);
	if(len > 0){
		ZeroMemory(&UserName[0], MAX_LOGIN_INFO_LEN);
		strncpy(&UserName[0], pName, MAX_LOGIN_INFO_LEN);
	}
}

void CLoginInfo::SetPassword(char *pPassword)
{
	int len = strlen(pPassword);
	if(len > 0){
		ZeroMemory(&Password[0], MAX_LOGIN_INFO_LEN);
		strncpy(&Password[0], pPassword, MAX_LOGIN_INFO_LEN);
	}
}

void CLoginInfo::SetInfo(char *pName, char *pPassword)
{
	SetName(pName);
	SetPassword(pPassword);
}

