//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgauges"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
CVersionNumber MinimumVerNum, CurrentVerNum, UpdatesVerNum;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	if(!OneInstance.Check("REMOTE_PC_UPDATER_2014")){
		Application->Terminate();
		return;
	}

	CRawBuffer Caption(256);
	SetCaption("RemotePC Updater 2014", Caption.GetStrBuffer(), 256);
	this->Caption = AnsiString(Caption.GetStrBuffer());

	#ifdef _DEBUG
	ButtonTest->Visible = true;
	#endif
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LBClearMenuClick(TObject *Sender)
{
	ListBox->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonCloseClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------
void CFileDownloaderExt::OnProgress(double DLTotal, double DLNow, double ULTotal, double ULNow)
{
	MainForm->CGauge->MaxValue = DLTotal;
	MainForm->CGauge->Progress = DLNow;
	MainForm->CGauge->Update();
	Application->ProcessMessages();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonCheckForUpdateClick(TObject *Sender)
{
	Updater.CheckForUpdates("http://vortex666.webs.com/RemotePC/");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDownloadUpdatesClick(TObject *Sender)
{
	Updater.DownloadUpdates();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDLPatchNotesClick(TObject *Sender)
{
	Updater.DownloadPatchNotes();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonCancelClick(TObject *Sender)
{
	Updater.CancelDownload();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonTestClick(TObject *Sender)
{
	TOpenDialog *pOpnDlg = new TOpenDialog(this);
	pOpnDlg->DefaultExt = "RPC";
	pOpnDlg->Filter = "RPC files (*.rpc)|*.RPC;All files (*.*)|*.*";
	pOpnDlg->FilterIndex = 0;

	if(pOpnDlg->Execute()){
		AnsiString s = AnsiString(pOpnDlg->FileName.c_str());
		if(Updater.TestUpdateFile(s.c_str()))
			Updater.DoUpdates();
	}

	SAFE_DELETE_OBJECT(pOpnDlg);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void CUpdaterExt::OnClearMsgs()
{
	MainForm->ListBox->Clear();
}

void CUpdaterExt::OnUpdateMsgs()
{
	MainForm->ListBox->Update();
}

void CUpdaterExt::OnOutputMsg(char *txt)
{
	MainForm->ListBox->Items->Add(txt);
}

void CUpdaterExt::OnOutputMsg(const char *fmt, ...)
{
	if(fmt == NULL)
		return;

	const int BufSize = 8192;

	char TextBuf[BufSize];
	char *pTxt = &TextBuf[0];
	ZeroMemory(pTxt, BufSize);

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(pTxt, BufSize, fmt, ap);
	va_end(ap);

	MainForm->ListBox->Items->Add(pTxt);
}

void CUpdaterExt::OnClearPatchNotesMsg()
{
	PatchNotesForm->ListBox->Clear();
}

void CUpdaterExt::OnOutputPatchNotesMsg(char *txt)
{
	PatchNotesForm->ListBox->Items->Add(txt);
}

void CUpdaterExt::OnShowPatchForm()
{
	PatchNotesForm->Show();
}

void CUpdaterExt::OnDownloadCanceled()
{
	MainForm->ListBox->Items->Add("Canceling download...");
}

void CUpdaterExt::OnProcessMessages()
{
	Application->ProcessMessages();
}

void CUpdaterExt::OnUpdatesReady(int res, int PatchSize)
{
	MainForm->ButtonDownloadUpdates->Enabled = (res > 0 && PatchSize > 0);
	MainForm->ButtonCheckForUpdate->Enabled = res == 0;
}

//---------------------------------------------------------------------------
/*DWORD WINAPI DownloadPatchThreadFunc(void *param)
{
	DownloadPatchThreadStruct *pInfo = (DownloadPatchThreadStruct*)param;

	CRawBuffer Buf;

	CFileDownloader Downloader;
	if(Downloader.DownloadFile(pInfo->UpdateAddr, &Buf, &pInfo->BufSize)){
		DownloadPatchThreadLock.Lock();
		AddListboxMessageArg(MainForm->ListBox, "Reading patch part #%d...", pInfo->PartNo);
		DownloadPatchThreadLock.Unlock();
		if(pInfo->BufSize == Buf.GetSize())
			memcpy(pInfo->pBuffer, Buf.GetBuffer(), Buf.GetSize());
	}

	return 0;
}*/

