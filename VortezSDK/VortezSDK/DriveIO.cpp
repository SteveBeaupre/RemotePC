#include "DriveIO.h"

CDriveIO::CDriveIO()
{
	hDrive = NULL;
	LoadSetFilePointerEx();
	ResetDiskInfo();
}

void CDriveIO::LoadSetFilePointerEx()
{
	pSetFilePointerEx = NULL;
	HINSTANCE hDll = LoadLibrary("kernel32.dll");
	pSetFilePointerEx = (BOOL (WINAPI*)(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod))GetProcAddress(hDll, "SetFilePointerEx");
	FreeLibrary(hDll);
}


bool CDriveIO::IsDriveLetterValid(char *Drive)
{
	if(*Drive >= 'a' && *Drive <= 'z'){
		*Drive -= 32;
		return true;
	} else if(*Drive >= 'A' && *Drive <= 'Z'){
		return true;
	}

	return false;
}


void CDriveIO::ResetDiskInfo()
{
	CursorPos = 0;
	DriveLetter = 0;
	NumBytes    = 0;
	NumSectors  = 0;
	SectorSize  = 0;
	NumBytesPerSector    = 0;
	NumSectorsPerCluster = 0;
	NumberOfClusters     = 0;
}

void CDriveIO::LoadDriveInformation(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return;}
	char szRoot[4] = {Drive,':', '\\', 0};
	
	DWORD NumFreeCluster = 0;
	GetDiskFreeSpace(&szRoot[0], &NumSectorsPerCluster, &NumBytesPerSector, &NumFreeCluster, &NumberOfClusters);

	SectorSize = NumBytesPerSector * NumSectorsPerCluster;
	NumSectors = NumberOfClusters * NumSectorsPerCluster;
	NumBytes   = SectorSize * NumSectors;
}

bool CDriveIO::IncFilePointerPosition(LARGE_INTEGER DistanceToMove)
{
	CursorPos += DistanceToMove.QuadPart;
	if(CursorPos > NumBytes)
		CursorPos = NumBytes;

	return CursorPos == NumBytes;
}

UI64 CDriveIO::Tell()
{
	return CursorPos;
}

void CDriveIO::Seek(LARGE_INTEGER DistanceToMove, DWORD MoveMethod)
{
	if(MoveMethod == 0)
		CursorPos = 0;

	if(LARGE_INTEGERToUI64(DistanceToMove) + CursorPos < NumBytes){

		pSetFilePointerEx(hDrive, DistanceToMove, NULL, MoveMethod);
		
		switch(MoveMethod)
		{
		case FILE_BEGIN: 
		case FILE_CURRENT: 
			IncFilePointerPosition(DistanceToMove); 
			break;
		case FILE_END:     
			CursorPos = NumBytes; 
			break;
		}
	} else {
		CursorPos = NumBytes; 
	}
}

void CDriveIO::Seek(UI64 uiDistanceToMove, DWORD dwMoveMethod)
{
	Seek(UI64ToLARGE_INTEGER(uiDistanceToMove), dwMoveMethod);
}

bool CDriveIO::IsOpened()
{
	return hDrive != NULL;
}

bool CDriveIO::Open(char Drive, bool WriteAccess)
{
	if(!IsDriveLetterValid(&Drive)){return false;}
	DriveLetter = Drive;
	char DrivePath[7];
	ZeroMemory(&DrivePath[0], 7);
	sprintf(&DrivePath[0], "\\\\.\\%c:", Drive);

	if(hDrive == NULL){
		switch(WriteAccess)
		{
		case false: hDrive = CreateFile(&DrivePath[0], GENERIC_READ,                 FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL   | FILE_FLAG_NO_BUFFERING, NULL); break;
		case true:  hDrive = CreateFile(&DrivePath[0], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING, NULL); break;
		}
	}
	
	if(hDrive != NULL)
		LoadDriveInformation(Drive);

	return hDrive != NULL;
}

void CDriveIO::Close()
{
	if(hDrive != NULL){
		CloseHandle(hDrive);
		ResetDiskInfo();
	}
}

