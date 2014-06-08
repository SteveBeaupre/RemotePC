#pragma once
//---------------------------------------------------------------------------
#pragma comment(lib, "VortezSDK.lib")
#include "VortezSDK.h"
//---------------------------------------------------------------------------
#include "Windows.h"
#include "stdio.h"
//---------------------------------------------------------------------------
#include "FileDownloader.h"
#include "VersionNumber.h"
#include "VersionNumberCls.h"
//---------------------------------------------------------------------------

class CFileDownloaderExt : public CFileDownloader {
public:
	void OnProgress(double DLTotal, double DLNow, double ULTotal, double ULNow);
};

class CUpdater
{
public:
	CUpdater();
private:
	CVersionNumber MinimumVerNum, CurrentVerNum, UpdatesVerNum;
	CFileDownloaderExt FileDownloader;

	CRawBuffer PatchBuffer;

	bool IsValidURLChar(char c);
	void FormatULR(char *pURL, UINT bufsize);

	void GenUpdateULR();
	void GenPatchNoteULR();
	char* GetCurrentVersion();

	void UpdateULRBuffer(CRawBuffer *pRawBuffer);
	void UpdatePatchNoteBuffer(CRawBuffer *pRawBuffer);
	void UpdateCurrentVersion();
public:
public:
	void DoUpdates();
	bool DownloadUpdates();
	bool DownloadPatchNotes();
	void CancelDownload();
	bool CheckForUpdates(char *pURL);
	bool TestUpdateFile(char *fname);
public:
	virtual void OnClearPatchNotesMsg(){}
	virtual void OnOutputPatchNotesMsg(char *txt){}

	virtual void OnClearMsgs(){}
	virtual void OnUpdateMsgs(){}
	virtual void OnOutputMsg(char *txt){}
	virtual void OnOutputMsg(const char *fmt, ...){}
	virtual void OnShowPatchForm(){}
	virtual void OnDownloadCanceled(){}
	virtual void OnProcessMessages(){}
	virtual void OnUpdatesReady(int res, int PatchSize){}
};
