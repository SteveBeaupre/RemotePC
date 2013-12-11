//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
CRemotePCClient *pRemotePCClient = NULL;
CKbHookDllStub KbHookDllStub;
void __cdecl OnKeyEvent(DWORD wParam, DWORD lParam);
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	#ifdef _DEBUG
	Position = poDefault;
	Left = 520;
	Top  = 20;
	Width = 820;
	#else
	Position = poDesktopCenter;
	#endif

	char AppCaption[256];
	SetCaption("RemotePC Client 2014", AppCaption, 256);
	Caption = AnsiString(AppCaption);

	if(!KbHookDllStub.Load("KBHook.dll")){
		Application->Terminate();
	} else {
		KbHookDllStub.InstallHook(DesktopViewer->Handle, OnKeyEvent);
	}

	LabelDLSpeed->Caption = AnsiString("D: 0 Byte");
	LabelULSpeed->Caption = AnsiString("U: 0 Byte");
	LabelTotalDownload->Caption = AnsiString("Av: 0.00 Kbp\\s");
	LabelTotalUpload->Caption   = AnsiString("Av: 0.00 Kbp\\s");

	LangID = REMOTEPC_LANG_ENGLISH;
	LogedIn = false;

	EnableUI();
	InitializeWinSock();

	pRemotePCClient = new CRemotePCClient();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	KbHookDllStub.Free();

	pRemotePCClient->Disconnect();

	SAFE_DELETE_OBJECT(pRemotePCClient);
	ShutdownWinSock();

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBoxConnectAsServerClick(TObject *Sender)
{
	EnableUI();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EnableUI()
{
	switch(CheckBoxConnectAsServer->Checked){
	case false: ButtonConnect->Caption = "Connect";   break;
	case true:  ButtonConnect->Caption = "Listen..."; break;
	}
	ButtonConnect->Enabled = true;
	ButtonDisconnect->Enabled = false;
	ButtonPause->Enabled = false;
	ButtonPause->Caption = "Pause";
	CheckBoxConnectAsServer->Enabled = true;
	ComboBoxHostName->Enabled = !CheckBoxConnectAsServer->Checked;
	EditPort->Enabled = true;
	EditPassword->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DisableUI()
{
	ButtonDisconnect->Enabled = true;
	ButtonConnect->Enabled = false;
	CheckBoxConnectAsServer->Enabled = false;
	ComboBoxHostName->Enabled = false;
	EditPort->Enabled = false;
	EditPassword->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WndProc(Messages::TMessage &Message)
{
	switch(Message.Msg)
	{
	case WM_SOCKET:
		if(pRemotePCClient)
			pRemotePCClient->ProcessWinsockMessages(Message.LParam);
		break;
	case ON_THREAD_START:
		if(!CheckBoxConnectAsServer->Checked){
			AddListboxMessageArg(ListBox, "Connecting");
		} else {
			AddListboxMessageArg(ListBox, "Listening...");
		}
		break;
	case ON_CONNECTED:
		AddListboxMessageArg(ListBox, "Connection established!");
		if(pRemotePCClient){
			char Password[32];
			ConvertUnicodeToChar(Password, 32, EditPassword->Text.c_str());
			if(pRemotePCClient){
				pRemotePCClient->Reset();
				pRemotePCClient->SetRendererWnd(DesktopViewer->Handle);
				pRemotePCClient->SendLoginRequest("", Password);
				pRemotePCClient->StartThread();
			}
			NetworkSpeedTimer->Enabled = true;
			ButtonPause->Enabled = true;
		}
		break;
	case ON_DISCONNECTED:
		LogedIn = false;
		NetworkSpeedTimer->Enabled = false;
		if(pRemotePCClient)
			pRemotePCClient->StopThread();
		AddListboxMessageArg(ListBox, "Disconnected");
		EnableUI();
		break;
	case ON_CONNECTION_LOST:
		LogedIn = false;
		NetworkSpeedTimer->Enabled = false;
		AddListboxMessageArg(ListBox, "Connection closed by peer.");
		EnableUI();
		break;
	case ON_CONNECTION_CANCELED:
		AddListboxMessageArg(ListBox, "Connection Canceled...");
		EnableUI();
		break;
	case ON_CONNECTION_TIMED_OUT:
		{
			int NumAttemp   = Message.WParam;
			int TotalAttemp = Message.LParam;

			if(NumAttemp < TotalAttemp){
				AddListboxMessageArg(ListBox, "Attemp #%d Failed... Retrying...", NumAttemp);
			} else {
				AddListboxMessageArg(ListBox, "Attemp #%d Failed... Aborting...", NumAttemp);
				AddListboxMessageArg(ListBox, "Unable to connect to server.");
				EnableUI();
			}
		}
		break;

	case ON_LOGIN:
		switch(Message.WParam)
		{
		case TRUE:
			AddListboxMessageArg(ListBox, "Login Sucessful.");
			LogedIn = true;
			break;
		case FALSE:
			AddListboxMessageArg(ListBox, "Login Failed.");
			break;
		}

		if(pRemotePCClient && Message.WParam != FALSE)
			pRemotePCClient->SendScreenshotRequest();

		break;

	case ON_UPDATE_SCROLLBARS_MSG:
		ScrollBox->VertScrollBar->Range = Message.LParam;
		ScrollBox->HorzScrollBar->Range = Message.WParam;
		break;
	}

	TForm::WndProc(Message); // Default processing for any other message
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonConnectClick(TObject *Sender)
{
	const int BufSize = 8;
	char sPort[BufSize];
	ConvertUnicodeToChar(sPort, BufSize, EditPort->Text.c_str());

	WORD Port = 9966;
	if(strlen(sPort) > 0)
		Port = atoi(sPort);

	if(!CheckBoxConnectAsServer->Checked){

		const int BufSize = 16;
		char ip[BufSize];
		ConvertUnicodeToChar(ip, BufSize, ComboBoxHostName->Text.c_str());

		pRemotePCClient->ConnectAsClient(Handle, ip, Port);
	} else {
		pRemotePCClient->ConnectAsServer(Handle, Port);
	}

	DisableUI();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDisconnectClick(TObject *Sender)
{
	pRemotePCClient->Disconnect();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	if(CheckBoxFullscreen->Checked){
		SettingsPanel->Visible = X < 3;
	}

	if(LogedIn && !pRemotePCClient->GetThread()->IsThreadPaused()){
		CMouseInputMsgStruct mm;
		mm.Msg  = MSG_MOUSE_MOVE;
		mm.Data = pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, 0,0, true);

		pRemotePCClient->SendMouseMsg(&mm);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DesktopViewerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!LogedIn  || pRemotePCClient->GetThread()->IsThreadPaused())
		return;

	CMouseInputMsgStruct mm;
	mm.Msg  = pRemotePCClient->GetClientInputs()->EncodeMouseButton(Button, false);
	mm.Data = pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, 0,0, true);

	pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!LogedIn  || pRemotePCClient->GetThread()->IsThreadPaused())
		return;

	CMouseInputMsgStruct mm;
	mm.Msg  = pRemotePCClient->GetClientInputs()->EncodeMouseButton(Button, true);
	mm.Data = pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, 0,0, CheckBoxStretch->Checked);

	pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseRoll(TObject *Sender, short WheelDelta)
{
	if(!LogedIn  || pRemotePCClient->GetThread()->IsThreadPaused())
		return;

	CMouseInputMsgStruct mm;
	mm.Msg  = MSG_MOUSE_ROLL;
	mm.Data = WheelDelta;

	pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __cdecl OnKeyEvent(DWORD wParam, DWORD lParam)
{
	if(!MainForm->LogedIn || pRemotePCClient->GetThread()->IsThreadPaused())
		return;

	CKeyboardInputMsgStruct km;
	switch(wParam)
	{
	case WM_KEYDOWN:    km.Msg = MSG_KEY_DOWN;     break;
	case WM_KEYUP:      km.Msg = MSG_KEY_UP;       break;
	case WM_SYSKEYDOWN: km.Msg = MSG_SYS_KEY_DOWN; break;
	case WM_SYSKEYUP:   km.Msg = MSG_SYS_KEY_UP;   break;
	default: return;    // should never happen...
	}

	KBDLLHOOKSTRUCT *pData = (KBDLLHOOKSTRUCT*)lParam;
	memcpy(&km.Data, pData, sizeof(KBDLLHOOKSTRUCT));

	pRemotePCClient->SendKeyboardMsg(&km);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBoxStretchClick(TObject *Sender)
{
	pRemotePCClient->GetOpenGL()->SetStretchedFlag(CheckBoxStretch->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBoxShowFPSClick(TObject *Sender)
{
	pRemotePCClient->GetOpenGL()->SetShowFPSFlag(CheckBoxShowFPS->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBoxFullscreenClick(TObject *Sender)
{
	switch(CheckBoxFullscreen->Checked)
	{
	case FALSE: SwitchToWindowedMode();   break;
	case TRUE:  SwitchToFullscreenMode(); break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SwitchToFullscreenMode()
{
	//if(KbHookDll.IsLoaded())
	//	KbHookDll.RemoveHook();

	int ScrW = Screen->Width;
	int ScrH = Screen->Height;

	BorderStyle = bsNone;
	SetWindowLongPtr(Handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
	MoveWindow(Handle, 0, 0, ScrW, ScrH, TRUE);

	SettingsPanel->Hide();
	ConnectionPanel->Hide();

	//if(KbHookDll.IsLoaded())
	//	KbHookDll.InstallHook(DesktopViewer->Handle, OnKeyEvent);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SwitchToWindowedMode()
{
	SetWindowLongPtr(Handle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	BorderStyle = bsSingle;
	MoveWindow(Handle, 520, 20, 820, 538, TRUE);

	ConnectionPanel->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NetworkSpeedTimerTimer(TObject *Sender)
{
	CNetStats* pStats = pRemotePCClient->GetNetManager()->GetStats();
	pStats->UpdateStats();

	char TotDownloaded[256];
	char AvgDownloaded[256];
	char TotUploaded[256];
	char AvgUploaded[256];

	ZeroMemory(TotDownloaded, 256);
	ZeroMemory(TotUploaded,   256);
	ZeroMemory(AvgDownloaded, 256);
	ZeroMemory(AvgUploaded,   256);

	sprintf(TotDownloaded, "D: %s",  pStats->GetTotalDownloaded());
	sprintf(AvgDownloaded, "Av: %s", pStats->GetAverageDownloadKBPS());
	sprintf(TotUploaded, "U: %s",    pStats->GetTotalUploaded());
	sprintf(AvgUploaded, "Av: %s",   pStats->GetAverageUploadKBPS());

	LabelDLSpeed->Caption = AnsiString(TotDownloaded);
	LabelULSpeed->Caption = AnsiString(TotUploaded);
	LabelTotalDownload->Caption = AnsiString(AvgDownloaded);
	LabelTotalUpload->Caption   = AnsiString(AvgUploaded);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonPauseClick(TObject *Sender)
{
	if(pRemotePCClient->GetThread()->IsThreadRunning()){
		if(!pRemotePCClient->GetThread()->IsThreadPaused()){
			pRemotePCClient->GetThread()->PauseThread();
			ButtonPause->Caption = "Resume";
		} else {
			pRemotePCClient->GetThread()->ResumeThread();
			ButtonPause->Caption = "Pause";
		}
	}
}
//---------------------------------------------------------------------------