DWORD CDriveIO::ReadBuffer(BYTE *pBuffer, UINT NumBytesToRead)
{
	DWORD NumBytesRead = 0;
	if(hDrive != NULL && CursorPos < NumBytes){
		ReadFile(hDrive, pBuffer, NumBytesToRead, &NumBytesRead, NULL);
		IncFilePointerPosition(DWORDToLARGE_INTEGER(NumBytesRead));
	}

	return NumBytesRead;
}

DWORD CDriveIO::WriteBuffer(BYTE *pBuffer, UINT NumBytesToWrite)
{
	DWORD NumBytesWritten = 0;
	if(hDrive != NULL && CursorPos < NumBytes){
		WriteFile(hDrive, pBuffer, NumBytesToWrite, &NumBytesWritten, NULL);
		IncFilePointerPosition(DWORDToLARGE_INTEGER(NumBytesWritten));
	}

	return NumBytesWritten;
}

DWORD CDriveIO::ReadSectors(BYTE *pBuffer, UINT NumSectorsToRead)
{
	DWORD Res = ReadBuffer(pBuffer, NumSectorsToRead * (DWORD)SectorSize);
	return Res / (DWORD)SectorSize;
}

DWORD CDriveIO::WriteSectors(BYTE *pBuffer, UINT NumSectorsToWrite)
{
	DWORD Res = WriteBuffer(pBuffer, NumSectorsToWrite * (DWORD)SectorSize);
	return Res / (DWORD)SectorSize;
}

char CDriveIO::GetOpenedDriveLetter()
{
	return DriveLetter;
}

UI64 CDriveIO::GetSize()
{
	return NumBytes;
}

UI64 CDriveIO::GetSectorsCount()
{
	return NumSectors;
}

DWORD CDriveIO::GetNumberOfClusters()
{
	return NumberOfClusters;
}

DWORD CDriveIO::GetNumBytesPerSector()
{
	return NumBytesPerSector;
}

DWORD CDriveIO::GetNumSectorsPerCluster()
{
	return NumSectorsPerCluster;
}

UI64 CDriveIO::GetNumBytesFree(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return false;}
	char szRoot[4] = {Drive,':', '\\', 0};

	DWORD dwNumSectorsPerCluster, dwNumBytesPerSector, dwNumFreeCluster, dwNumberOfClusters;
	GetDiskFreeSpace(&szRoot[0], &dwNumSectorsPerCluster, &dwNumBytesPerSector, &dwNumFreeCluster, &dwNumberOfClusters);

	return dwNumFreeCluster * (dwNumBytesPerSector * dwNumSectorsPerCluster);
}

UI64 CDriveIO::GetNumSectorFree(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return false;}
	char szRoot[4] = {Drive,':', '\\', 0};

	DWORD dwNumSectorsPerCluster, dwNumBytesPerSector, dwNumFreeCluster, dwNumberOfClusters;
	GetDiskFreeSpace(&szRoot[0], &dwNumSectorsPerCluster, &dwNumBytesPerSector, &dwNumFreeCluster, &dwNumberOfClusters);

	return dwNumFreeCluster * dwNumSectorsPerCluster;
}

UI64 CDriveIO::GetNumBytesUsed(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return false;}
	char szRoot[4] = {Drive,':', '\\', 0};

	DWORD dwNumSectorsPerCluster, dwNumBytesPerSector, dwNumFreeCluster, dwNumberOfClusters;
	GetDiskFreeSpace(&szRoot[0], &dwNumSectorsPerCluster, &dwNumBytesPerSector, &dwNumFreeCluster, &dwNumberOfClusters);

	return (dwNumBytesPerSector * dwNumSectorsPerCluster * dwNumberOfClusters) - (dwNumFreeCluster * (dwNumBytesPerSector * dwNumSectorsPerCluster));
}

UI64 CDriveIO::GetNumSectorUsed(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return false;}
	char szRoot[4] = {Drive,':', '\\', 0};

	DWORD dwNumSectorsPerCluster, dwNumBytesPerSector, dwNumFreeCluster, dwNumberOfClusters;
	GetDiskFreeSpace(&szRoot[0], &dwNumSectorsPerCluster, &dwNumBytesPerSector, &dwNumFreeCluster, &dwNumberOfClusters);

	return (dwNumSectorsPerCluster * dwNumberOfClusters) - (dwNumFreeCluster * dwNumSectorsPerCluster);
}