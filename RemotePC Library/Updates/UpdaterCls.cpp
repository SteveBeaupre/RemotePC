#include "UpdaterCls.h"

const int MaxURLSize = 8*1024;
const int MaxPatchUpdateTextSize = (8*1024*1024);

LPCSTR BaseURL = "http://vortex666.webs.com/RemotePC/";

LPSTR UpdateFileName     = "Update.rpc";
LPSTR PatchNotesFileName = "PatchNotes.txt";

HKEY   RootKey = HKEY_LOCAL_MACHINE;
LPCSTR RegKey  = "SOFTWARE\\RemotePC 2014";
LPCSTR RegVer  = "Version";

CUpdater::CUpdater()
{
	MinimumVerNum.Set(1, 0, 0);
	UpdatesVerNum.Set(0, 0, 0);

	CurrentVerNum.Scan(GetCurrentVersion());

	if(!GetCurrentVersion()){
		// version not found, so write "1.0.0"...
		CRawBuffer Buf(VERSION_SIZE);
		sprintf(Buf.GetBuffer(), "1.0.0");

		CRegistry Registry;
		Registry.SaveString(RootKey, RegKey, RegVer, Buf.GetBuffer(), Buf.GetSize());
		CurrentVerNum.Set(1,0,0);
	} else {
		UpdateCurrentVersion();
	}
}

bool CUpdater::IsValidURLChar(char c)
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

void CUpdater::FormatULR(char *pURL, UINT bufsize)
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

void CUpdater::UpdateULRBuffer(CRawBuffer *pRawBuffer)
{
	pRawBuffer->Allocate(MaxURLSize);
	sprintf(pRawBuffer->GetStrBuffer(), "%s%s", BaseURL, UpdateFileName);
}

void CUpdater::UpdatePatchNoteBuffer(CRawBuffer *pRawBuffer)
{
	pRawBuffer->Allocate(MaxPatchUpdateTextSize);
	sprintf(pRawBuffer->GetStrBuffer(), "%s%s", BaseURL, PatchNotesFileName);
}

void CUpdater::UpdateCurrentVersion()
{
	CRawBuffer CurrentVer(256);
	sprintf(CurrentVer.GetStrBuffer(), "Current Version: %d.%d.%d", CurrentVerNum.GetHi(), CurrentVerNum.GetMd(), CurrentVerNum.GetLo());
	OnOutputMsg(CurrentVer.GetStrBuffer());
};

