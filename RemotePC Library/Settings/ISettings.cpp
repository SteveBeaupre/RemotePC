#include "ISettings.h"

IRemotePCSettings::IRemotePCSettings()
{
	Type = 0;
	FileName.Allocate(MAX_PATH);
	EraseSettings();
}


void IRemotePCSettings::ReadGlobalSettings()
{
	CommonSettingsStruct *pCommonSettings = GetCommonSettings();

	IniFile.ReadString("ConnectionSettings", "IP", DEFAULT_IP, pCommonSettings->ConnectionSettings.ip, 16);
	IniFile.ReadString("ConnectionSettings", "Password", NULL, pCommonSettings->ConnectionSettings.pw, 32);
	pCommonSettings->ConnectionSettings.Port = IniFile.ReadInteger("ConnectionSettings", "Port", DEFAULT_PORT);

	ReadSettings();

	pCommonSettings->WndCoords.l = IniFile.ReadInteger("ScreenSettings", "WndLeft", 0);
	pCommonSettings->WndCoords.t = IniFile.ReadInteger("ScreenSettings", "WndTop", 0);
	pCommonSettings->WndCoords.w = IniFile.ReadInteger("ScreenSettings", "WndWidth", 0);
	pCommonSettings->WndCoords.h = IniFile.ReadInteger("ScreenSettings", "WndHeight", 0);
	pCommonSettings->WndCoords.wState = IniFile.ReadInteger("ScreenSettings", "WndState", 0);

	pCommonSettings->LangID = IniFile.ReadInteger("Lang", "LanguageID", 0);
}

void IRemotePCSettings::WriteGlobalSettings()
{
	CommonSettingsStruct *pCommonSettings = GetCommonSettings();

	IniFile.WriteString("ConnectionSettings", "IP", pCommonSettings->ConnectionSettings.ip);
	IniFile.WriteString("ConnectionSettings", "Password", pCommonSettings->ConnectionSettings.pw);
	IniFile.WriteInteger("ConnectionSettings", "Port", pCommonSettings->ConnectionSettings.Port);
	IniFile.EndLine();

	WriteSettings();

	IniFile.WriteInteger("ScreenSettings", "WndLeft", pCommonSettings->WndCoords.l);
	IniFile.WriteInteger("ScreenSettings", "WndTop", pCommonSettings->WndCoords.t);
	IniFile.WriteInteger("ScreenSettings", "WndWidth", pCommonSettings->WndCoords.w);
	IniFile.WriteInteger("ScreenSettings", "WndHeight", pCommonSettings->WndCoords.h);
	IniFile.WriteInteger("ScreenSettings", "WndState", pCommonSettings->WndCoords.wState);
	IniFile.EndLine();

	IniFile.WriteInteger("Lang", "LanguageID", pCommonSettings->LangID);
}


void IRemotePCSettings::Load(char *fname)
{
	IniFile.SetFileName(fname);
	ReadGlobalSettings();
}

void IRemotePCSettings::Save(char *fname)
{
	IniFile.SetFileName(fname);
	WriteGlobalSettings();
}

