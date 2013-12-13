#include "Wallpaper.h"

CWallpaper::CWallpaper()
{
	IsWallpaperRemoved = false;
	OriginalColor = 0;
}

CWallpaper::~CWallpaper()
{
	RestoreWallpaper();
}

void CWallpaper::RemoveWallpaper()
{
	if(IsWallpaperRemoved)
		return;

	// Black colour...
	static const DWORD BlackColor = RGB(0,0,0);

	// Save the current background color
	OriginalColor = GetSysColor(COLOR_BACKGROUND);

	// Set the desktop background color to the wanted one
	int colorID = COLOR_BACKGROUND;
	COLORREF color = BlackColor;
	SetSysColors(1, &colorID, &color);

	// Remove the desktop wallpaper, without updating the registry
	char nullchar = 0;
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, &nullchar, 0);

	//Sleep(500);
	IsWallpaperRemoved = true;
}

void CWallpaper::RestoreWallpaper()
{
	if(!IsWallpaperRemoved)
		return;

	// Restore the original desktop wallpaper
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, 0, 0);

	// Restore the original desktop color
	int colorID = COLOR_BACKGROUND;
	SetSysColors(1, &colorID, &OriginalColor);

	IsWallpaperRemoved = false;
}
