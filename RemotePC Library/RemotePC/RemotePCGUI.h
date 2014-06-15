//---------------------------------------------------------------------------
#ifndef RemotePCGUIH
#define RemotePCGUIH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
#include "RemotePCClient.h"
#include "OneInstance.h"
//---------------------------------------------------------------------------
void GetAppDir(char *AppDir, UINT BufSize);
AnsiString GenSaveFileName(AnsiString AppDir, AnsiString SaveFileName);
void FormatNetworkSpeedStats(CNetStats* pStats, TStatusBar *StatusBar);
void AddListboxMessageArg(TListBox *pListBox, TStatusBar *pStatusBar, const char *fmt, ...);
//---------------------------------------------------------------------------
#endif
