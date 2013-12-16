//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
const HKEY RootKey = HKEY_LOCAL_MACHINE;
LPCSTR SoftwareKey = "Software\\RemotePC 2014";
LPCSTR UpdaterToolKey = "Software\\RemotePC 2014\\PatchMaker\\";
//---------------------------------------------------------------------------
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	char szCaption[256];
	ZeroMemory(szCaption, 256);
	SetCaption("RemotePC Patch Maker 2014", szCaption, 256);
	this->Caption = AnsiString(szCaption);

	LoadSettings();
	CheckBoxSplit->OnClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	SaveSettings();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadSettings()
{
	CRegistry Registry;
	if(!Registry.DoesKeyExist(RootKey, UpdaterToolKey)){
		Registry.CreateKey(RootKey, UpdaterToolKey);
	}

	UINT iSplit = 0;
	UINT iSplitValue = 0;
	UINT iSplitMulti = 0;
	UINT iAutoGenPathName = 0;
	char szVersion[VERSION_SIZE];
	char szPatchName[MAX_PATH];
	char szInputFileName[MAX_PATH];
	ZeroMemory(&szVersion[0], VERSION_SIZE);
	ZeroMemory(&szPatchName[0], MAX_PATH);
	ZeroMemory(&szInputFileName[0], MAX_PATH);

	Registry.LoadString(RootKey, UpdaterToolKey, "Version", &szVersion[0], VERSION_SIZE, "1.0.0");
	EditVersion->Text = AnsiString(&szVersion[0]);

	Registry.LoadString(RootKey, UpdaterToolKey, "PatchName", &szPatchName[0], MAX_PATH, "Patch 1.0.0");
	EditPatchName->Text = AnsiString(&szPatchName[0]);

	Registry.LoadString(RootKey, UpdaterToolKey, "InputFileName", &szInputFileName[0], MAX_PATH, "");
	EditInputFileName->Text = AnsiString(&szInputFileName[0]);

	Registry.LoadInteger(RootKey, UpdaterToolKey, "AutoGenPathName", &iAutoGenPathName, 1);
	CheckBoxAutoGenPathName->Checked = iAutoGenPathName != 0;

	Registry.LoadInteger(RootKey, UpdaterToolKey, "Split", &iSplit, 0);
	Registry.LoadInteger(RootKey, UpdaterToolKey, "SplitValue", &iSplitValue, 1);
	Registry.LoadInteger(RootKey, UpdaterToolKey, "SplitMulti", &iSplitMulti, 2);
	CheckBoxSplit->Checked = iSplit != 0;
	SpinEditPiecesSize->Value = iSplitValue;
	ComboBoxPiecesSize->ItemIndex = iSplitMulti;

	CheckBoxAutoGenPathName->OnClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveSettings()
{
	CRegistry Registry;
	if(!Registry.DoesKeyExist(RootKey, UpdaterToolKey)){
		Registry.CreateKey(RootKey, UpdaterToolKey);
	}

	char szVersion[VERSION_SIZE];
	char szPatchName[MAX_PATH];
	char szInputFileName[MAX_PATH];
	UINT iAutoGenPathName = CheckBoxAutoGenPathName->Checked ? 1 : 0;
	UINT iSplit = CheckBoxSplit->Checked ? 1 : 0;
	UINT iSplitValue = SpinEditPiecesSize->Value;
	UINT iSplitMulti = ComboBoxPiecesSize->ItemIndex;

	ConvertUnicodeToChar(&szVersion[0], VERSION_SIZE, EditVersion->Text.c_str());
	ConvertUnicodeToChar(&szPatchName[0], MAX_PATH, EditPatchName->Text.c_str());
	ConvertUnicodeToChar(&szInputFileName[0], MAX_PATH, EditInputFileName->Text.c_str());

	Registry.SaveString(RootKey, UpdaterToolKey, "Version", &szVersion[0], VERSION_SIZE);
	Registry.SaveString(RootKey, UpdaterToolKey, "PatchName", &szPatchName[0], MAX_PATH);
	Registry.SaveString(RootKey, UpdaterToolKey, "InputFileName", &szInputFileName[0], MAX_PATH);
	Registry.SaveInteger(RootKey, UpdaterToolKey, "AutoGenPathName", &iAutoGenPathName);
	Registry.SaveInteger(RootKey, UpdaterToolKey, "Split", &iSplit);
	Registry.SaveInteger(RootKey, UpdaterToolKey, "SplitValue", &iSplitValue);
	Registry.SaveInteger(RootKey, UpdaterToolKey, "SplitMulti", &iSplitMulti);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonBrowseInputClick(TObject *Sender)
{
	if(OpenDialogInput->Execute(this->Handle))
	   EditInputFileName->Text = OpenDialogInput->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonMakeClick(TObject *Sender)
{
	CFileManager FileManager;
	CFileIO InputFile, OutputFile;

	char Version[VERSION_SIZE];
	char PatchName[MAX_PATH];
	char InputFileName[MAX_PATH];
	char OutputFileName[MAX_PATH];
	ConvertUnicodeToChar(&Version[0],    VERSION_SIZE, EditVersion->Text.c_str());
	ConvertUnicodeToChar(&PatchName[0],      MAX_PATH, EditPatchName->Text.c_str());
	ConvertUnicodeToChar(&InputFileName[0],  MAX_PATH, EditInputFileName->Text.c_str());

	if(strlen(&Version[0]) != 5){
		ShowMessage("Error: Invalid version number. (x.x.x)");
		return;
	}

	if(strlen(&PatchName[0]) == 0){
		ShowMessage("Error: You must enter a patch file name.");
		return;
	}

	if(strlen(&InputFileName[0]) == 0){
		ShowMessage("Error: You must enter an input file name.");
		return;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	SaveDialogOutput->FileName = AnsiString("Update");

	// Ask the output file name
	if(SaveDialogOutput->Execute()){
		ConvertUnicodeToChar(&OutputFileName[0], MAX_PATH, SaveDialogOutput->FileName.c_str());
		if(!OutputFile.OpenForWriting(&OutputFileName[0])){
			ShowMessage("Error: Unable to create the output file.");
			return;
		}
	} else {
		return;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	UINT InputFileSize = 0;
	CRawBuffer InputBuffer;

	// Make sur the input file exist before reading it
	if(FileManager.DoesExist(InputFileName)){

		// Get the file size
		InputFileSize = FileManager.GetSize(InputFileName);

		// Open the file if size > 0
		if(InputFileSize > 0 && InputFile.OpenForReading(InputFileName)){
			// Read it in the Input buffer
			InputBuffer.Allocate(InputFileSize);
			InputFile.Read(InputBuffer.GetBuffer(), InputFileSize);
			InputFile.Close();
		} else {
			OutputFile.Close();
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	RPCHeader Header;

	DWORD Sig[SIGNATURE_SIZE/4] = {0x6E8BB3CC, 0x09EF018F, 0x644C3091, 0x1E68478B};

	memcpy(&Header.Signature[0], &Sig[0], SIGNATURE_SIZE);
	memcpy(&Header.Version[0], &Version[0], VERSION_SIZE);

	Header.PatchHash = InputBuffer.Hash();
	InputBuffer.Encrypt(0xDEADC0DE);
	Header.PatchSize = InputFileSize;

	if(!CheckBoxSplit->Checked){
		Header.PiecesSize = InputFileSize;
	} else {
		Header.PiecesSize = SpinEditPiecesSize->Value;
		switch(ComboBoxPiecesSize->ItemIndex) // don't add breaks! NOT A BUG FOR ONCE !!!
		{
		case 3: Header.PiecesSize *= 1024;
		case 2: Header.PiecesSize *= 1024;
		case 1: Header.PiecesSize *= 1024;
		}

		UINT NumFilesToCreate = Header.PatchSize / Header.PiecesSize;
		if(Header.PatchSize % Header.PiecesSize != 0)
			NumFilesToCreate++;

		char ErrMsg[MAX_PATH];
		ZeroMemory(ErrMsg, MAX_PATH);
		sprintf(ErrMsg, "Warning: %d files will be created.\nContinue?", NumFilesToCreate);

		if(MessageBoxA(Handle, ErrMsg, "Warning.", MB_YESNOCANCEL) != ID_YES){
			return;
		}
	}
	Header.PatchNameSize = strlen(&PatchName[0]);

	Gauge->MaxValue = sizeof(RPCHeader) + Header.PatchNameSize + InputBuffer.GetSize();
	Gauge->Progress = 0;

	OutputFile.Write(&Header, sizeof(RPCHeader));
	Gauge->Progress = sizeof(RPCHeader);

	OutputFile.Write(&PatchName[0], Header.PatchNameSize);
	Gauge->Progress = sizeof(RPCHeader) + Header.PatchNameSize;

	OutputFile.Close();

	UINT i = 0;
	UINT j = 0;
	while(i < InputBuffer.GetSize()){

		char fname[MAX_PATH];
		ZeroMemory(&fname[0], MAX_PATH);
		sprintf(fname, "%s.%3.3d", &PatchName[0], j++);

		if(OutputFile.OpenForWriting(fname)){

			UINT NumBytesToWrite = InputBuffer.GetSize() - i;
			if(NumBytesToWrite > Header.PiecesSize)
				NumBytesToWrite = Header.PiecesSize;

			int res = OutputFile.Write(InputBuffer.GetBuffer(), NumBytesToWrite);

			i += res;
		}
	}

	OutputFile.Close();

	Gauge->Progress = Gauge->MaxValue;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBoxAutoGenPathNameClick(TObject *Sender)
{
	EditPatchName->Enabled = !CheckBoxAutoGenPathName->Checked;
	if(CheckBoxAutoGenPathName->Checked)
    	EditVersion->OnChange(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::EditVersionChange(TObject *Sender)
{
	if(CheckBoxAutoGenPathName->Checked){
		 char Buf[MAX_PATH];
		 ZeroMemory(&Buf[0], MAX_PATH);

		 char Version[VERSION_SIZE];
		 ConvertUnicodeToChar(&Version[0], VERSION_SIZE, EditVersion->Text.c_str());

		 sprintf(&Buf[0], "Patch %s", &Version[0]);

		 EditPatchName->Text = AnsiString(&Buf[0]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBoxSplitClick(TObject *Sender)
{
	ComboBoxPiecesSize->Enabled = CheckBoxSplit->Checked;
	SpinEditPiecesSize->Enabled = CheckBoxSplit->Checked;
}
//---------------------------------------------------------------------------

