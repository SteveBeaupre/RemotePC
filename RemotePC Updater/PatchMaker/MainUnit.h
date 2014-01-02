//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Samples.Gauges.hpp>
#include <Vcl.Samples.Spin.hpp>
//---------------------------------------------------------------------------
#pragma comment(lib, "VortezSDK.lib")
#include "VortezSDK.h"
//---------------------------------------------------------------------------
#include "VersionNumber.h"
#include "UnicodeToChar.h"
#include "CompileDateTimeStamp.h"
//---------------------------------------------------------------------------
#include "rpc.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialogInput;
	TButton *ButtonMake;
	TGauge *Gauge;
	TSaveDialog *SaveDialogOutput;
	TGroupBox *GroupBox1;
	TEdit *EditVersion;
	TGroupBox *GroupBox2;
	TButton *ButtonBrowseInput;
	TEdit *EditInputFileName;
	TGroupBox *GroupBox3;
	TEdit *EditPatchName;
	TCheckBox *CheckBoxAutoGenPathName;
	TGroupBox *GroupBox4;
	TComboBox *ComboBoxPiecesSize;
	TSpinEdit *SpinEditPiecesSize;
	TCheckBox *CheckBoxSplit;
	void __fastcall ButtonBrowseInputClick(TObject *Sender);
	void __fastcall ButtonMakeClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CheckBoxAutoGenPathNameClick(TObject *Sender);
	void __fastcall EditVersionChange(TObject *Sender);
	void __fastcall CheckBoxSplitClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);

	void __fastcall LoadSettings();
	void __fastcall SaveSettings();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
