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
CRemotePCServer *pRemotePCServer = NULL;
CServerSettings Settings;
CWallpaper Wallpaper;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
AnsiString AppDir = "";
AnsiString SaveFileName = "RemotePC Server.ini";
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
	AppDir = GetCurrentDir();

	if(!OneInstance.Check("REMOTE_PC_SERVER_2014")){
		Application->Terminate();
		return;
	}

	#ifdef _DEBUG
	Position = poDefault;
	Left = 15;
	Top  = 20;
	#endif

	char AppCaption[256];
	SetCaption("RemotePC Server 2014", AppCaption, 256);
	Caption = AnsiString(AppCaption);

	InitializeWinSock();
	pRemotePCServer = new CRemotePCServer();

	LoadSettings();
	EnableUI();

	CheckBoxAllowControl->OnClick(this);

	StatusBar->Panels->Items[0]->Text = "Disconnected.";
	StatusBar->Panels->Items[1]->Text = "D: 0 Byte  T: 0.00 Kbp\\s";
	StatusBar->Panels->Items[2]->Text = "U: 0 Byte  T: 0.00 Kbp\\s";

	Application->ShowMainForm = true;
	Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	pRemotePCServer->Disconnect();

	SAFE_DELETE_OBJECT(pRemotePCServer);
	ShutdownWinSock();

	SaveSettings();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadSettings()
{
	AnsiString fname = GenSaveFileName(AppDir, SaveFileName);
	Settings.Load(fname.c_str());

	ServerSettingsStruct *pSettings = Settings.GetSettings();

	LangID = pSettings->CommonSettings.LangID;
	SetLanguage(LangID);
	EnglishMenu->Checked = LangID == REMOTEPC_LANG_ENGLISH;
	FrenchMenu->Checked  = LangID == REMOTEPC_LANG_FRENCH;

	/*ComboBoxHostName->Items->Clear();
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
	}*/
	EditPort->Text = AnsiString(pSettings->CommonSettings.ConnectionSettings.Port);
	EditPassword->Text = AnsiString(pSettings->CommonSettings.ConnectionSettings.pw);

	CheckBoxConnectAsClient->Checked = pSettings->GUISettings.ConnectAsClient;
	CheckBoxRemoveWallpaper->Checked = pSettings->GUISettings.RemoveWallpaper;
	CheckBoxMultithreaded->Checked   = pSettings->GUISettings.MultithreadScreenshot;
	CheckBoxAllowControl->Checked    = pSettings->GUISettings.AllowControl;

	this->Position = poDesktopCenter;

	/*if(pSettings->WndCoords.l == 0 || pSettings->WndCoords.t == 0 || pSettings->WndCoords.w == 0 || pSettings->WndCoords.h == 0){
		#ifndef _DEBUG
		this->Position = poDesktopCenter;
		#endif
		return;
	}

	this->Left   = pSettings->WndCoords.l;
	this->Top    = pSettings->WndCoords.t;
	this->Width  = pSettings->WndCoords.w;
	this->Height = pSettings->WndCoords.h;*/
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveSettings()
{
	ServerSettingsStruct ServerSettings;
	ZeroMemory(&ServerSettings, sizeof(ServerSettingsStruct));

	ServerSettings.CommonSettings.LangID = LangID;

	ConvertUnicodeToChar(ServerSettings.CommonSettings.ConnectionSettings.ip, 16, ComboBoxHostName->Text.c_str());
	ConvertUnicodeToChar(ServerSettings.CommonSettings.ConnectionSettings.pw, 32, EditPassword->Text.c_str());
	ServerSettings.CommonSettings.ConnectionSettings.Port = _wtoi(EditPort->Text.c_str());

	ServerSettings.GUISettings.ConnectAsClient = CheckBoxConnectAsClient->Checked;
	ServerSettings.GUISettings.RemoveWallpaper = CheckBoxRemoveWallpaper->Checked;
	ServerSettings.GUISettings.MultithreadScreenshot = CheckBoxMultithreaded->Checked;
	ServerSettings.GUISettings.AllowControl = CheckBoxAllowControl->Checked;

	ServerSettings.CommonSettings.WndCoords.l = this->Left;
	ServerSettings.CommonSettings.WndCoords.t = this->Top;
	ServerSettings.CommonSettings.WndCoords.w = this->Width;
	ServerSettings.CommonSettings.WndCoords.h = this->Height;
	ServerSettings.CommonSettings.WndCoords.wState = 0;

	Settings.SetSettings(&ServerSettings);

	AnsiString fname = GenSaveFileName(AppDir, SaveFileName);
	Settings.Save(fname.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBoxConnectAsClientClick(TObject *Sender)
{
	EnableUI();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EnableUI()
{
	switch(CheckBoxConnectAsClient->Checked){
	case false: ButtonListen->Caption = szButtonListenCaption[LangID]; break;
	case true:  ButtonListen->Caption = szButtonListenCaptionModeClient[LangID]; break;
	}
	ButtonListen->Enabled = true;
	ListenMenu->Enabled = true;
	ButtonDisconnect->Enabled = false;
	DisconnectMenu->Enabled = false;
	CheckBoxConnectAsClient->Enabled = true;
	CheckBoxRemoveWallpaper->Enabled = true;
	CheckBoxMultithreaded->Enabled = true;
	ComboBoxHostName->Enabled = CheckBoxConnectAsClient->Checked;
	EditPort->Enabled = true;
	EditPassword->Enabled = true;
	LanguageMenu->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DisableUI()
{
	ButtonDisconnect->Enabled = true;
	DisconnectMenu->Enabled = true;
	ButtonListen->Enabled = false;
	ListenMenu->Enabled = false;
	CheckBoxConnectAsClient->Enabled = false;
	CheckBoxRemoveWallpaper->Enabled = false;
	CheckBoxMultithreaded->Enabled = false;
	ComboBoxHostName->Enabled = false;
	EditPort->Enabled = false;
	EditPassword->Enabled = false;
	LanguageMenu->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WndProc(Messages::TMessage &Message)
{
	switch(Message.Msg)
	{
	case WM_SOCKET:
		if(pRemotePCServer)
			pRemotePCServer->ProcessWinsockMessages(Message.LParam);
		break;
	case ON_THREAD_START:
		if(CheckBoxConnectAsClient->Checked){
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionThreadStartedAsClient[LangID]);
		} else {
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionThreadStartedAsServer[LangID]);
		}
		break;
	case ON_CONNECTED:
		AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionEstablished[LangID]);
		if(pRemotePCServer){
			// Reset screenshot related stuffs
			pRemotePCServer->Reset();

			// Remove the wallpaper
			if(CheckBoxRemoveWallpaper->Checked)
				Wallpaper.Remove();

			// Setup multithreaded screenshot stuffs
			pRemotePCServer->SetMultiThreadedMode(CheckBoxMultithreaded->Checked);
			if(CheckBoxMultithreaded->Checked){
				pRemotePCServer->StartScreenshotThread();
			}

			NetworkSpeedTimer->Enabled = true;

			// Start the network worker thread
			pRemotePCServer->StartThread();
		}
		break;
	case ON_DISCONNECTED:
		AddListboxMessageArg(ListBoxLog, StatusBar, szOnDisconnect[LangID]);
		NetworkSpeedTimer->Enabled = false;
		if(pRemotePCServer)
			pRemotePCServer->StopThread();
		if(CheckBoxRemoveWallpaper->Checked)
			Wallpaper.Restore();
		EnableUI();
		break;
	case ON_CONNECTION_LOST:
		AddListboxMessageArg(ListBoxLog, StatusBar, szOnConnectionLoss[LangID]);
		if(CheckBoxRemoveWallpaper->Checked)
			Wallpaper.Restore();
		EnableUI();
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
			break;
		case InvalidPassword:
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnLoginFailedInvPass[LangID]);
			break;
		case InvalidAuthorizationCode:
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnLoginFailedInvAuth[LangID]);
			AddListboxMessageArg(ListBoxLog, StatusBar, szOnLoginFailedInvAuthTip[LangID]);
			break;
		}
		break;
	}

	TForm::WndProc(Message);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonListenClick(TObject *Sender)
{
	const int PasswordBufSize = 32;
	char sPassword[PasswordBufSize];
	ConvertUnicodeToChar(sPassword, PasswordBufSize, EditPassword->Text.c_str());
	pRemotePCServer->SetLoginInfo("", sPassword);

	const int PortBufSize = 8;
	char sPort[PortBufSize];
	ConvertUnicodeToChar(sPort, PortBufSize, EditPort->Text.c_str());

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

	if(CheckBoxConnectAsClient->Checked){

		const int IPBufSize = 16;
		char ip[IPBufSize];
		ConvertUnicodeToChar(ip, IPBufSize, ComboBoxHostName->Text.c_str());

		if(!IsIPValid(ip)){
			ShowMessage(AnsiString(szInvalidIPMsg[LangID]));
			return;
		}

		pRemotePCServer->ConnectAsClient(Handle, ip, Port);
	} else {
		pRemotePCServer->ConnectAsServer(Handle, Port);
	}

	DisableUI();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDisconnectClick(TObject *Sender)
{
	pRemotePCServer->Disconnect();
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

	if(!CheckBoxConnectAsClient->Checked){
		ButtonListen->Caption = szButtonListenCaption[LangID];
	} else {
		ButtonListen->Caption = szButtonListenCaptionModeClient[LangID];
	}
	ButtonDisconnect->Caption = szButtonDisconnectCaption[LangID];
	ButtonClose->Caption = szButtonCloseCaption[LangID];

	CheckBoxConnectAsClient->Caption = szCheckBoxConnectAsClientCaption[LangID];
	LabelPassword->Caption = szLabelPasswordCaption[LangID];
	CheckBoxRemoveWallpaper->Caption = szCheckBoxRemoveWallpaperCaption[LangID];
	CheckBoxAllowControl->Caption = szCheckBoxAllowControlCaption[LangID];
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
void __fastcall TMainForm::NetworkSpeedTimerTimer(TObject *Sender)
{
	CNetStats* pStats = pRemotePCServer->GetNetManager()->GetStats();
	FormatNetworkSpeedStats(pStats, StatusBar);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBoxAllowControlClick(TObject *Sender)
{
	pRemotePCServer->EnableControl(CheckBoxAllowControl->Checked == true);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ClearLogMenuClick(TObject *Sender)
{
	ListBoxLog->Clear();
}
//---------------------------------------------------------------------------

