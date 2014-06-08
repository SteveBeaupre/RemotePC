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
#include "OneInstance.h"
#include "UpdaterCls.h"
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
	COneInstance OneInstance;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;

class CUpdaterExt : public CUpdater
{
public:
	void OnClearPatchNotesMsg();
	void OnOutputPatchNotesMsg(char *txt);

	void OnClearMsgs();
	void OnUpdateMsgs();
	void OnOutputMsg(char *txt);
	void OnOutputMsg(const char *fmt, ...);
	void OnShowPatchForm();
	void OnDownloadCanceled();
	void OnProcessMessages();
	void OnUpdatesReady(int res, int PatchSize);
};
CUpdaterExt Updater;

#endif
