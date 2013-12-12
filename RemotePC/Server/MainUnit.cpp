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
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	LangID = REMOTEPC_LANG_ENGLISH;

	#ifdef _DEBUG
	Position = poDefault;
	Left = 15;
	Top  = 20;
	#else
	Position = poDesktopCenter;
	#endif

	char AppCaption[256];
	SetCaption("RemotePC Server 2014", AppCaption, 256);
	Caption = AnsiString(AppCaption);

	InitializeWinSock();
	pRemotePCServer = new CRemotePCServer();

	LoadSettings();
	EnableUI();
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
	Settings.Load();
	ServerSettingsStruct *pSettings = Settings.GetSettings();

	ComboBoxHostName->Text = AnsiString(pSettings->ip);
	EditPort->Text = AnsiString(pSettings->Port);
	EditPassword->Text = AnsiString(pSettings->pw);

	CheckBoxConnectAsClient->Checked = pSettings->ConnectAsClient;
	CheckBoxRemoveWallpaper->Checked = pSettings->RemoveWallpaper;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveSettings()
{
	ServerSettingsStruct ServerSettings;
	ZeroMemory(&ServerSettings, sizeof(ServerSettingsStruct));

	ConvertUnicodeToChar(ServerSettings.ip, 16, ComboBoxHostName->Text.c_str());
	ConvertUnicodeToChar(ServerSettings.pw, 32, EditPassword->Text.c_str());
	ServerSettings.Port = _wtoi(EditPort->Text.c_str());

	ServerSettings.ConnectAsClient = CheckBoxConnectAsClient->Checked;
	ServerSettings.RemoveWallpaper = CheckBoxRemoveWallpaper->Checked;

	Settings.SetSettings(&ServerSettings);
	Settings.Save();
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
	ButtonDisconnect->Enabled = false;
	CheckBoxConnectAsClient->Enabled = true;
	CheckBoxRemoveWallpaper->Enabled = true;
	//CheckBoxMultithreaded->Enabled = true;
	ComboBoxHostName->Enabled = CheckBoxConnectAsClient->Checked;
	EditPort->Enabled = true;
	EditPassword->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DisableUI()
{
	ButtonDisconnect->Enabled = true;
	ButtonListen->Enabled = false;
	CheckBoxConnectAsClient->Enabled = false;
	CheckBoxRemoveWallpaper->Enabled = false;
	CheckBoxMultithreaded->Enabled = false;
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
		if(pRemotePCServer)
			pRemotePCServer->ProcessWinsockMessages(Message.LParam);
		break;
	case ON_THREAD_START:
		if(CheckBoxConnectAsClient->Checked){
			AddListboxMessageArg(ListBox, "Connecting");
		} else {
			AddListboxMessageArg(ListBox, "Listening...");
		}
		break;
	case ON_CONNECTED:
		AddListboxMessageArg(ListBox, "Connection established!");
		if(pRemotePCServer){
			pRemotePCServer->Reset();
			pRemotePCServer->StartThread();
		}
		break;
	case ON_DISCONNECTED:
		AddListboxMessageArg(ListBox, "Disconnected");
		if(pRemotePCServer)
			pRemotePCServer->StopThread();
		EnableUI();
		break;
	case ON_CONNECTION_LOST:
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

	case MSG_CLIENT_LOGIN_COMPLETED:
		AddListboxMessageArg(ListBox, "Client Loged In.");
		break;
	case MSG_CLIENT_LOGIN_FAILED:
		AddListboxMessageArg(ListBox, "Client Login Failed. \nClosing Connection.");
		if(pRemotePCServer)
			pRemotePCServer->Disconnect();
		break;

	case ON_LOGIN:
		switch(Message.WParam)
		{
		case TRUE:  AddListboxMessageArg(ListBox, "Client Loged In Sucessfully."); break;
		case FALSE: AddListboxMessageArg(ListBox, "Client Login Failed...");       break;
		}
		break;
	}

	TForm::WndProc(Message); // Default processing for any other message
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

	WORD Port = 9966;
	if(strlen(sPort) > 0)
		Port = atoi(sPort);

	if(CheckBoxConnectAsClient->Checked){

		const int IPBufSize = 16;
		char ip[IPBufSize];
		ConvertUnicodeToChar(ip, IPBufSize, ComboBoxHostName->Text.c_str());

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

