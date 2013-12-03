#ifndef _CWINDOW_H_
#define _CWINDOW_H_
#ifdef __cplusplus

#include "Preproc.h"

#include "Windows.h"
#include "Bits.h"

#define SEARCH_CLASS_NAME  0x00000001
#define SEARCH_CAPTION     0x00000002
#define SEARCH_STYLE       0x00000004
#define SEARCH_SIZE        0x00000008
#define SEARCH_SCR_POS     0x00000010
#define SEARCH_WND_POS     0x00000020
#define SEARCH_FULL        0x0000003F

#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL  0x020A
#endif

#ifdef COMPILE_FOR_BORLAND
class AWindow {
public:
	virtual void _INTERNAL_LINKAGE Assign(DWORD dw) = 0;
	virtual void _INTERNAL_LINKAGE Assign(HWND hWnd) = 0;
	virtual HWND _INTERNAL_LINKAGE GetHWND() = 0;

	virtual void _INTERNAL_LINKAGE GetClsName(char *pClassName, DWORD BufferSize) = 0;
	virtual void _INTERNAL_LINKAGE GetText(char *pCaption, DWORD BufferSize) = 0;
	virtual void _INTERNAL_LINKAGE GetScreenPos(DWORD *x, DWORD *y) = 0;
	virtual void _INTERNAL_LINKAGE GetClientPos(DWORD *x, DWORD *y) = 0;
	virtual void _INTERNAL_LINKAGE GetSize(DWORD *w, DWORD *h) = 0;
	virtual void _INTERNAL_LINKAGE GetStyles(DWORD *pStyles) = 0;

	virtual void _INTERNAL_LINKAGE SetText(char *pCaption) = 0;
	virtual void _INTERNAL_LINKAGE SetScreenPos(DWORD x, DWORD y) = 0;
	virtual void _INTERNAL_LINKAGE SetClientPos(DWORD x, DWORD y) = 0;
	virtual void _INTERNAL_LINKAGE SetSize(DWORD w, DWORD h) = 0;
	virtual void _INTERNAL_LINKAGE SetStyles(DWORD Styles) = 0;

	virtual void _INTERNAL_LINKAGE CenterRectToDeskTop(RECT *r) = 0;

	virtual void _INTERNAL_LINKAGE Hide() = 0;
	virtual void _INTERNAL_LINKAGE Show() = 0;
	virtual void _INTERNAL_LINKAGE ShowDefault() = 0;
	virtual void _INTERNAL_LINKAGE Maximize() = 0;
	virtual void _INTERNAL_LINKAGE Minimize() = 0;
	virtual void _INTERNAL_LINKAGE Restore() = 0;

	virtual bool _INTERNAL_LINKAGE GetParentWnd(bool GetDesktopWindowOnFail = false) = 0;
	virtual bool _INTERNAL_LINKAGE GetFirstChild(bool RestoreValueOnFail = false) = 0;
	virtual bool _INTERNAL_LINKAGE GetLastChild(bool RestoreValueOnFail = false) = 0;
	virtual bool _INTERNAL_LINKAGE GetNextChild(bool RestoreValueOnFail = false) = 0;
	virtual bool _INTERNAL_LINKAGE GetPrevChild(bool RestoreValueOnFail = false) = 0;
	virtual UINT _INTERNAL_LINKAGE GetNumChilds() = 0;

	virtual bool _INTERNAL_LINKAGE GetFromPoint(DWORD x, DWORD y) = 0;

	virtual bool _INTERNAL_LINKAGE CenterCursorOnWindow() = 0;

	virtual void _INTERNAL_LINKAGE PostMouseMoveMsg(DWORD x, DWORD y) = 0;
	virtual void _INTERNAL_LINKAGE PostMouseWheelMsg(bool DirUp, WORD NumRolls, DWORD x = 0, DWORD y = 0) = 0;
	virtual void _INTERNAL_LINKAGE PostMouseLeftBtnDownMsg(DWORD x = 0, DWORD y = 0) = 0;
	virtual void _INTERNAL_LINKAGE PostMouseLeftBtnUpMsg(DWORD x = 0, DWORD y = 0) = 0;
	virtual void _INTERNAL_LINKAGE PostMouseMiddleBtnDownMsg(DWORD x = 0, DWORD y = 0) = 0;
	virtual void _INTERNAL_LINKAGE PostMouseMiddleBtnUpMsg(DWORD x = 0, DWORD y = 0) = 0;
	virtual void _INTERNAL_LINKAGE PostMouseRightBtnDownMsg(DWORD x = 0, DWORD y = 0) = 0;
	virtual void _INTERNAL_LINKAGE PostMouseRightBtnUpMsg(DWORD x = 0, DWORD y = 0) = 0;

	virtual void _INTERNAL_LINKAGE PostKeyDownMsg(BYTE VKCode) = 0;
	virtual void _INTERNAL_LINKAGE PostKeyUpMsg(BYTE VKCode) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif


#ifdef COMPILE_FOR_BORLAND
#undef  _EXP_FUNC
#define _EXP_FUNC
class CWindow : public AWindow {
#else
class _EXP_FUNC CWindow {
#endif
public:
	CWindow();
	CWindow(HWND h);
public:
	struct _EXP_FUNC CWindowSize{UINT Width, Height;};
	struct _EXP_FUNC CWindowPos {UINT Left,  Top;};
	struct _EXP_FUNC CWindowInfo {
		char  ClassName[MAX_PATH];
		char  Caption[MAX_PATH];
		DWORD Styles;
		CWindowSize Size;
		CWindowPos  ScreenPos;
		CWindowPos  ClientPos;
	};
private:
	HWND hWindow;
public:
	void _INTERNAL_LINKAGE Assign(DWORD dw);
	void _INTERNAL_LINKAGE Assign(HWND hWnd);
	HWND _INTERNAL_LINKAGE GetHWND();

