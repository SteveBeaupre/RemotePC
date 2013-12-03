#include "WinVersion.h"

DWORD CWinVersion::DetectWindowsVersion()
{
	OSVERSIONINFO OSversion;
		
	OSversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSversion);

	switch(OSversion.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s: 
		return _WIN_31_;

	case VER_PLATFORM_WIN32_WINDOWS:
		if(OSversion.dwMinorVersion == 0){
			return _WIN_95_; 
		} else {
			if(OSversion.dwMinorVersion == 10){
				return _WIN_98_;
			} else {
				if(OSversion.dwMinorVersion == 90)  
					return _WIN_ME_;
			}
		}

	case VER_PLATFORM_WIN32_NT:
		if(OSversion.dwMajorVersion==5 && OSversion.dwMinorVersion==0){
			return _WIN_2000_;
		} else {
			if(OSversion.dwMajorVersion==5 &&   OSversion.dwMinorVersion==1){
				return _WIN_XP_;
			} else {
				if(OSversion.dwMajorVersion<=4)
					return _WIN_NT_;			
				/*else	
					return _WIN_7_;*/
				else if(OSversion.dwMajorVersion == 6)
					return _WIN_VISTA_;
				else if(OSversion.dwMajorVersion == 7)
					return _WIN_7_;
			}
		}
	}

	return _WIN_UNKNOW_;
}
