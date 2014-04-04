#pragma once
//----------------------------------------------------------------------//
#include <IWallpaper.h>
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CWallpaper : public IWallpaper{
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
