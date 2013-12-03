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
void __fastcall TMainForm::WndProc(Messages::TMessage &Message)
{
	switch(Message.Msg)
	{
	case WM_SOCKET:
		if(pRemotePCClient)
			pRemotePCClient->ProcessWinsockMessages(Message.LParam);
		break;
	case ON_CONN_THREAD_START:
		if(!CheckBoxConnectAsServer->Checked){
			AddListboxMessageArg(ListBox, "Connecting");
		} else {
			AddListboxMessageArg(ListBox, "Listening...");
		}
		break;
	case ON_CONNECTED:
		AddListboxMessageArg(ListBox, "Connection established!");
		break;
	case ON_DISCONNECTED:
		AddListboxMessageArg(ListBox, "Disconnected");
		break;
	case ON_CONNECTION_LOST:
		AddListboxMessageArg(ListBox, "Connection closed by peer.");
		break;
	case ON_CONNECTION_CANCELED:
		AddListboxMessageArg(ListBox, "Connection Canceled...");
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
			}
		}
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
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDisconnectClick(TObject *Sender)
{
	pRemotePCClient->Disconnect();
}
//---------------------------------------------------------------------------

