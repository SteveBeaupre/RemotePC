//---------------------------------------------------------------------------
#pragma hdrstop
#include "RemotePCGUI.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void GetAppDir(char *AppDir, UINT BufSize)
{
	CFolderManager FolderManager;
	ZeroMemory(AppDir, BufSize);
	FolderManager.GetCurrent(&AppDir, BufSize);
}

AnsiString GenSaveFileName(AnsiString AppDir, AnsiString SaveFileName)
{
	return (AppDir + "\\" + SaveFileName);
}

void FormatNetworkSpeedStats(CNetStats* pStats, TStatusBar *StatusBar)
{
	pStats->UpdateStats();

	char* TD = pStats->GetTotalDownloaded();
	char* TU = pStats->GetTotalUploaded();
	char* AD = pStats->GetAverageDownloadKBPS();
	char* AU = pStats->GetAverageUploadKBPS();

	char Tot[256];
	char Avg[256];
	ZeroMemory(Tot, 256);
	ZeroMemory(Avg, 256);

	sprintf(Tot, "D: %s Byte  T: %s", AD, TD);
	sprintf(Avg, "U: %s Byte  T: %s", AU, TU);

	StatusBar->Panels->Items[1]->Text = AnsiString(Tot);
	StatusBar->Panels->Items[2]->Text = AnsiString(Avg);
}

void AddListboxMessageArg(TListBox *pListBox, TStatusBar *pStatusBar, const char *fmt, ...)
{
	if(fmt == NULL)
		return;

	const int BufSize = 2048;

	char TextBuf[BufSize];
	char *pTxt = &TextBuf[0];
	ZeroMemory(pTxt, BufSize);

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(pTxt, BufSize, fmt, ap);
	va_end(ap);

	pListBox->Items->Add(pTxt);
	pStatusBar->Panels->Items[0]->Text = pTxt;
}
