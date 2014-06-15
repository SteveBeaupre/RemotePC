#include "Settings.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

CServerSettings::CServerSettings()
{
	Type = REMOTEPC_SERVER_SETTINGS;
	FileName.Allocate(MAX_PATH);
	EraseSettings();
}

//----------------------------------------------------------------------//

void CServerSettings::EraseSettings()
{
	ZeroMemory(&Settings, sizeof(ServerSettingsStruct));
}

//----------------------------------------------------------------------//

CommonSettingsStruct* CServerSettings::GetCommonSettings()
{
	return &Settings.CommonSettings;
}

//----------------------------------------------------------------------//

void CServerSettings::ReadSettings()
{
	Settings.GUISettings.AllowControl = IniFile.ReadBoolean("GUISettings", "AllowControl", false);
	Settings.GUISettings.ConnectAsClient = IniFile.ReadBoolean("GUISettings", "ConnectAsClient", false);
	Settings.GUISettings.RemoveWallpaper = IniFile.ReadBoolean("GUISettings", "RemoveWallpaper", true);
	Settings.GUISettings.MultithreadScreenshot = IniFile.ReadBoolean("GUISettings", "MultithreadScreenshot", true);
	IniFile.EndLine();
}

//----------------------------------------------------------------------//

void CServerSettings::WriteSettings()
{
	IniFile.WriteBoolean("GUISettings", "AllowControl", Settings.GUISettings.AllowControl);
	IniFile.WriteBoolean("GUISettings", "ConnectAsClient", Settings.GUISettings.ConnectAsClient);
	IniFile.WriteBoolean("GUISettings", "RemoveWallpaper", Settings.GUISettings.RemoveWallpaper);
	IniFile.WriteBoolean("GUISettings", "MultithreadScreenshot", Settings.GUISettings.MultithreadScreenshot);
	IniFile.EndLine();
}

//----------------------------------------------------------------------//

ServerSettingsStruct* CServerSettings::GetSettings()
{
	return &Settings;
}

//----------------------------------------------------------------------//

void CServerSettings::SetSettings(ServerSettingsStruct *pSettings)
{
	memcpy(&Settings, pSettings, sizeof(ServerSettingsStruct));
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

CClientSettings::CClientSettings()
{
	Type = REMOTEPC_CLIENT_SETTINGS;
	FileName.Allocate(MAX_PATH);
	EraseSettings();
}

//----------------------------------------------------------------------//

void CClientSettings::EraseSettings()
{
	ZeroMemory(&Settings, sizeof(ClientSettingsStruct));
}

//----------------------------------------------------------------------//

CommonSettingsStruct* CClientSettings::GetCommonSettings()
{
	return &Settings.CommonSettings;
}

//----------------------------------------------------------------------//

ClientSettingsStruct* CClientSettings::GetSettings()
{
	return &Settings;
}

//----------------------------------------------------------------------//

void CClientSettings::SetSettings(ClientSettingsStruct *pSettings)
{
	memcpy(&Settings, pSettings, sizeof(ClientSettingsStruct));
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CClientSettings::ReadSettings()
{
	Settings.GUISettings.ColorDepth = IniFile.ReadBoolean("GUISettings", "ColorDepth", false);
	Settings.GUISettings.ConnectAsServer = IniFile.ReadBoolean("GUISettings", "ConnectAsServer", false);
	Settings.GUISettings.ShowFPS = IniFile.ReadBoolean("GUISettings", "ShowFPS", true);
	Settings.GUISettings.Stretch = IniFile.ReadBoolean("GUISettings", "Stretch", true);
	IniFile.EndLine();
}

//----------------------------------------------------------------------//

void CClientSettings::WriteSettings()
{
	IniFile.WriteBoolean("GUISettings", "ColorDepth", Settings.GUISettings.ColorDepth);
	IniFile.WriteBoolean("GUISettings", "ConnectAsServer", Settings.GUISettings.ConnectAsServer);
	IniFile.WriteBoolean("GUISettings", "ShowFPS", Settings.GUISettings.ShowFPS);
	IniFile.WriteBoolean("GUISettings", "Stretch", Settings.GUISettings.Stretch);
	IniFile.EndLine();
}

//----------------------------------------------------------------------//

