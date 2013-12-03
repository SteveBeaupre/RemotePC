//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include "trayicon.h"
//---------------------------------------------------------------------------
#include "DesktopViewer.hpp"
#include "NetworkSpeedViewer.hpp"
//---------------------------------------------------------------------------
#include "RemotePCClient.h"
#include "UnicodeToChar.h"
#include "Listbox.h"
#include "Lang.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *ConnectionPanel;
	TLabel *LabelHostIP;
	TLabel *LabelPort;
	TLabel *LabelPassword;
	TEdit *EditPort;
	TEdit *EditPassword;
	TButton *ButtonConnect;
	TButton *ButtonDisconnect;
	TCheckBox *CheckBoxConnectAsServer;
	TComboBox *ComboBoxHostName;
	TTimer *NetworkSpeedTimer;
	TPanel *Panel1;
	TScrollBox *ScrollBox;
	TDesktopViewer *DesktopViewer;
	TPanel *SettingsPanel;
	TPanel *OptionsPanel;
	TSplitter *Splitter1;
	TPanel *NetworkSpeedCaptionPanel;
	TLabel *LabelULSpeed;
	TLabel *LabelTotalUpload;
	TGroupBox *GroupBoxSettings;
	TCheckBox *CheckBoxStretch;
	TButton *ButtonPause;
	TCheckBox *CheckBoxShowCursor;
	TCheckBox *CheckBoxFullscreen;
	TCheckBox *CheckBoxShowFPS;
	TCheckBox *CheckBoxShowSpeedGraph;
	TListBox *ListBox;
	TTimer *RenderTimer;
	TCTrayIcon *CTrayIcon;
	TImageList *TrayImageList;
	TPopupMenu *TrayPopupMenu;
	TMenuItem *ConnectMenu;
	TMenuItem *DisconnectMenu;
	TMenuItem *CloseMenu;
	TMenuItem *LanguageMenu;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *EnglishMenu;
	TMenuItem *FrenchMenu;
	TPanel *Panel2;
	TLabel *LabelDLSpeed;
	TLabel *LabelTotalDownload;
	void __fastcall ButtonConnectClick(TObject *Sender);
	void __fastcall ButtonDisconnectClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
protected:
	void __fastcall WndProc(Messages::TMessage &Message);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
