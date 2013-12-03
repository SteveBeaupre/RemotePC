//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma resource "*.dfm"
TMainForm *MainForm;
CRemotePCServer *pRemotePCServer = NULL;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	InitializeWinSock();
	pRemotePCServer = new CRemotePCServer();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	pRemotePCServer->Disconnect();
	SAFE_DELETE_OBJECT(pRemotePCServer);
	ShutdownWinSock();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonCloseClick(TObject *Sender)
{
	Close();
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
	case ON_CONN_THREAD_START:
		if(CheckBoxConnectAsClient->Checked){
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
void __fastcall TMainForm::ButtonListenClick(TObject *Sender)
{
	const int BufSize = 8;
	char sPort[BufSize];
	ConvertUnicodeToChar(sPort, BufSize, EditPort->Text.c_str());

	WORD Port = 9966;
	if(strlen(sPort) > 0)
		Port = atoi(sPort);

	if(CheckBoxConnectAsClient->Checked){

		const int BufSize = 16;
		char ip[BufSize];
		ConvertUnicodeToChar(ip, BufSize, ComboBoxHostName->Text.c_str());

		pRemotePCServer->ConnectAsClient(Handle, ip, Port);
	} else {
		pRemotePCServer->ConnectAsServer(Handle, Port);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDisconnectClick(TObject *Sender)
{
	pRemotePCServer->Disconnect();
}
//---------------------------------------------------------------------------


