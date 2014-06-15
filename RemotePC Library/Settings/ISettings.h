#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "IniFile.h"
//----------------------------------------------------------------------//
#define DEFAULT_PORT  13981
#define DEFAULT_IP    "127.0.0.1"
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
#pragma pack(1)
//----------------------------------------------------------------------//
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
	int wState;
};
//----------------------------------------------------------------------//
//------------------? why did i use inheritance here ?------------------//
struct ServerSettingsStruct : public ConnectionSettingsStruct, public LanguageSettingsStruct {
	WndCoordsStruct WndCoords;
	bool ConnectAsClient;
	bool RemoveWallpaper;
	bool MultithreadScreenshot;
	bool AllowControl;
};
//----------------------------------------------------------------------//
struct ClientSettingsStruct : public ConnectionSettingsStruct, public LanguageSettingsStruct {
	WndCoordsStruct WndCoords;
	UINT ColorDepth;
	bool ConnectAsServer;
	bool ShowFPS;
	bool Stretch;
};
//----------------------------------------------------------------------//
#pragma pack()
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

class IRemotePCSettings {
public:
	virtual void Load(char *fname) = 0;
	virtual void Save(char *fname) = 0;
};

