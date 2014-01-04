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
LPCSTR BaseURL = "http://vortex666.webs.com/RemotePC/";
//---------------------------------------------------------------------------
LPSTR UpdateFileName     = "Update.rpc";
LPSTR PatchNotesFileName = "PatchNotes.txt";
//---------------------------------------------------------------------------
HKEY   RootKey = HKEY_LOCAL_MACHINE;
LPCSTR RegKey  = "SOFTWARE\\RemotePC 2014";
LPCSTR RegVer  = "Version";
//---------------------------------------------------------------------------
CVersionNumber MinimumVerNum, CurrentVerNum, UpdatesVerNum;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool IsValidURLChar(char c)
{
	static LPCSTR LegalChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~://?#[]@!$&'()*+,;=";
	static const int LegalCharsLen = strlen(LegalChars);

	bool legal = false;
	for(int i = 0; i < LegalCharsLen; i++){
		if(c == LegalChars[i]){
			legal = true;
			break;
		}
	}

	return legal;
}
//---------------------------------------------------------------------------
// This function might be unsafe (only 260 char), but should work most of the time...
void FormatULR(char *pURL, UINT bufsize)
{
	char *out = new BYTE[bufsize];
	ZeroMemory(out, bufsize);

	UINT j = 0;
	for(UINT i = 0; i < bufsize; i++){

		char c = pURL[i];

		if(i == bufsize || c == NULL)
			break;

		if(IsValidURLChar(c)){
			out[j++] = c;
		} else {
			if(j+2 < bufsize){
				out[j++] = 37;

				char tmp[8];
				ZeroMemory(&tmp[0], 8);
				sprintf(&tmp[0], "%x", c);

				UINT k = 0;
				while(k < strlen(tmp) && k < 8)
					out[j++] = tmp[k++];
			} else {
				break;
			}
		}
	}
	// Make sure the null terminator is there
	out[bufsize-1] = NULL;

	memcpy(pURL, out, bufsize);
}
//---------------------------------------------------------------------------
bool GetCurrentVersion()
{
	CurrentVerNum.Erase();

	CRegistry Registry;
	if(!Registry.DoesKeyExist(RootKey, RegKey))
		return false;

	if(!Registry.DoesValueExist(RootKey, RegKey, RegVer))
		return false;

	DWORD BufSize = 64;
	CRawBuffer Version(BufSize);
	Registry.GetValue(RootKey, RegKey, RegVer, REG_SZ, Version.GetStrBuffer(), &BufSize);

	CurrentVerNum.Scan(Version.GetStrBuffer());

	return true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	char szCaption[256];
	ZeroMemory(szCaption, 256);
	SetCaption("RemotePC Updater 2014", szCaption, 256);
	this->Caption = AnsiString(szCaption);

	MinimumVerNum.Set(1, 0, 0);

	if(!GetCurrentVersion()){
		// version not found, so write "1.0.0"...
		CRawBuffer Buf(VERSION_SIZE);
		sprintf(Buf.GetBuffer(), "1.0.0");

		CRegistry Registry;
		Registry.SaveString(RootKey, RegKey, RegVer, Buf.GetBuffer(), Buf.GetSize());
		CurrentVerNum.Set(1,0,0);
	} else {
		char szCurrentVer[256];
		ZeroMemory(&szCurrentVer[0], 256);
		sprintf(&szCurrentVer[0], "Current Version: %d.%d.%d", CurrentVerNum.GetHi(), CurrentVerNum.GetMd(), CurrentVerNum.GetLo());
		ListBox->Items->Add(&szCurrentVer[0]);
	}

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
	ListBox->Clear();

	const UINT DefBufferSize = SIGNATURE_SIZE + VERSION_SIZE;
	UINT BufferSize = DefBufferSize;
	ListBox->Items->Add("Checking for updates...");
	Application->ProcessMessages();

	char UpdateURL[MAX_PATH];
	ZeroMemory(&UpdateURL[0], MAX_PATH);
	sprintf(&UpdateURL[0], "%s%s", BaseURL, UpdateFileName);

	CRawBuffer UpdateBuffer;
	if(FileDownloader.DownloadFile(UpdateURL, &UpdateBuffer, &BufferSize)){
		if(UpdateBuffer.GetSize() >= DefBufferSize){

			DWORD Sig[SIGNATURE_SIZE/4] = {0x6E8BB3CC, 0x09EF018F, 0x644C3091, 0x1E68478B};

			RPCHeader *pHeader = (RPCHeader*)UpdateBuffer.GetBuffer();

			if(memcmp(&Sig[0], &pHeader->Signature[0], SIGNATURE_SIZE) != 0){
				ListBox->Items->Add("Invalid file signature.");
				return;
			}

			UpdatesVerNum.Scan((char*)&pHeader->Version[0]);
			char szCurrentVer[256];
			ZeroMemory(&szCurrentVer[0], 256);
			sprintf(&szCurrentVer[0], "Patch Version: v%d.%d.%d", UpdatesVerNum.GetHi(), UpdatesVerNum.GetMd(), UpdatesVerNum.GetLo());
			ListBox->Items->Add(&szCurrentVer[0]);

			int res = UpdatesVerNum.Compare(&CurrentVerNum);
			if(res == 0){
				ListBox->Items->Add("Version up-to-date.");
			} else if(res > 0){
				ListBox->Items->Add("A new version is disponible.");
				AddListboxMessageArg(ListBox, "Patch Size: %d bytes.", pHeader->PatchSize);
				AddListboxMessageArg(ListBox, "Pieces Size: %d bytes.", pHeader->PiecesSize);
				AddListboxMessageArg(ListBox, "Number of Pieces: %d.", CalcNumPieces(pHeader->PatchSize, pHeader->PiecesSize));
				if(pHeader->PatchSize > 0)
					ListBox->Items->Add("Press \"Download Updates\" when ready.");
			}

			ButtonDownloadUpdates->Enabled = (res > 0 && pHeader->PatchSize > 0);
			ButtonCheckForUpdate->Enabled = res == 0;
		}
	} else {
		ListBox->Items->Add("Unable to find Update.rpc");
	}
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
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDownloadUpdatesClick(TObject *Sender)
{
	ListBox->Clear();
	AddListboxMessageArg(ListBox, "Downloading updates, please wait...");

	char UpdateURL[MAX_PATH];
	ZeroMemory(&UpdateURL[0], MAX_PATH);
	sprintf(&UpdateURL[0], "%s%s", BaseURL, UpdateFileName);

	CRawBuffer UpdateBuffer;
	AddListboxMessageArg(ListBox, "Downloading patch header...");
	if(FileDownloader.DownloadFile(UpdateURL, &UpdateBuffer)){
		AddListboxMessageArg(ListBox, "Reading patch header...");

		RPCHeader *pHeader = (RPCHeader*)UpdateBuffer.GetBuffer();

		CRawBuffer PatchBuffer;
		if(pHeader && UpdateBuffer.GetSize() == sizeof(RPCHeader) + pHeader->PatchNameSize){

			// Calculate the number of pieces
			UINT NumPieces = CalcNumPieces(pHeader->PatchSize, pHeader->PiecesSize);

			AddListboxMessageArg(ListBox, "Patch Size: %d bytes.", pHeader->PatchSize);
			AddListboxMessageArg(ListBox, "Pieces Size: %d bytes.", pHeader->PiecesSize);
			AddListboxMessageArg(ListBox, "Number of Pieces: %d.", NumPieces);

			if(pHeader->PatchSize == 0){
				AddListboxMessageArg(ListBox, "Nothing to patch.", pHeader->PatchSize);
				return;
			}

			// Allocate the buffer
			PatchBuffer.Allocate(pHeader->PatchSize);

			// Buffers indexes
			BYTE *pIn  = NULL;
			BYTE *pOut = NULL;

			int j = 0;
			for(UINT i = 0; i < NumPieces; i++){

				char PatchName[MAX_PATH];
				char UpdatePieceURL[MAX_PATH];
				ZeroMemory(&UpdatePieceURL[0], MAX_PATH);
				ZeroMemory(&PatchName[0], MAX_PATH);

				memcpy(&PatchName[0], UpdateBuffer.GetBuffer(sizeof(RPCHeader)), pHeader->PatchNameSize);
				FormatULR(&PatchName[0], MAX_PATH);

				sprintf(&UpdatePieceURL[0], "%s%s.%3.3d", BaseURL, PatchName, i);

				AddListboxMessageArg(ListBox, "Downloading part #%d of %d, please wait...", i+1, NumPieces);

				UINT PieceSize = CalcPieceSize(pHeader->PatchSize, pHeader->PiecesSize, i);

				pOut = PatchBuffer.GetBuffer(j);

				CRawBuffer Buf;
				if(FileDownloader.DownloadFile(UpdatePieceURL, &Buf)){
					if(PieceSize == Buf.GetSize()){
						memcpy(pOut, Buf.GetBuffer(), Buf.GetSize());
					} else {
						goto ErrorJmp;
					}
				} else {
					ErrorJmp:
					AddListboxMessageArg(ListBox, "Failed Downloading part #%d...", i+1);
					AddListboxMessageArg(ListBox, "Aborting.");
					return;
				}

				j += PieceSize;
			}
		}

		PatchBuffer.Decrypt(0xDEADC0DE);
		PatchBuffer.SaveToFile("C:\\Temp\\PatchContent.raw");

		DWORD BufHash = PatchBuffer.Hash();

		if(BufHash != pHeader->PatchHash){
			ListBox->Items->Add("Patch corrupted: Invalid hash.");
			return;
		} else {
			ListBox->Items->Add("Patch hash verified. Data is valid.");
		}

		DoUpdates(&PatchBuffer);
	} else {
		ListBox->Items->Add("Unable to find Update.rpc");
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDLPatchNotesClick(TObject *Sender)
{
	char PatchNotesURL[MAX_PATH];
	ZeroMemory(&PatchNotesURL[0], MAX_PATH);
	sprintf(&PatchNotesURL[0], "%s%s", BaseURL, PatchNotesFileName);

	CRawBuffer PatchNotesBuffer;
	if(FileDownloader.DownloadFile(PatchNotesURL, &PatchNotesBuffer)){
		PatchNotesBuffer.SaveToFile(PatchNotesFileName);
		TStringList *pLst = new TStringList;
		pLst->LoadFromFile(PatchNotesFileName);
		PatchNotesForm->ListBox->Items->Clear();
		PatchNotesForm->ListBox->Items->AddStrings(pLst);
		delete pLst;
		PatchNotesForm->Show();
	} else {
		ListBox->Items->Add("Unable to find Patchnotes.txt");
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonCancelClick(TObject *Sender)
{
	ListBox->Items->Add("Canceling download...");
	FileDownloader.Cancel();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::DoUpdates(CRawBuffer* pPatchBuffer)
{
	if(pPatchBuffer->GetSize() != 0){

		pPatchBuffer->SaveToFile("Patch.exe");

		ListBox->Items->Add("Patching...");
		ListBox->Update();
		Sleep(1000);

		// Lauch the exe and close the program
		CFileManager FileManager;
		FileManager.Open("Patch.exe", NULL);

		Sleep(1000);
		// This will auto-delete the file "Patch.exe" when it close
		FileManager.Open("DelPatch.exe", NULL);
	}

	ListBox->Items->Add("Closing...");
	Sleep(500);
	PostQuitMessage(0);
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonTestClick(TObject *Sender)
{
	char FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);

	LPCSTR SourceFolder = "C:\\Temp\\patch 1.1";
	LPCSTR UpdateFileName = "Update";

	sprintf(FileName, "%s\\%s.rpc", SourceFolder, UpdateFileName);

	CFileManager FileManager;
	int HeaderFileSize = FileManager.GetSize("C:\\Temp\\Update.rpc");

	CRawBuffer UpdateBuffer;
	UpdateBuffer.Allocate(HeaderFileSize);
	RPCHeader* pHeader = (RPCHeader*)UpdateBuffer.GetBuffer();

	CFileIO f;
	if(f.OpenForReading(FileName)){
		f.Read(UpdateBuffer.GetBuffer(), UpdateBuffer.GetSize());
		f.Close();
	}

	//UINT PatchFileSizeFromFile   = FileManager.GetSize("C:\\Temp\\PatchContent.exe");
	//UINT PatchFileSizeFromHeader = pHeader->PatchSize;

	UINT PatchFileSize = pHeader->PatchSize;
	/*if(PatchFileSizeFromFile != PatchFileSizeFromHeader)
		return;*/

	CRawBuffer PatchBuffer(PatchFileSize);

	int NumPieces = CalcNumPieces(pHeader->PatchSize, pHeader->PiecesSize);

	int j = 0;
	for(int i = 0; i < NumPieces; i++){

		ZeroMemory(FileName, MAX_PATH);

		char PatchName[MAX_PATH];
		ZeroMemory(PatchName, MAX_PATH);
		memcpy(&PatchName[0], UpdateBuffer.GetBuffer(sizeof(RPCHeader)), pHeader->PatchNameSize);

		sprintf(FileName, "%s\\%s.%3.3d", SourceFolder, PatchName, i);

		if(f.OpenForReading(FileName)){
			UINT NumBytesToRead = CalcPieceSize(pHeader->PatchSize, pHeader->PiecesSize, i);

			UINT res = f.Read(PatchBuffer.GetBuffer(j), NumBytesToRead);
			f.Close();

			if(res != NumBytesToRead)
				return;

			j += res;
		} else {
			return;
		}
	}

	PatchBuffer.Decrypt(0xDEADC0DE);
	UINT PatchHash = PatchBuffer.Hash();

	if(PatchHash == pHeader->PatchHash){
		DoUpdates(&PatchBuffer);
	} else {
		ListBox->Items->Add("Patch corrupted: Invalid hash.");
	}
}