bool CUpdater::DownloadUpdates()
{
	OnClearMsgs();
	OnOutputMsg("Downloading updates, please wait...");

	CRawBuffer UpdateURL(MaxURLSize);
	UpdateULRBuffer(&UpdateURL);

	CRawBuffer UpdateBuffer;
	OnOutputMsg("Downloading patch header...");
	if(FileDownloader.DownloadFile(UpdateURL.GetStrBuffer(), &UpdateBuffer)){
		OnOutputMsg("Reading patch header...");

		RPCHeader *pHeader = (RPCHeader*)UpdateBuffer.GetBuffer();

		if(pHeader && UpdateBuffer.GetSize() == sizeof(RPCHeader) + pHeader->PatchNameSize){

			// Calculate the number of pieces
			UINT NumPieces = CalcNumPieces(pHeader->PatchSize, pHeader->PiecesSize);

			OnOutputMsg("Patch Size: %d bytes.", pHeader->PatchSize);
			OnOutputMsg("Pieces Size: %d bytes.", pHeader->PiecesSize);
			OnOutputMsg("Number of Pieces: %d.", NumPieces);

			if(pHeader->PatchSize == 0){
				OnOutputMsg("Nothing to patch.", pHeader->PatchSize);
				return false;
			}

			// Allocate the buffer
			PatchBuffer.Allocate(pHeader->PatchSize);

			BYTE *pOut = NULL;

			CRawBuffer PatchName(MAX_PATH);
			CRawBuffer PieceURL(MaxURLSize);

			int j = 0;
			for(UINT i = 0; i < NumPieces; i++){

				PatchName.Erase();
				PieceURL.Erase();

				memcpy(PatchName.GetBuffer(), UpdateBuffer.GetBuffer(sizeof(RPCHeader)), pHeader->PatchNameSize);
				FormatULR(PatchName.GetBuffer(), PatchName.GetSize());

				snprintf(PieceURL.GetStrBuffer(), PieceURL.GetSize(), "%s%s.%3.3d", BaseURL, PatchName.GetStrBuffer(), i);

				OnOutputMsg("Downloading part #%d of %d, please wait...", i+1, NumPieces);

				UINT PieceSize = CalcPieceSize(pHeader->PatchSize, pHeader->PiecesSize, i);

				pOut = PatchBuffer.GetBuffer(j);

				CRawBuffer Buf;
				if(FileDownloader.DownloadFile(PieceURL.GetStrBuffer(), &Buf)){
					if(PieceSize == Buf.GetSize()){
						memcpy(pOut, Buf.GetBuffer(), Buf.GetSize());
					} else {
						goto ErrorJmp;
					}
				} else {
					ErrorJmp:
					OnOutputMsg("Failed Downloading part #%d...", i+1);
					OnOutputMsg("Aborting.");
					return false;
				}

				j += PieceSize;
			}
		}

		PatchBuffer.Decrypt(0xDEADC0DE);
		//PatchBuffer.SaveToFile("C:\\Temp\\PatchContent.raw");

		DWORD BufHash = PatchBuffer.Hash();

		if(BufHash != pHeader->PatchHash){
			OnOutputMsg("Patch corrupted: Invalid hash.");
			return false;
		} else {
			OnOutputMsg("Patch hash verified. Data is valid.");
		}

		DoUpdates();
	} else {
		OnOutputMsg("Unable to download patch file...");
		return false;
	}

	return true;
}

bool CUpdater::DownloadPatchNotes()
{
	CRawBuffer PatchNotesURL(MaxPatchUpdateTextSize);
	UpdatePatchNoteBuffer(&PatchNotesURL);

	CRawBuffer PatchNotesBuffer;
	if(FileDownloader.DownloadFile(PatchNotesURL.GetStrBuffer(), &PatchNotesBuffer)){
		PatchNotesBuffer.SaveToFile(PatchNotesFileName);

		CTxtFileIO f;

		OnClearPatchNotesMsg();
		if(f.OpenForReading(PatchNotesFileName)){

			char *line = NULL;
			CRawBuffer Buf(MaxURLSize);
			while(1){
				Buf.Erase();
				line = f.ReadLine(Buf.GetBuffer(), MaxURLSize);
				if(!line)
					break;

				int sLen = strlen(Buf.GetStrBuffer());
				if(sLen > 0){
					char *szBuf = Buf.GetStrBuffer(sLen-1);
					char c = *szBuf;
					if(c == '\n')
						*szBuf = NULL;
				}

				OnOutputPatchNotesMsg(Buf.GetStrBuffer());
			}
		}

		OnClearMsgs();
		OnShowPatchForm();

		return true;
	} else {
		OnOutputMsg("Unable to find Patchnotes.txt");
	}

	return false;
}

char* CUpdater::GetCurrentVersion()
{
	static const char* vn = VERNUM;
	return (char*)&vn[0];

}

void CUpdater::CancelDownload()
{
	FileDownloader.Cancel();
	OnDownloadCanceled();
}

