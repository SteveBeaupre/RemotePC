//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma resource "*.dfm"
TMainForm *MainForm;
int LangID = REMOTEPC_LANG_ENGLISH;
//---------------------------------------------------------------------------
CRemotePCClient *pRemotePCClient = NULL;
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

	EnableUI();
	InitializeWinSock();

	pRemotePCClient = new CRemotePCClient();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
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
		}
		break;
	case ON_DISCONNECTED:
		AddListboxMessageArg(ListBox, "Disconnected");
		if(pRemotePCClient)
			pRemotePCClient->StopThread();
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

	case ON_LOGIN:
		switch(Message.WParam)
		{
		case TRUE:  AddListboxMessageArg(ListBox, "Login Sucessful."); break;
		case FALSE: AddListboxMessageArg(ListBox, "Login Failed.");    break;
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
	if(!pRemotePCClient->GetNetManager()->IsConnected())
		return;

	CMouseInputMsgStruct mm;

	mm.Msg  = MSG_MOUSE_MOVE;
	mm.Data = pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, 0,0, true);

	pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DesktopViewerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!pRemotePCClient->GetNetManager()->IsConnected())
		return;

	CMouseInputMsgStruct mm;

	mm.Msg  = pRemotePCClient->GetClientInputs()->EncodeMouseButton(Button, false);
	mm.Data = pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, 0,0, true);

	pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!pRemotePCClient->GetNetManager()->IsConnected())
		return;

	CMouseInputMsgStruct mm;

	mm.Msg  = pRemotePCClient->GetClientInputs()->EncodeMouseButton(Button, true);
	mm.Data = pRemotePCClient->GetClientInputs()->EncodeMousePosition(X,Y, DesktopViewer->Width, DesktopViewer->Height, 0,0, CheckBoxStretch->Checked);

	pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DesktopViewerMouseRoll(TObject *Sender, short WheelDelta)
{
	if(!pRemotePCClient->GetNetManager()->IsConnected())
		return;

	CMouseInputMsgStruct mm;

	mm.Msg  = MSG_MOUSE_ROLL;
	mm.Data = WheelDelta;

	pRemotePCClient->SendMouseMsg(&mm);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBoxStretchClick(TObject *Sender)
{
	pRemotePCClient->GetOpenGL()->SetStretchedFlag(CheckBoxStretch->Checked);
}
//---------------------------------------------------------------------------

