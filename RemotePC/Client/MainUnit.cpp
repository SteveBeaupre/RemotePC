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
CClientSettings Settings;
//---------------------------------------------------------------------------
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
	LangID = REMOTEPC_LANG_ENGLISH;
	LogedIn = false;

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

	InitializeWinSock();
	pRemotePCClient = new CRemotePCClient();

	LoadSettings();
	EnableUI();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	KbHookDllStub.Free();

	pRemotePCClient->Disconnect();
	Sleep(250);
	Application->ProcessMessages();

	SAFE_DELETE_OBJECT(pRemotePCClient);
	ShutdownWinSock();

	SaveSettings();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadSettings()
{
	Settings.Load();
	ClientSettingsStruct *pSettings = Settings.GetSettings();

	ComboBoxHostName->Text = AnsiString(pSettings->ip);
	EditPort->Text = AnsiString(pSettings->Port);
	EditPassword->Text = AnsiString(pSettings->pw);

	CheckBoxConnectAsServer->Checked = pSettings->ConnectAsServer;
	CheckBoxStretch->Checked = pSettings->Stretch;
	CheckBoxShowFPS->Checked = pSettings->ShowFPS;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveSettings()
{
	ClientSettingsStruct ClientSettings;
	ZeroMemory(&ClientSettings, sizeof(ClientSettingsStruct));

	ConvertUnicodeToChar(ClientSettings.ip, 16, ComboBoxHostName->Text.c_str());
	ConvertUnicodeToChar(ClientSettings.pw, 32, EditPassword->Text.c_str());
	ClientSettings.Port = _wtoi(EditPort->Text.c_str());

	ClientSettings.ConnectAsServer = CheckBoxConnectAsServer->Checked;
	ClientSettings.Stretch = CheckBoxStretch->Checked;
	ClientSettings.ShowFPS = CheckBoxShowFPS->Checked;

	Settings.SetSettings(&ClientSettings);
	Settings.Save();
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
	if(CheckBoxFullscreen->Checked)
		CheckBoxFullscreen->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	if(CheckBoxFullscreen->Checked){
		bool ShowHiddenStuffs = X < 2;
		LeftPanel->Visible = ShowHiddenStuffs;
		ConnectionPanel->Visible = ShowHiddenStuffs;
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
	int ScrW = Screen->Width;
	int ScrH = Screen->Height;

	BorderStyle = bsNone;
	SetWindowLongPtr(Handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
	MoveWindow(Handle, 0, 0, ScrW, ScrH, TRUE);

	LeftPanel->Hide();
	ConnectionPanel->Hide();
	ViewerPanel->BevelOuter = bvNone;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SwitchToWindowedMode()
{
	SetWindowLongPtr(Handle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	BorderStyle = bsSingle;
	MoveWindow(Handle, 520, 20, 820, 538, TRUE);

	LeftPanel->Show();
	ConnectionPanel->Show();
	ViewerPanel->BevelOuter = bvRaised;
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
void __fastcall TMainForm::SetLanguage(int LanguageID)
{
	if(LanguageID >= 0 && LanguageID <= 1){
		LangID = LanguageID;
	} else {
		return;
	}

	EnglishMenu->Caption = szEnglishMenuCaption[LangID];
	FrenchMenu->Caption = szFrenchMenuCaption[LangID];
	ButtonDisconnect->Caption = szButtonDisconnectCaption[LangID];
	CheckBoxConnectAsServer->Caption = szCheckBoxConnectAsServerCaption[LangID];
	LabelPassword->Caption = szLabelPasswordCaption[LangID];
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::EnglishMenuClick(TObject *Sender)
{
	SetLanguage(REMOTEPC_LANG_ENGLISH);
	EnglishMenu->Checked = true;
	FrenchMenu->Checked  = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FrenchMenuClick(TObject *Sender)
{
	SetLanguage(REMOTEPC_LANG_FRENCH);
	EnglishMenu->Checked = false;
	FrenchMenu->Checked  = true;
}
//---------------------------------------------------------------------------

