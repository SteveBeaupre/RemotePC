#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "ISettings.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CServerSettings : public IRemotePCSettings {
public:
	CServerSettings();
private:
	char FileName[MAX_PATH];
	ServerSettingsStruct Settings;
public:
	void Load(char *fname);
	void Save(char *fname);
public:
	ServerSettingsStruct* GetSettings();
	void SetSettings(ServerSettingsStruct *pSettings);
};

class EXP_FUNC CClientSettings : public IRemotePCSettings {
public:
	CClientSettings();
private:
	char FileName[MAX_PATH];
	ClientSettingsStruct Settings;
public:
	void Load(char *fname);
	void Save(char *fname);
public:
	ClientSettingsStruct* GetSettings();
	void SetSettings(ClientSettingsStruct *pSettings);
};