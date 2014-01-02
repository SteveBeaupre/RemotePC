#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//

class IWallpaper {
	virtual void Remove()  = 0;
	virtual void Restore() = 0;
};
