#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "IniFile.h"
#include "RawBuffer.h"
//----------------------------------------------------------------------//
#define DEFAULT_PORT  13981
#define DEFAULT_IP    "127.0.0.1"
//----------------------------------------------------------------------//
#define REMOTEPC_SERVER_SETTINGS  1
#define REMOTEPC_CLIENT_SETTINGS  2
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
#pragma pack(1)
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
struct ServerGUISettingsStruct {

	bool ConnectAsClient;
	bool RemoveWallpaper;
	bool MultithreadScreenshot;
	bool AllowControl;
};
//----------------------------------------------------------------------//
struct ClientGUISettingsStruct {
	UINT ColorDepth;
	bool ConnectAsServer;
	bool ShowFPS;
	bool Stretch;
};
//----------------------------------------------------------------------//
struct CommonSettingsStruct {
	ConnectionSettingsStruct ConnectionSettings;
	WndCoordsStruct WndCoords;
	int LangID;
};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
struct ServerSettingsStruct {
	ServerGUISettingsStruct GUISettings;
	CommonSettingsStruct CommonSettings;
};
//----------------------------------------------------------------------//
struct ClientSettingsStruct {
	ClientGUISettingsStruct GUISettings;
	CommonSettingsStruct CommonSettings;
};
//----------------------------------------------------------------------//
#pragma pack()
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

class IRemotePCSettings {
public:
	IRemotePCSettings();
protected:
	int Type;
	CRawBuffer FileName;
private:
	void ReadBasicSettings();
	void WriteBasicSettings();
protected:
	CIniFile IniFile;

	void ReadGlobalSettings();
	void WriteGlobalSettings();

	virtual void ReadSettings(){}
	virtual void WriteSettings(){}
	virtual void EraseSettings(){}
	virtual CommonSettingsStruct* GetCommonSettings(){return NULL;}
public:
	void Load(char *fname);
	void Save(char *fname);
};

