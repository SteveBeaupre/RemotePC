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
#include "KbHookDllStub.h"
#include "Settings.h"
#include "CompileDateTimeStamp.h"
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
	TListBox *ListBox;
	TPanel *SettingsPanel;
	TPanel *UploadPanel;
	TLabel *LabelULSpeed;
	TLabel *LabelTotalUpload;
	TPanel *DownloadPanel;
	TLabel *LabelDLSpeed;
	TLabel *LabelTotalDownload;
	TGroupBox *GroupBoxSettings;
	TCheckBox *CheckBoxStretch;
	TButton *ButtonPause;
	TCheckBox *CheckBoxFullscreen;
	TCheckBox *CheckBoxShowFPS;
	TPanel *ViewerPanel;
	TScrollBox *ScrollBox;
	TDesktopViewer *DesktopViewer;
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
private:	// User declarations
protected:
	void __fastcall WndProc(Messages::TMessage &Message);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);

	int  LangID;
	bool LogedIn;

	void __fastcall LoadSettings();
	void __fastcall SaveSettings();
	void __fastcall EnableUI();
	void __fastcall DisableUI();
	void __fastcall SwitchToWindowedMode();
	void __fastcall SwitchToFullscreenMode();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
