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
	ServerSettingsStruct Settings;
public:
	void Load();
	void Save();
public:
	ServerSettingsStruct* GetSettings();
	void SetSettings(ServerSettingsStruct *pSettings);
};

class EXP_FUNC CClientSettings : public IRemotePCSettings {
public:
	CClientSettings();
private:
	ClientSettingsStruct Settings;
public:
	void Load();
	void Save();
public:
	ClientSettingsStruct* GetSettings();
	void SetSettings(ClientSettingsStruct *pSettings);
};