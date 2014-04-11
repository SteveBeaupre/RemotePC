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
#include <Vcl.ComCtrls.hpp>
#include "trayicon.h"
//---------------------------------------------------------------------------
#include "DesktopViewer.hpp"
#include "NetworkSpeedViewer.hpp"
//---------------------------------------------------------------------------
#include "RemotePCGUI.h"
/*#include "RemotePCClient.h"
#include "KbHookDllStub.h"
#include "Settings.h"
#include "CompileDateTimeStamp.h"
#include "UnicodeToChar.h"
#include "Lang.h"
#include "IP.h"*/
#pragma comment(lib, "RemotePCLib.lib")
#include "RemotePCLib.h"
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
	TPanel *LeftPanel;
	TSplitter *Splitter1;
	TPanel *ListboxPanel;
	TPanel *SettingsPanel;
	TGroupBox *GroupBoxSettings;
	TCheckBox *CheckBoxStretch;
	TButton *ButtonPause;
	TCheckBox *CheckBoxFullscreen;
	TCheckBox *CheckBoxShowFPS;
	TPanel *ViewerPanel;
	TScrollBox *ScrollBox;
	TDesktopViewer *DesktopViewer;
	TComboBox *ComboBoxScrFormat;
	TStatusBar *StatusBar;
	TTabControl *TabControl;
	TListBox *ListBoxLog;
	TPanel *PanelFilesManager;
	TTreeView *FilesManagerTreeView;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TButton *Button11;
	TComboBox *ComboBox1;
	TButton *Button12;
	TSplitter *Splitter2;
	TPopupMenu *PopupMenuClearLog;
	TMenuItem *ClearLogMenu;
	TPanel *Panel1;
	void __fastcall ButtonConnectClick(TObject *Sender);
	void __fastcall ButtonDisconnectClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CheckBoxConnectAsServerClick(TObject *Sender);
	void __fastcall DesktopViewerMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall DesktopViewerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall DesktopViewerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall DesktopViewerMouseRoll(TObject *Sender, short WheelDelta);
	void __fastcall CheckBoxStretchClick(TObject *Sender);
	void __fastcall CheckBoxShowFPSClick(TObject *Sender);
	void __fastcall CheckBoxFullscreenClick(TObject *Sender);
	void __fastcall NetworkSpeedTimerTimer(TObject *Sender);
	void __fastcall ButtonPauseClick(TObject *Sender);
	void __fastcall EnglishMenuClick(TObject *Sender);
	void __fastcall CloseMenuClick(TObject *Sender);
	void __fastcall ComboBoxScrFormatChange(TObject *Sender);
	void __fastcall TabControlChange(TObject *Sender);
	void __fastcall ClearLogMenuClick(TObject *Sender);
private:	// User declarations
	void __fastcall SaveScreenCoordinates();
	void __fastcall RestoreScreenCoordinates();
protected:
	void __fastcall WndProc(Messages::TMessage &Message);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);

	int  LangID;
	bool LogedIn;

	//void AddListboxMessageArg(const char *fmt, ...);
	bool __fastcall IsLoopbackAddress(AnsiString s);
	void __fastcall LoadSettings();
	void __fastcall SaveSettings();
	void __fastcall EnableUI();
	void __fastcall DisableUI();
	void __fastcall SwitchToWindowedMode();
	void __fastcall SwitchToFullscreenMode();
	void __fastcall SetLanguage(int LanguageID);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
