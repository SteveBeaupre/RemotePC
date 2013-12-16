//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include "cgauges.h"
//---------------------------------------------------------------------------
#include "PatchNotesUnit.h"
//---------------------------------------------------------------------------
#pragma comment(lib, "VortezSDK.lib")
#include "VortezSDK.h"
//---------------------------------------------------------------------------
#include "FileDownloader.h"
#include "VersionNumber.h"
#include "Listbox.h"
#include "CompileDateTimeStamp.h"
#include "rpc.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox;
	TButton *ButtonCheckForUpdate;
	TButton *ButtonDownloadUpdates;
	TLabel *LabelProgress;
	TCGauge *CGauge;
	TButton *ButtonClose;
	TButton *ButtonDLPatchNotes;
	TButton *ButtonCancel;
	TButton *ButtonTest;
	TPopupMenu *LBPopupMenu;
	TMenuItem *LBClearMenu;
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall ButtonDLPatchNotesClick(TObject *Sender);
	void __fastcall ButtonDownloadUpdatesClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCheckForUpdateClick(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall ButtonTestClick(TObject *Sender);
	void __fastcall LBClearMenuClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);

	void __fastcall DoUpdates(CRawBuffer* pPatchBuffer);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
class CFileDownloaderExt : public CFileDownloader {
public:
	void OnProgress(double DLTotal, double DLNow, double ULTotal, double ULNow);
};
//---------------------------------------------------------------------------
struct DownloadPatchThreadStruct {
	char  UpdateAddr[MAX_PATH];
	UINT  PartNo;
	UINT  BufSize;
	BYTE *pBuffer;
};
CLock DownloadPatchThreadLock;
//---------------------------------------------------------------------------
CFileDownloaderExt  FileDownloader;
//---------------------------------------------------------------------------
#endif
