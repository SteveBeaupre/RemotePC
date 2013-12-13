#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//

class IWallpaper {
	virtual void RemoveWallpaper()  = 0;
	virtual void RestoreWallpaper() = 0;
};
