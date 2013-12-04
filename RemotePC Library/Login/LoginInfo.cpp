#include "LoginInfo.h"

CLoginInfo::CLoginInfo()
{
	Reset();
}

void CLoginInfo::SetAuthCode()
{
	const BYTE Code[MAX_LOGIN_INFO_LEN] = {0x8E, 0x98, 0xD2, 0x4C, 0x83, 0x13, 0xFE, 0xD0, 0x16, 0x34, 0x03, 0x23, 0xD8, 0x3B, 0x5A, 0x8B, 0x71, 0x4F, 0x47, 0x53, 0x65, 0x0E, 0x29, 0x1E, 0xCC, 0x8F, 0xDD, 0x66, 0xC0, 0x85, 0xBC, 0x22};
	memcpy(&AuthCode[0], &Code[0], MAX_LOGIN_INFO_LEN);
}

bool CLoginInfo::CompareLoginInfo(LoginInfoStruct *pInfo)
{
	if(memcmp(&AuthCode[0], &pInfo->AuthCode[0], MAX_LOGIN_INFO_LEN) == 0){
		//if(memcmp(&UserName[0], &pInfo->UserName[0], MAX_LOGIN_INFO_LEN) == 0){
			if(memcmp(&Password[0], &pInfo->Password[0], MAX_LOGIN_INFO_LEN) == 0){
				return true;
			}
		//}
	}

	return false;
}

void CLoginInfo::Reset()
{
	SetAuthCode();
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

