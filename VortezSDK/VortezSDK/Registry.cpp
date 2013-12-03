#include "Registry.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// Create a registry key
////////////////////////////////////////////////////////////////////////////////////////////////
void CRegistry::CreateKey(HKEY MainKey, LPCTSTR SubKey)
{
	HKEY hKey = NULL;
	DWORD Disposition = 0;

	RegCreateKeyEx(MainKey, SubKey, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &Disposition);
	CloseKey(hKey);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Delete a registry key
////////////////////////////////////////////////////////////////////////////////////////////////
void CRegistry::DeleteKey(HKEY MainKey, LPCTSTR SubKey)
{
	RegDeleteKey(MainKey, SubKey);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Tell if a registry key exist
////////////////////////////////////////////////////////////////////////////////////////////////
bool CRegistry::DoesKeyExist(HKEY MainKey, LPCTSTR SubKey)
{
	HKEY hKey = NULL;
	bool Res = false;
	if(this->OpenKey(MainKey, SubKey, &hKey)){
		this->CloseKey(hKey);
		Res = true;
	}

	return Res;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Get a registry value
////////////////////////////////////////////////////////////////////////////////////////////////
bool CRegistry::GetValue(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value, DWORD Type, void *pBuffer, DWORD *pBufferSize)
{
	HKEY hKey = NULL;
	bool Res = false;
	if(this->OpenKey(MainKey, SubKey, &hKey)){
		Res = RegQueryValueEx(hKey, Value, 0L, &Type, (BYTE*)pBuffer, pBufferSize) == ERROR_SUCCESS;
		this->CloseKey(hKey);
	}
	return Res;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Get a registry value
////////////////////////////////////////////////////////////////////////////////////////////////
UINT CRegistry::GetValueType(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value)
{
	HKEY hKey = NULL;
	DWORD Type = 0, BufferSize = 0;
	if(this->OpenKey(MainKey, SubKey, &hKey)){
		RegQueryValueEx(hKey, Value, 0L, &Type, NULL, &BufferSize);
		this->CloseKey(hKey);
	}
	return Type;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Set a registry value
////////////////////////////////////////////////////////////////////////////////////////////////
void CRegistry::SetValue(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value, DWORD Type, void *pBuffer, DWORD BufferSize)
{
	HKEY hKey = NULL;
	if(this->OpenKey(MainKey, SubKey, &hKey)){
		RegSetValueEx(hKey, Value, 0L, Type, (BYTE*)pBuffer, BufferSize);
		this->CloseKey(hKey);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Delete a registry value
////////////////////////////////////////////////////////////////////////////////////////////////
void CRegistry::DeleteValue(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value)
{
	HKEY hKey = NULL;
	if(this->OpenKey(MainKey, SubKey, &hKey)){
		RegDeleteValue(hKey, Value);
		this->CloseKey(hKey);
	}
}

bool CRegistry::DoesValueExist(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value)
{
	HKEY hKey = NULL;
	DWORD Type = 0, BufferSize = 0;
	bool Res = false;
	if(this->OpenKey(MainKey, SubKey, &hKey)){
		Res = RegQueryValueEx(hKey, Value, 0L, &Type, NULL, &BufferSize) != ERROR_FILE_NOT_FOUND;
		this->CloseKey(hKey);
	}
	return Res;
}

bool CRegistry::LoadInteger(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value, void *pBuffer, DWORD DefaultValue)
{
	DWORD *pData = (DWORD*)pBuffer;
	*pData = DefaultValue;

	DWORD dwSizeOfValue = sizeof(DWORD);

	if(DoesValueExist(HKEY_CURRENT_USER, SubKey, Value)){
		GetValue(HKEY_CURRENT_USER, SubKey, Value, REG_DWORD, pBuffer, &dwSizeOfValue);
		return true;
	} else {
		SetValue(HKEY_CURRENT_USER, SubKey, Value, REG_DWORD, pBuffer, dwSizeOfValue);
	}

	return false;
}

void CRegistry::SaveInteger(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value, void *pBuffer)
{
	SetValue(HKEY_CURRENT_USER, SubKey, Value, REG_DWORD, pBuffer, sizeof(DWORD));
}

bool CRegistry::LoadString(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value, void *pBuffer, int BufSize, char *pDefaultValue)
{
	char *pData = (char*)pBuffer;
	if(pDefaultValue)
		strncpy(pData, pDefaultValue, BufSize);

	DWORD dwSizeOfValue = BufSize;

	if(DoesValueExist(HKEY_CURRENT_USER, SubKey, Value)){
		GetValue(HKEY_CURRENT_USER, SubKey, Value, REG_SZ, pBuffer, &dwSizeOfValue);
		return true;
	} else {
		SetValue(HKEY_CURRENT_USER, SubKey, Value, REG_SZ, pBuffer, dwSizeOfValue);
	}

	return false;
}

void CRegistry::SaveString(HKEY MainKey, LPCTSTR SubKey, LPCTSTR Value, void *pBuffer, int BufSize)
{
	SetValue(HKEY_CURRENT_USER, SubKey, Value, REG_SZ, pBuffer, BufSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Open a registry key (internal only)
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef RAD_STUDIO_XE
bool CRegistry::OpenKey(HKEY MainKey, LPCTSTR SubKey, PHKEY pKey)
#else
bool CRegistry::OpenKey(HKEY MainKey, LPCTSTR SubKey, HKEY *pKey)
#endif
{
	return RegOpenKeyEx(MainKey, SubKey, 0L, KEY_ALL_ACCESS, pKey) == ERROR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Close a registry key (internal only)
////////////////////////////////////////////////////////////////////////////////////////////////
void CRegistry::CloseKey(HKEY hKey)
{
	if(hKey)
		RegCloseKey(hKey);
}