bool CUpdater::CheckForUpdates(char *pURL)
{
	OnClearMsgs();

	const UINT DefBufferSize = SIGNATURE_SIZE + VERSION_SIZE;
	UINT BufferSize = DefBufferSize;
	OnOutputMsg("Checking for updates...");
	OnProcessMessages();

	CRawBuffer UpdateURL(MaxURLSize);
	UpdateULRBuffer(&UpdateURL);

	CRawBuffer UpdateBuffer;
	if(FileDownloader.DownloadFile(UpdateURL.GetStrBuffer(), &UpdateBuffer, &BufferSize)){
		if(UpdateBuffer.GetSize() >= DefBufferSize){

			DWORD Sig[SIGNATURE_SIZE/4] = {0x6E8BB3CC, 0x09EF018F, 0x644C3091, 0x1E68478B};

			RPCHeader *pHeader = (RPCHeader*)UpdateBuffer.GetBuffer();

			if(memcmp(&Sig[0], &pHeader->Signature[0], SIGNATURE_SIZE) != 0){
				OnOutputMsg("Invalid file signature.");
				return false;
			}

			UpdatesVerNum.Scan((char*)&pHeader->Version[0]);
			UpdateCurrentVersion();

			int res = UpdatesVerNum.Compare(&CurrentVerNum);
			if(res <= 0){
				OnOutputMsg("Version up-to-date.");
			} else if(res > 0){
				OnOutputMsg("A new version is disponible.");
				OnOutputMsg("Patch Size: %d bytes.", pHeader->PatchSize);
				OnOutputMsg("Pieces Size: %d bytes.", pHeader->PiecesSize);
				OnOutputMsg("Number of Pieces: %d.", CalcNumPieces(pHeader->PatchSize, pHeader->PiecesSize));
				if(pHeader->PatchSize > 0)
					OnOutputMsg("Press \"Download Updates\" when ready.");
			}

			OnUpdatesReady(res, pHeader->PatchSize);
		}
	} else {
		OnOutputMsg("An error occured while downloading the patch.");
		return false;
	}

	return true;
}

bool CUpdater::TestUpdateFile(char *fname)
{
	CFileManager FileManager;
	UINT HeaderFileSize = FileManager.GetSize(fname);

	CRawBuffer UpdateBuffer;
	UpdateBuffer.Allocate(HeaderFileSize);
	RPCHeader* pHeader = (RPCHeader*)UpdateBuffer.GetBuffer();

	CFileIO f;
	if(f.OpenForReading(fname)){
		f.Read(UpdateBuffer.GetBuffer(), UpdateBuffer.GetSize());
		f.Close();
	}

	if(HeaderFileSize != pHeader->PatchSize)
		return false;

	UINT PatchFileSize = pHeader->PatchSize;
	CRawBuffer PatchBuffer(PatchFileSize);

	int NumPieces = CalcNumPieces(pHeader->PatchSize, pHeader->PiecesSize);

	int j = 0;
	for(int i = 0; i < NumPieces; i++){

		CRawBuffer FileName(MAX_PATH);
		FileName.Erase();

		CRawBuffer PatchName(MAX_PATH);
		memcpy(PatchName.GetStrBuffer(), UpdateBuffer.GetBuffer(sizeof(RPCHeader)), pHeader->PatchNameSize);

		sprintf(FileName.GetStrBuffer(), "%s.%3.3d", FileName.GetStrBuffer(), i);

		// Read the file in chunks
		if(f.OpenForReading(FileName.GetStrBuffer())){
			UINT NumBytesToRead = CalcPieceSize(pHeader->PatchSize, pHeader->PiecesSize, i);

			UINT res = f.Read(PatchBuffer.GetBuffer(j), NumBytesToRead);
			f.Close();

			if(res != NumBytesToRead)
				return false;

			j += res;
		} else {
			return false;
		}
	}

	PatchBuffer.Decrypt(0xDEADC0DE);
	UINT PatchHash = PatchBuffer.Hash();

	return PatchHash == pHeader->PatchHash;
}

void CUpdater::DoUpdates()
{
	if(PatchBuffer.GetSize() != 0){

		// Save the patch to file
		PatchBuffer.SaveToFile("Patch.exe");

		// Prepare to run the patch...
		OnOutputMsg("Patching...");
		OnUpdateMsgs();
		Sleep(1000);

		// Lauch the exe and close the program
		CFileManager FileManager;
		FileManager.Open("Patch.exe", NULL);
		Sleep(1000);

		// <-- This will auto-delete the file "Patch.exe" (non-blocking)
		FileManager.Open("DelPatch.exe", NULL);
	}

	// Close this program
	OnOutputMsg("Closing...");
	Sleep(500);

	PostQuitMessage(0);
}




