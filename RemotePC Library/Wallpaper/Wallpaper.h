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
	void Remove();
	void Restore();
};