	void _INTERNAL_LINKAGE GetClsName(char *pClassName, DWORD BufferSize);
	void _INTERNAL_LINKAGE GetText(char *pCaption, DWORD BufferSize);
	void _INTERNAL_LINKAGE GetScreenPos(DWORD *x, DWORD *y);
	void _INTERNAL_LINKAGE GetClientPos(DWORD *x, DWORD *y);


	DWORD _INTERNAL_LINKAGE GetWidth();
	DWORD _INTERNAL_LINKAGE GetHeight();
	void _INTERNAL_LINKAGE GetSize(DWORD *w, DWORD *h);


	DWORD _INTERNAL_LINKAGE GetClientWidth();
	DWORD _INTERNAL_LINKAGE GetClientHeight();
	void _INTERNAL_LINKAGE GetClientSize(DWORD *w, DWORD *h);

	void _INTERNAL_LINKAGE GetStyles(DWORD *pStyles);

	void _INTERNAL_LINKAGE SetText(char *pCaption);
	void _INTERNAL_LINKAGE SetScreenPos(DWORD x, DWORD y);
	void _INTERNAL_LINKAGE SetClientPos(DWORD x, DWORD y);
	void _INTERNAL_LINKAGE SetSize(DWORD w, DWORD h);
	void _INTERNAL_LINKAGE SetStyles(DWORD Styles);

	void _INTERNAL_LINKAGE CenterRectToDeskTop(RECT *r);

	void _INTERNAL_LINKAGE Hide();
	void _INTERNAL_LINKAGE Show();
	void _INTERNAL_LINKAGE ShowDefault();
	void _INTERNAL_LINKAGE Maximize();
	void _INTERNAL_LINKAGE Minimize();
	void _INTERNAL_LINKAGE Restore();

	bool _INTERNAL_LINKAGE GetParentWnd(bool GetDesktopWindowOnFail = false);
	bool _INTERNAL_LINKAGE GetFirstChild(bool RestoreValueOnFail = false);
	bool _INTERNAL_LINKAGE GetLastChild(bool RestoreValueOnFail = false);
	bool _INTERNAL_LINKAGE GetNextChild(bool RestoreValueOnFail = false);
	bool _INTERNAL_LINKAGE GetPrevChild(bool RestoreValueOnFail = false);
	UINT _INTERNAL_LINKAGE GetNumChilds();

	bool _INTERNAL_LINKAGE GetFromPoint(DWORD x, DWORD y);

	bool _INTERNAL_LINKAGE CenterCursorOnWindow();

	void _INTERNAL_LINKAGE PostMouseMoveMsg(DWORD x, DWORD y);
	void _INTERNAL_LINKAGE PostMouseWheelMsg(bool DirUp, WORD NumRolls, DWORD x = 0, DWORD y = 0);
	void _INTERNAL_LINKAGE PostMouseLeftBtnDownMsg(DWORD x = 0, DWORD y = 0);
	void _INTERNAL_LINKAGE PostMouseLeftBtnUpMsg(DWORD x = 0, DWORD y = 0);
	void _INTERNAL_LINKAGE PostMouseMiddleBtnDownMsg(DWORD x = 0, DWORD y = 0);
	void _INTERNAL_LINKAGE PostMouseMiddleBtnUpMsg(DWORD x = 0, DWORD y = 0);
	void _INTERNAL_LINKAGE PostMouseRightBtnDownMsg(DWORD x = 0, DWORD y = 0);
	void _INTERNAL_LINKAGE PostMouseRightBtnUpMsg(DWORD x = 0, DWORD y = 0);

	void _INTERNAL_LINKAGE PostKeyDownMsg(BYTE VKCode);
	void _INTERNAL_LINKAGE PostKeyUpMsg(BYTE VKCode);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

_EXP_FUNC void _EXTERNAL_LINKAGE CalcWindowSize(HWND hWnd, int *w, int *h);
_EXP_FUNC void _EXTERNAL_LINKAGE CenterRectToDeskTop(RECT *r);

_EXP_FUNC void _EXTERNAL_LINKAGE FillSearchInfo(HWND hSearchWnd, CWindow::CWindowInfo *pWindowInfo);
_EXP_FUNC bool _EXTERNAL_LINKAGE CompareWindowInfo(CWindow::CWindowInfo *pWndInfo1, CWindow::CWindowInfo *pWndInfo2, DWORD SearchFlags);
_EXP_FUNC HWND _EXTERNAL_LINKAGE SearchWnd(HWND hParentWnd, CWindow::CWindowInfo *pWindowInfo, DWORD SearchFlags, bool Recurse);
_EXP_FUNC HWND _EXTERNAL_LINKAGE SearchWndEx(CWindow::CWindowInfo *pParentInfo, CWindow::CWindowInfo *pTargetInfo, DWORD SearchFlags);

#endif
#endif //_CWINDOW_H_/*