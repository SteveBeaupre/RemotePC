#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Windows.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "FileIO.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CIniFile
{
public:
	CIniFile();
private:
	char FileName[MAX_PATH];
	char StrBuffer[8192];
	bool CompareStr(char *s1, char* s2);
public:
	bool DoesFileExists(char *fname);

	void SetFileName(char *fname);
	void EraseFileName();

	void ClearFile();
	void EndLine();

	void  ReadString(char *pSection, char *pKey, char *pDefault, char *pValue, int BufSize);
	char* ReadString(char *pSection, char *pKey, char *pDefault = NULL);
	int   ReadInteger(char *pSection, char *pKey, int Default = 0);
	bool  ReadBoolean(char *pSection, char *pKey, bool pDefault = false);

	bool WriteString(char *pSection, char *pKey, char *pValue = NULL);
	bool WriteString(char *pSection, char *pKey, const char *fmt, ...);
	bool WriteInteger(char *pSection, char *pKey, int Value = 0);
	bool WriteBoolean(char *pSection, char *pKey, bool Value = false);
};
