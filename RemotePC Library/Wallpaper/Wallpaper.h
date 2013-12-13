#pragma once
//----------------------------------------------------------------------//
#include <IWallpaper.h>
//----------------------------------------------------------------------//

class CWallpaper : public IWallpaper{
public:
	CWallpaper();
	~CWallpaper();
private:
	bool  IsWallpaperRemoved;
	DWORD OriginalColor;
public:
	void RemoveWallpaper();
	void RestoreWallpaper();
};
