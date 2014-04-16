#include "Settings.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

CServerSettings::CServerSettings()
{
	ZeroMemory(&Settings, sizeof(ServerSettingsStruct));
}

//----------------------------------------------------------------------//

void CServerSettings::Load()
{
	CRegistry Reg;

	// Check if the key exist
	if(Reg.DoesKeyExist(REG_ROOT_KEY, REG_PROG_KEY)){
		
		// Check if the value exist
		if(Reg.DoesValueExist(REG_ROOT_KEY, REG_PROG_KEY, "Server")){
		
			// If so, load it from the registry
			DWORD size = sizeof(ServerSettingsStruct);
			Reg.GetValue(REG_ROOT_KEY, REG_PROG_KEY, "Server", REG_BINARY, &Settings, &size);
			// If the size dosen't match, use default values
			if(size != sizeof(ServerSettingsStruct))
				goto CreateValueJmp;
		} else {
			goto CreateValueJmp;
		}
	} else {
	
		// Create the key
		Reg.CreateKey(REG_ROOT_KEY, REG_PROG_KEY);

		// create value using default settings
		CreateValueJmp:
		ZeroMemory(&Settings, sizeof(ServerSettingsStruct));

		Settings.Port = DEFAULT_PORT;
		Settings.RemoveWallpaper = true;
		Settings.AllowControl = true;

		#ifdef RAD_STUDIO_XE
		snprintf(Settings.ip, 16, "192.168.0.1");
		#else
		sprintf(Settings.ip, "192.168.0.1");
		#endif
	
		Reg.SetValue(REG_ROOT_KEY, REG_PROG_KEY, "Server", REG_BINARY, &Settings, sizeof(ServerSettingsStruct));
	}
}

//----------------------------------------------------------------------//

void CServerSettings::Save()
{
	CRegistry Reg;

	// If the key dosen't exist, create it
	if(!Reg.DoesKeyExist(REG_ROOT_KEY, REG_PROG_KEY))
		Reg.CreateKey(REG_ROOT_KEY, REG_PROG_KEY);
		
	// Write settings to the registry
	Reg.SetValue(REG_ROOT_KEY, REG_PROG_KEY, "Server", REG_BINARY, &Settings, sizeof(ServerSettingsStruct));
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
	ZeroMemory(&Settings, sizeof(ClientSettingsStruct));
}

//----------------------------------------------------------------------//

void CClientSettings::Load()
{
	CRegistry Reg;

	// Check if the key exist
	if(Reg.DoesKeyExist(REG_ROOT_KEY, REG_PROG_KEY)){
		
		// Check if the value exist
		if(Reg.DoesValueExist(REG_ROOT_KEY, REG_PROG_KEY, "Client")){
		
			// If so, load it from the registry
			DWORD size = sizeof(ClientSettingsStruct);
			Reg.GetValue(REG_ROOT_KEY, REG_PROG_KEY, "Client", REG_BINARY, &Settings, &size);
			// If the size dosen't match, use default values
			if(size != sizeof(ClientSettingsStruct))
				goto CreateValueJmp;
		} else {
			goto CreateValueJmp;
		}
	} else {
	
		// Create the key
		Reg.CreateKey(REG_ROOT_KEY, REG_PROG_KEY);

		// create value using default settings
		CreateValueJmp:
		ZeroMemory(&Settings, sizeof(ClientSettingsStruct));

		Settings.Port = DEFAULT_PORT;
		Settings.Stretch = true;

		#ifdef RAD_STUDIO_XE
		snprintf(Settings.ip, 16, "192.168.0.1");
		#else
		sprintf(Settings.ip, "192.168.0.1");
		#endif
	
		Reg.SetValue(REG_ROOT_KEY, REG_PROG_KEY, "Client", REG_BINARY, &Settings, sizeof(ClientSettingsStruct));
	}
}

//----------------------------------------------------------------------//

void CClientSettings::Save()
{
	CRegistry Reg;

	// If the key dosen't exist, create it
	if(!Reg.DoesKeyExist(REG_ROOT_KEY, REG_PROG_KEY))
		Reg.CreateKey(REG_ROOT_KEY, REG_PROG_KEY);
		
	// Write settings to the registry
	Reg.SetValue(REG_ROOT_KEY, REG_PROG_KEY, "Client", REG_BINARY, &Settings, sizeof(ClientSettingsStruct));
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


