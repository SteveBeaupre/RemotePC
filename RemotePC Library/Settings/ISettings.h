#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "Registry.h"
//----------------------------------------------------------------------//

#define DEFAULT_PORT 13981

#define REG_ROOT_KEY HKEY_LOCAL_MACHINE
#define REG_PROG_KEY "SOFTWARE\\RemotePC 2014"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
#pragma pack(1)
struct LanguageSettingsStruct {
	int LangID;
};
//----------------------------------------------------------------------//
struct ConnectionSettingsStruct {
	char ip[16];
	char pw[32];
	WORD Port;
};
//----------------------------------------------------------------------//
struct WndCoordsStruct {
	int l,t,w,h;
	int ws;
};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
struct ServerSettingsStruct : public ConnectionSettingsStruct, public LanguageSettingsStruct {
	WndCoordsStruct WndCoords;
	bool ConnectAsClient;
	bool RemoveWallpaper;
	bool MultithreadScreenshot;
};
//----------------------------------------------------------------------//
struct ClientSettingsStruct : public ConnectionSettingsStruct, public LanguageSettingsStruct {
	WndCoordsStruct WndCoords;
	bool ConnectAsServer;
	bool ShowFPS;
	bool Stretch;
};
#pragma pack()
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

class IRemotePCSettings {
public:
	virtual void Load() = 0;
	virtual void Save() = 0;
};

