#include "Settings.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

CServerSettings::CServerSettings()
{
	ZeroMemory(FileName, MAX_PATH);
	sprintf(FileName, "%s", "RemotePC Server.ini");
	ZeroMemory(&Settings, sizeof(ServerSettingsStruct));
}

//----------------------------------------------------------------------//

void CServerSettings::Load(char *fname)
{
	CIniFile IniFile;
	IniFile.SetFileName(fname);

	IniFile.ReadString("ConnectionSettings", "IP", DEFAULT_IP, Settings.ip, 16);
	IniFile.ReadString("ConnectionSettings", "Password", NULL, Settings.pw, 32);
	Settings.Port = IniFile.ReadInteger("ConnectionSettings", "Port", DEFAULT_PORT);

	Settings.AllowControl = IniFile.ReadBoolean("GUISettings", "AllowControl", false);
	Settings.ConnectAsClient = IniFile.ReadBoolean("GUISettings", "ConnectAsClient", true);
	Settings.RemoveWallpaper = IniFile.ReadBoolean("GUISettings", "RemoveWallpaper", true);
	Settings.MultithreadScreenshot = IniFile.ReadBoolean("GUISettings", "MultithreadScreenshot", true);

	Settings.WndCoords.l = IniFile.ReadInteger("ScreenSettings", "WndLeft", 0);
	Settings.WndCoords.t = IniFile.ReadInteger("ScreenSettings", "WndTop", 0);
	Settings.WndCoords.w = IniFile.ReadInteger("ScreenSettings", "WndWidth", 0);
	Settings.WndCoords.h = IniFile.ReadInteger("ScreenSettings", "WndHeight", 0);
	Settings.WndCoords.wState = IniFile.ReadInteger("ScreenSettings", "WndState", 0);

	Settings.LangID = IniFile.ReadInteger("Lang", "LanguageID", 0);
}

//----------------------------------------------------------------------//

void CServerSettings::Save(char *fname)
{
	CIniFile IniFile;
	IniFile.SetFileName(fname);
	IniFile.ClearFile();

	IniFile.WriteString("ConnectionSettings", "IP", Settings.ip);
	IniFile.WriteString("ConnectionSettings", "Password", Settings.pw);
	IniFile.WriteInteger("ConnectionSettings", "Port", Settings.Port);
	IniFile.EndLine();

	IniFile.WriteBoolean("GUISettings", "AllowControl", Settings.AllowControl);
	IniFile.WriteBoolean("GUISettings", "ConnectAsClient", Settings.ConnectAsClient);
	IniFile.WriteBoolean("GUISettings", "RemoveWallpaper", Settings.RemoveWallpaper);
	IniFile.WriteBoolean("GUISettings", "MultithreadScreenshot", Settings.MultithreadScreenshot);
	IniFile.EndLine();

	IniFile.WriteInteger("ScreenSettings", "WndLeft", Settings.WndCoords.l);
	IniFile.WriteInteger("ScreenSettings", "WndTop", Settings.WndCoords.t);
	IniFile.WriteInteger("ScreenSettings", "WndWidth", Settings.WndCoords.w);
	IniFile.WriteInteger("ScreenSettings", "WndHeight", Settings.WndCoords.h);
	IniFile.WriteInteger("ScreenSettings", "WndState", Settings.WndCoords.wState);
	IniFile.EndLine();

	IniFile.WriteInteger("Lang", "LanguageID", Settings.LangID);
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
	ZeroMemory(FileName, MAX_PATH);
	sprintf(FileName, "%s", "RemotePC Client.ini");
	ZeroMemory(&Settings, sizeof(ClientSettingsStruct));
}

//----------------------------------------------------------------------//

void CClientSettings::Load(char *fname)
{
	CIniFile IniFile;
	IniFile.SetFileName(fname);

	IniFile.ReadString("ConnectionSettings", "IP", DEFAULT_IP, Settings.ip, 16);
	IniFile.ReadString("ConnectionSettings", "Password", NULL, Settings.pw, 32);
	Settings.Port = IniFile.ReadInteger("ConnectionSettings", "Port", DEFAULT_PORT);

	Settings.ConnectAsServer = IniFile.ReadBoolean("GUISettings", "ConnectAsServer", false);
	Settings.ColorDepth = IniFile.ReadInteger("GUISettings", "ColorDepth", 0);
	Settings.ShowFPS = IniFile.ReadBoolean("GUISettings", "ShowFPS", false);
	Settings.Stretch = IniFile.ReadBoolean("GUISettings", "Stretch", true);

	Settings.WndCoords.l = IniFile.ReadInteger("ScreenSettings", "WndLeft", 0);
	Settings.WndCoords.t = IniFile.ReadInteger("ScreenSettings", "WndTop", 0);
	Settings.WndCoords.w = IniFile.ReadInteger("ScreenSettings", "WndWidth", 0);
	Settings.WndCoords.h = IniFile.ReadInteger("ScreenSettings", "WndHeight", 0);
	Settings.WndCoords.wState = IniFile.ReadInteger("ScreenSettings", "WndState", 0);

	Settings.LangID = IniFile.ReadInteger("Lang", "LanguageID", 0);
}

//----------------------------------------------------------------------//

void CClientSettings::Save(char *fname)
{
	CIniFile IniFile;
	IniFile.SetFileName(fname);
	IniFile.ClearFile();

	IniFile.WriteString("ConnectionSettings", "IP", Settings.ip);
	IniFile.WriteString("ConnectionSettings", "Password", Settings.pw);
	IniFile.WriteInteger("ConnectionSettings", "Port", Settings.Port);
	IniFile.EndLine();

	IniFile.WriteInteger("GUISettings", "ColorDepth", Settings.ColorDepth);
	IniFile.WriteBoolean("GUISettings", "ConnectAsServer", Settings.ConnectAsServer);
	IniFile.WriteBoolean("GUISettings", "ShowFPS", Settings.ShowFPS);
	IniFile.WriteBoolean("GUISettings", "Stretch", Settings.Stretch);
	IniFile.EndLine();

	IniFile.WriteInteger("ScreenSettings", "WndLeft", Settings.WndCoords.l);
	IniFile.WriteInteger("ScreenSettings", "WndTop", Settings.WndCoords.t);
	IniFile.WriteInteger("ScreenSettings", "WndWidth", Settings.WndCoords.w);
	IniFile.WriteInteger("ScreenSettings", "WndHeight", Settings.WndCoords.h);
	IniFile.WriteInteger("ScreenSettings", "WndState", Settings.WndCoords.wState);
	IniFile.EndLine();

	IniFile.WriteInteger("Lang", "LanguageID", Settings.LangID);
	IniFile.EndLine();
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


