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
#ifdef EMULATE_OPENGL
TColor PanelBackColor = clBlack;
#endif
//---------------------------------------------------------------------------
CKbHookDllStub KbHookDllStub;
void __cdecl OnKeyEvent(DWORD wParam, DWORD lParam);
//---------------------------------------------------------------------------
WndCoordsStruct WndCoords;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::IsLoopbackAddress(AnsiString s)
{
	return s == "127.0.0.1" || s == "192.168.0.1";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	if(!OneInstance.Check("REMOTE_PC_CLIENT_2014")){
		Application->Terminate();
		return;
	}

	LogedIn = false;

	#ifdef EMULATE_OPENGL
	DWORD dwColor = 0x00262626;
	memcpy(&PanelBackColor, &dwColor, sizeof(DWORD));
	#endif

	#ifdef _DEBUG
	Position = poDefault;
	Left = 520;
	Top  = 20;
	Width = 820;
	#endif

	Application->ShowMainForm = true;
	Show();

	char AppCaption[256];
	SetCaption("RemotePC Client 2014", AppCaption, 256);
	Caption = AnsiString(AppCaption);

	if(!KbHookDllStub.Load("KBHook.dll"))
		Application->Terminate();

	StatusBar->Panels->Items[0]->Text = "Disconnected.";
	StatusBar->Panels->Items[1]->Text = "D: 0 Byte  T: 0.00 Kbp\\s";
	StatusBar->Panels->Items[2]->Text = "U: 0 Byte  T: 0.00 Kbp\\s";

	//FilesManagerTreeView->Align = alTop;
	//FilesManagerTreeView->Height = 150;
	PanelFilesManager->Parent = TabControl;
	PanelFilesManager->Align = alClient;
	TabControl->Tabs->Delete(1);
	TabControl->OnChange(this);

	InitializeWinSock();
	pRemotePCClient = new CRemotePCClient();

	LoadSettings();
	EnableUI();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{

	pRemotePCClient->Disconnect();
	Sleep(250);
	Application->ProcessMessages();

	KbHookDllStub.Free();

	SAFE_DELETE_OBJECT(pRemotePCClient);
	ShutdownWinSock();

	SaveSettings();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadSettings()
{
	Settings.Load();
	ClientSettingsStruct *pSettings = Settings.GetSettings();

	LangID = pSettings->LangID;
	EnglishMenu->Checked = LangID == REMOTEPC_LANG_ENGLISH;
	FrenchMenu->Checked  = LangID == REMOTEPC_LANG_FRENCH;
	SetLanguage(LangID);

	ComboBoxHostName->Items->Clear();
	ComboBoxHostName->Items->Add("127.0.0.1");
	ComboBoxHostName->Items->Add("192.168.0.1");
	if(IsLoopbackAddress(AnsiString(pSettings->ip))){
		if(AnsiString(pSettings->ip) == "127.0.0.1")
			ComboBoxHostName->ItemIndex = 0;
		else
			ComboBoxHostName->ItemIndex = 1;
	} else {
		ComboBoxHostName->Items->Add(AnsiString(pSettings->ip));
		ComboBoxHostName->ItemIndex = ComboBoxHostName->Items->Count-1;
	}
	EditPassword->Text = AnsiString(pSettings->pw);
	EditPort->Text = AnsiString(pSettings->Port);

	CheckBoxConnectAsServer->Checked = pSettings->ConnectAsServer;
	CheckBoxShowFPS->Checked = pSettings->ShowFPS;
	CheckBoxStretch->Checked = pSettings->Stretch;
	pRemotePCClient->GetOpenGL()->SetStretchedFlag(pSettings->Stretch);
	pRemotePCClient->GetOpenGL()->SetShowFPSFlag(pSettings->ShowFPS);

	ComboBoxScrFormat->ItemIndex = pSettings->ColorDepth;

	this->Position = poDesktopCenter;
	this->WindowState = (TWindowState)pSettings->WndCoords.ws;

	/*if(pSettings->WndCoords.l == 0 || pSettings->WndCoords.t == 0 || pSettings->WndCoords.w == 0 || pSettings->WndCoords.h == 0){
		#ifndef _DEBUG
		this->Position = poDesktopCenter;
		#endif
		return;
	}

	this->Left   = pSettings->WndCoords.l;
	this->Top    = pSettings->WndCoords.t;
	this->Width  = pSettings->WndCoords.w;
	this->Height = pSettings->WndCoords.h;
	this->WindowState = (TWindowState)pSettings->WndCoords.ws;*/
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveSettings()
{
	ClientSettingsStruct ClientSettings;
	ZeroMemory(&ClientSettings, sizeof(ClientSettingsStruct));

	ClientSettings.LangID = LangID;

	ConvertUnicodeToChar(ClientSettings.ip, 16, ComboBoxHostName->Text.c_str());
	ConvertUnicodeToChar(ClientSettings.pw, 32, EditPassword->Text.c_str());
	ClientSettings.Port = _wtoi(EditPort->Text.c_str());

	ClientSettings.ConnectAsServer = CheckBoxConnectAsServer->Checked;
	ClientSettings.Stretch = CheckBoxStretch->Checked;
	ClientSettings.ShowFPS = CheckBoxShowFPS->Checked;

	ClientSettings.WndCoords.l = this->Left;
	ClientSettings.WndCoords.t = this->Top;
	ClientSettings.WndCoords.w = this->Width;
	ClientSettings.WndCoords.h = this->Height;
	ClientSettings.WndCoords.ws = (int)this->WindowState;

	ClientSettings.ColorDepth = ComboBoxScrFormat->ItemIndex;

	Settings.SetSettings(&ClientSettings);
	Settings.Save();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseMenuClick(TObject *Sender)
{
	Close();
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
	case false: ButtonConnect->Caption = szButtonConnectCaption[LangID]; break;
	case true:  ButtonConnect->Caption = szButtonConnectCaptionModeServer[LangID]; break;
	}

	ButtonConnect->Enabled = true;
	ConnectMenu->Enabled = true;
	ButtonDisconnect->Enabled = false;
	DisconnectMenu->Enabled = false;
	ButtonPause->Enabled = false;
	ButtonPause->Caption = szButtonPauseCaption1[LangID];
	CheckBoxConnectAsServer->Enabled = true;
	ComboBoxHostName->Enabled = !CheckBoxConnectAsServer->Checked;
	EditPort->Enabled = true;
	EditPassword->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DisableUI()
{
	ButtonDisconnect->Enabled = true;
	DisconnectMenu->Enabled = true;
	ButtonConnect->Enabled = false;
	ConnectMenu->Enabled = false;
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
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionThreadStartedAsClient[LangID]);
		} else {
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionThreadStartedAsServer[LangID]);
		}
		break;
	case ON_CONNECTED:
		AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionEstablished[LangID]);
		if(pRemotePCClient){
			KbHookDllStub.InstallHook(DesktopViewer->Handle, OnKeyEvent);

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
			#ifdef EMULATE_OPENGL
			DesktopViewer->Color = PanelBackColor;
			#endif
		}
		break;
	case ON_DISCONNECTED:
		LogedIn = false;
		NetworkSpeedTimer->Enabled = false;
		if(pRemotePCClient)
			pRemotePCClient->StopThread();
		AddListboxMessageArg(ListBoxLog, StatusBar, szOnDisconnect[LangID]);
		KbHookDllStub.RemoveHook();
		EnableUI();
		#ifdef EMULATE_OPENGL
		DesktopViewer->Color = clBlack;
		#endif
		break;
	case ON_CONNECTION_LOST:
		LogedIn = false;
		NetworkSpeedTimer->Enabled = false;
		AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionLoss[LangID]);
		KbHookDllStub.RemoveHook();
		EnableUI();
		#ifdef EMULATE_OPENGL
		DesktopViewer->Color = clBlack;
		#endif
		break;
	case ON_CONNECTION_CANCELED:
		AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionCanceled[LangID]);
		EnableUI();
		break;
	case ON_CONNECTION_TIMED_OUT:
		{
			int NumAttemp   = Message.WParam;
			int TotalAttemp = Message.LParam;

			AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionFailed[LangID], NumAttemp, TotalAttemp);
			if(NumAttemp == TotalAttemp){
				AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionTimedOut[LangID]);
				EnableUI();
			}
		}
		break;

	case ON_LOGIN:
		switch((LoginResults)Message.LParam)
		{
		case NoErrors:
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnLoginSucess[LangID]);
			LogedIn = true;
			break;

		case InvalidAuthorizationCode:
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnLoginFailedInvAuth[LangID]);
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnLoginFailedInvAuthTip[LangID]);
			break;
		case InvalidPassword:
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnLoginFailedInvPass[LangID]);
			break;
		}

		if(pRemotePCClient && Message.WParam != FALSE)
			pRemotePCClient->SendScreenshotRequest();

		break;

	case ON_UPDATE_SCROLLBARS_MSG:
		ScrollBox->VertScrollBar->Range = Message.LParam;
		ScrollBox->HorzScrollBar->Range = Message.WParam;
		break;

	/*case WM_AVAILDRIVE_CB_CLEAR:
		ComboBoxAvailableDrives->Clear();
		break;

	case WM_AVAILDRIVE_CB_ADDSTR:
		{
			UnicodeString s = UnicodeString((char*)Message.LParam);
			ComboBoxAvailableDrives->Items->Add(s);
		}
		break;*/
	}

	TForm::WndProc(Message);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonConnectClick(TObject *Sender)
{
	const int BufSize = 8;
	char sPort[BufSize];
	ConvertUnicodeToChar(sPort, BufSize, EditPort->Text.c_str());

	int Port = 9966;
	if(strlen(sPort) > 0)
		Port = atoi(sPort);

	// Port number error handling
	if(Port > 0x0000FFFF){
		ShowMessage(AnsiString(szInvalidPortRangeMsg[LangID]));
		EditPort->Text = "65535";
		return;
	} else if(Port < 0){
		ShowMessage(AnsiString(szInvalidPortRangeMsg[LangID]));
		EditPort->Text = "0";
		return;
	}

	ComboBoxScrFormat->OnChange(this);

	if(!CheckBoxConnectAsServer->Checked){

		const int BufSize = 16;
		char ip[BufSize];
		ConvertUnicodeToChar(ip, BufSize, ComboBoxHostName->Text.c_str());

		// IP error handling
		if(!IsIPValid(ip)){
			ShowMessage(AnsiString(szInvalidIPMsg[LangID]));
			return;
		}

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
		DesktopViewer->SetFocus();

		CMouseInputMsgStruct mm;
		int TexW = pRemotePCClient->GetOpenGL()->GetTexture()->Width;
		int TexH = pRemotePCClient->GetOpenGL()->GetTexture()->Height;
		mm.Msg  = MSG_MOUSE_MOVE;
		if(pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, TexW,TexH, CheckBoxStretch->Checked, &mm.Data))
			pRemotePCClient->SendMouseMsg(&mm);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DesktopViewerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!LogedIn || pRemotePCClient->GetThread()->IsThreadPaused())
		return;

	CMouseInputMsgStruct mm;
	int TexW = pRemotePCClient->GetOpenGL()->GetTexture()->Width;
	int TexH = pRemotePCClient->GetOpenGL()->GetTexture()->Height;
	mm.Msg  = pRemotePCClient->GetClientInputs()->EncodeMouseButton(Button, false);
	if(pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, TexW,TexH, CheckBoxStretch->Checked, &mm.Data))
		pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!LogedIn || pRemotePCClient->GetThread()->IsThreadPaused())
		return;

	CMouseInputMsgStruct mm;
	int TexW = pRemotePCClient->GetOpenGL()->GetTexture()->Width;
	int TexH = pRemotePCClient->GetOpenGL()->GetTexture()->Height;
	mm.Msg  = pRemotePCClient->GetClientInputs()->EncodeMouseButton(Button, true);
	if(pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, TexW,TexH, CheckBoxStretch->Checked, &mm.Data))
		pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseRoll(TObject *Sender, short WheelDelta)
{
	if(!LogedIn || pRemotePCClient->GetThread()->IsThreadPaused())
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
	#ifdef EMULATE_OPENGL
	DesktopViewer->Invalidate();
	#endif
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
void __fastcall TMainForm::SaveScreenCoordinates()
{
	WndCoords.l  = this->Left;
	WndCoords.t  = this->Top;
	WndCoords.w  = this->Width;
	WndCoords.h  = this->Height;
	WndCoords.ws = this->WindowState;
}
void __fastcall TMainForm::RestoreScreenCoordinates()
{
	MoveWindow(Handle, WndCoords.l, WndCoords.t, WndCoords.w, WndCoords.h, TRUE);
	this->WindowState = (TWindowState)WndCoords.ws;
	this->BorderStyle = bsSizeable;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SwitchToFullscreenMode()
{
	SaveScreenCoordinates();

	int ScrW = Screen->Width;
	int ScrH = Screen->Height;

	BorderStyle = bsNone;
	SetWindowLongPtr(Handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
	MoveWindow(Handle, 0, 0, ScrW, ScrH, TRUE);

	LeftPanel->Hide();
	ConnectionPanel->Hide();
	ViewerPanel->BevelOuter = bvNone;
	StatusBar->Hide();
	Splitter2->Hide();
	Splitter2->Parent = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SwitchToWindowedMode()
{
	SetWindowLongPtr(Handle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	BorderStyle = bsSingle;
	RestoreScreenCoordinates();

	LeftPanel->Show();
	ConnectionPanel->Show();
	Splitter2->Parent = MainForm;
	Splitter2->Left = ViewerPanel->Left;
	Splitter2->Show();
	ViewerPanel->BevelOuter = bvRaised;
	StatusBar->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NetworkSpeedTimerTimer(TObject *Sender)
{
	CNetStats* pStats = pRemotePCClient->GetNetManager()->GetStats();
	FormatNetworkSpeedStats(pStats, StatusBar);
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonPauseClick(TObject *Sender)
{
	if(pRemotePCClient->GetThread()->IsThreadRunning()){
		if(!pRemotePCClient->GetThread()->IsThreadPaused()){
			pRemotePCClient->GetThread()->PauseThread();
			ButtonPause->Caption = szButtonPauseCaption2[LangID];
		} else {
			pRemotePCClient->GetThread()->ResumeThread();
			ButtonPause->Caption = szButtonPauseCaption1[LangID];
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
	TMenuItem* it = dynamic_cast<TMenuItem*>(Sender);
	if (it != 0){
		SetLanguage(it->Tag);
		EnglishMenu->Checked = it->Tag == 0;
		FrenchMenu->Checked  = it->Tag == 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboBoxScrFormatChange(TObject *Sender)
{
	int n = ComboBoxScrFormat->ItemIndex;
	if(n < scrf_32){n = scrf_32;}
	if(n > scrf_1){n = scrf_1;}

	pRemotePCClient->SetScreenshotFormat((ScrFormat)n);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TabControlChange(TObject *Sender)
{
	ListBoxLog->Visible = TabControl->TabIndex == 0;
	PanelFilesManager->Visible = TabControl->TabIndex == 1;
	Panel1->Visible = TabControl->TabIndex == 2;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ClearLogMenuClick(TObject *Sender)
{
	ListBoxLog->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonSearchDrivesClick(TObject *Sender)
{
//	pRemotePCClient->SendGetDrivesRequest();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ComboBoxAvailableDrivesChange(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

