//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "trayicon.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
#include "RemotePCGUI.h"
//---------------------------------------------------------------------------
#pragma comment(lib, "RemotePCLib.lib")
#include "RemotePCLib.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *ConnectionPanel;
	TButton *ButtonListen;
	TButton *ButtonDisconnect;
	TListBox *ListBoxLog;
	TPanel *Panel2;
	TLabel *LabelHostIP;
	TLabel *LabelPort;
	TLabel *LabelPassword;
	TEdit *EditPort;
	TEdit *EditPassword;
	TGroupBox *GroupBoxSettings;
	TCheckBox *CheckBoxConnectAsClient;
	TCheckBox *CheckBoxRemoveWallpaper;
	TButton *ButtonClose;
	TComboBox *ComboBoxHostName;
	TCTrayIcon *CTrayIcon;
	TImageList *TrayImageList;
	TPopupMenu *TrayPopupMenu;
	TMenuItem *CloseMenu;
	TMenuItem *ListenMenu;
	TMenuItem *DisconnectMenu;
	TMenuItem *LanguageMenu;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *EnglishMenu;
	TMenuItem *FrenchMenu;
	TCheckBox *CheckBoxMultithreaded;
	TStatusBar *StatusBar;
	TTimer *NetworkSpeedTimer;
	TCheckBox *CheckBoxAllowControl;
	TPopupMenu *PopupMenuClearLog;
	TMenuItem *ClearLogMenu;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonListenClick(TObject *Sender);
	void __fastcall ButtonDisconnectClick(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CheckBoxConnectAsClientClick(TObject *Sender);
	void __fastcall EnglishMenuClick(TObject *Sender);
	void __fastcall FrenchMenuClick(TObject *Sender);
	void __fastcall NetworkSpeedTimerTimer(TObject *Sender);
	void __fastcall CheckBoxAllowControlClick(TObject *Sender);
	void __fastcall ClearLogMenuClick(TObject *Sender);
private:	// User declarations
protected:
	void __fastcall WndProc(Messages::TMessage &Message);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);

	int LangID;

	COneInstance OneInstance;
	bool __fastcall IsLoopbackAddress(AnsiString s);
	void __fastcall LoadSettings();
	void __fastcall SaveSettings();
	void __fastcall EnableUI();
	void __fastcall DisableUI();
	void __fastcall SetLanguage(int LanguageID);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
