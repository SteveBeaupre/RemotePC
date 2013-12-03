#include "Window.h"
///////////////////////////////////////////////////////////////////////////////////////////////
CWindow::CWindow()
{
	hWindow = NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////
CWindow::CWindow(HWND h)
{
	Assign(h);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::Assign(DWORD dw)
{
	memcpy(&hWindow, &dw, sizeof(DWORD));
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::Assign(HWND hWnd)
{
	hWindow = hWnd;
}
///////////////////////////////////////////////////////////////////////////////////////////////
HWND CWindow::GetHWND()
{
	return hWindow;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::GetClsName(char *pClassName, DWORD BufferSize)
{
	ZeroMemory(pClassName, BufferSize);
	if(hWindow)
		GetClassName(hWindow, pClassName, BufferSize);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::GetText(char *pCaption, DWORD BufferSize)
{
	ZeroMemory(pCaption, BufferSize);
	if(hWindow)
		GetWindowText(hWindow, pCaption, BufferSize);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::GetScreenPos(DWORD *x, DWORD *y)
{
	if(hWindow){
		RECT r;
		GetWindowRect(hWindow, &r);
		*x = r.left;
		*y = r.top;
	} else {
		*x = 0;
		*y = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::GetClientPos(DWORD *x, DWORD *y)
{
	if(hWindow){
		RECT r;
		GetClientRect(hWindow, &r);
		*x = r.left;
		*y = r.top;
	} else {
		*x = 0;
		*y = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
DWORD CWindow::GetWidth()
{
	if(hWindow){
		RECT r;
		GetWindowRect(hWindow, &r);
		return r.right  - r.left;
	} else {
		return  0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
DWORD CWindow::GetHeight()
{
	if(hWindow){
		RECT r;
		GetWindowRect(hWindow, &r);
		return r.bottom - r.top;
	} else {
		return  0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::GetSize(DWORD *w, DWORD *h)
{
	if(hWindow){
		RECT r;
		GetWindowRect(hWindow, &r);
		*w = r.right  - r.left;
		*h = r.bottom - r.top;
	} else {
		*w = 0; *h = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
DWORD CWindow::GetClientWidth()
{
	if(hWindow){
		RECT r;
		GetClientRect(hWindow, &r);
		return r.right  - r.left;
	} else {
		return  0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
DWORD CWindow::GetClientHeight()
{
	if(hWindow){
		RECT r;
		GetClientRect(hWindow, &r);
		return r.bottom - r.top;
	} else {
		return  0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::GetClientSize(DWORD *w, DWORD *h)
{
	if(hWindow){
		RECT r;
		GetClientRect(hWindow, &r);
		*w = r.right  - r.left;
		*h = r.bottom - r.top;
	} else {
		*w = 0; *h = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::GetStyles(DWORD *pStyles)
{
	*pStyles = 0;
	if(hWindow)
		*pStyles = GetWindowLong(hWindow, GWL_STYLE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::SetText(char *pCaption)
{
	if(hWindow)
		SetWindowText(hWindow, pCaption);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::SetScreenPos(DWORD x, DWORD y)
{
	if(hWindow)
		SetWindowPos(hWindow, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::SetClientPos(DWORD x, DWORD y)
{
	if(hWindow){
		RECT r;
		GetClientRect(hWindow, &r);
		MoveWindow(hWindow, x, y, r.right  - r.left, r.bottom - r.top, TRUE);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::SetSize(DWORD w, DWORD h)
{
	if(hWindow)
		SetWindowPos(hWindow, 0, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::SetStyles(DWORD Styles)
{
	if(hWindow)
		SetWindowLong(hWindow, GWL_STYLE, Styles);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::Hide()
{
	if(hWindow)
		ShowWindow(hWindow, SW_HIDE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::Show()
{
	if(hWindow)
		ShowWindow(hWindow, SW_SHOW);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::ShowDefault()
{
	if(hWindow)
		ShowWindow(hWindow, SW_SHOWDEFAULT);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::Maximize()
{
	if(hWindow)
		ShowWindow(hWindow, SW_MAXIMIZE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::Minimize()
{
	if(hWindow)
		ShowWindow(hWindow, SW_MINIMIZE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::Restore()
{
	if(hWindow)
		ShowWindow(hWindow, SW_RESTORE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
bool CWindow::GetParentWnd(bool GetDesktopWindowOnFail)
{
	bool Res = false;
	if(hWindow){
		hWindow = GetParent(hWindow);
		Res = hWindow != NULL;
		
		if(GetDesktopWindowOnFail && hWindow == NULL)
			hWindow = GetDesktopWindow();
	}
	return Res;
}
///////////////////////////////////////////////////////////////////////////////////////////////
bool CWindow::GetFirstChild(bool RestoreValueOnFail)
{
	bool Res = false;
	HWND hMem = hWindow;
	if(hWindow){
		hWindow = GetWindow(hWindow, GW_CHILD);
		Res = hWindow != NULL;
		
		if(RestoreValueOnFail && hWindow == NULL)
			hWindow = hMem;
	}
	return Res;
}
///////////////////////////////////////////////////////////////////////////////////////////////
bool CWindow::GetLastChild(bool RestoreValueOnFail)
{
	bool Res = false;
	HWND hMem = hWindow;
	if(hWindow){
		HWND hFirstChild = GetWindow(hWindow, GW_CHILD);
		hWindow = GetWindow(hFirstChild, GW_HWNDLAST);
		Res = hWindow != NULL;
		
		if(RestoreValueOnFail && hWindow == NULL)
			hWindow = hMem;
	}
	return Res;
}
///////////////////////////////////////////////////////////////////////////////////////////////
bool CWindow::GetNextChild(bool RestoreValueOnFail)
{
	bool Res = false;
	HWND hMem = hWindow;
	if(hWindow){
		hWindow = GetWindow(hWindow, GW_HWNDNEXT);
		Res = hWindow != NULL;
		
		if(RestoreValueOnFail && hWindow == NULL)
			hWindow = hMem;
	}
	return Res;
}
///////////////////////////////////////////////////////////////////////////////////////////////
bool CWindow::GetPrevChild(bool RestoreValueOnFail)
{
	bool Res = false;
	HWND hMem = hWindow;
	if(hWindow){
		hWindow = GetWindow(hWindow, GW_HWNDPREV);
		Res = hWindow != NULL;
		
		if(RestoreValueOnFail && hWindow == NULL)
			hWindow = hMem;
	}
	return Res;
}
///////////////////////////////////////////////////////////////////////////////////////////////
UINT CWindow::GetNumChilds()
{
	UINT NumChilds = 0;
	HWND hChild = GetWindow(hWindow, GW_CHILD);

	while(hChild){
		NumChilds++;
		hChild = GetWindow(hChild, GW_HWNDNEXT);
	}

	return NumChilds;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
bool CWindow::GetFromPoint(DWORD x, DWORD y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	hWindow = WindowFromPoint(pt);
	return hWindow != NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
bool CWindow::CenterCursorOnWindow()
{
	if(hWindow){
		RECT ScrRect, WndRect;
		GetWindowRect(hWindow, &ScrRect);
		GetClientRect(hWindow, &WndRect);

		int WndWidth  = WndRect.right  - WndRect.left;
		int WndHeight = WndRect.bottom - WndRect.top;

		int CurX = ScrRect.left + (WndWidth  / 2);
		int CurY = ScrRect.top  + (WndHeight / 2);

		POINT pt;
		pt.x = CurX;
		pt.y = CurY;
		HWND hTestWnd = WindowFromPoint(pt);
		if(hTestWnd == hWindow){
			SetCursorPos(CurX, CurY);
			return true;
		}
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseMoveMsg(DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = 0;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_MOUSEMOVE, wParam, lParam);
	} else {
		SetCursorPos(x, y);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseWheelMsg(bool DirUp, WORD NumRolls, DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = (DWORD)NumRolls * 120;
		if(!DirUp)
			wParam = 0 - wParam;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_MOUSEWHEEL, wParam, lParam);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseLeftBtnDownMsg(DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = 0;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_LBUTTONDOWN, wParam, lParam);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseLeftBtnUpMsg(DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = 0;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_LBUTTONUP, wParam, lParam);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseMiddleBtnDownMsg(DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = 0;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_MBUTTONDOWN, wParam, lParam);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseMiddleBtnUpMsg(DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = 0;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_MBUTTONUP, wParam, lParam);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseRightBtnDownMsg(DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = 0;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_RBUTTONDOWN, wParam, lParam);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostMouseRightBtnUpMsg(DWORD x, DWORD y)
{
	if(hWindow){
		DWORD wParam = 0;
		DWORD lParam = ((y << 16) | (x & 0x0000FFFF));
		PostMessage(hWindow, WM_RBUTTONUP, wParam, lParam);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostKeyDownMsg(BYTE VKCode)
{
	if(hWindow){
		DWORD wParam = VKCode;
		DWORD lParam = 0;
		PostMessage(hWindow, WM_KEYDOWN, wParam, lParam);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////
void CWindow::PostKeyUpMsg(BYTE VKCode)
{
	if(hWindow){
		DWORD wParam = VKCode;
		DWORD lParam = 0;
		PostMessage(hWindow, WM_KEYUP, wParam, lParam);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void _EXTERNAL_LINKAGE FillSearchInfo(HWND hSearchWnd, CWindow::CWindowInfo *pWindowInfo)
{
	GetWindowText(hSearchWnd, &pWindowInfo->Caption[0],   MAX_PATH);
	GetClassName(hSearchWnd,  &pWindowInfo->ClassName[0], MAX_PATH);
	pWindowInfo->Styles = GetWindowLong(hSearchWnd, GWL_STYLE);

	RECT r;
	GetWindowRect(hSearchWnd, &r);
	pWindowInfo->ScreenPos.Left = r.left;
	pWindowInfo->ScreenPos.Top  = r.top;

	GetClientRect(hSearchWnd, &r);
	pWindowInfo->ClientPos.Left = r.left;
	pWindowInfo->ClientPos.Top  = r.top;
	pWindowInfo->Size.Width     = r.right  - r.left;
	pWindowInfo->Size.Height    = r.bottom - r.top;
}
///////////////////////////////////////////////////////////////////////////////////////////////
bool _EXTERNAL_LINKAGE CompareWindowInfo(CWindow::CWindowInfo *pWndInfo1, CWindow::CWindowInfo *pWndInfo2, DWORD SearchFlags)
{
	if(SearchFlags != 0){
		if(ReadBit(&SearchFlags, 1)){
			if(strcmp(&pWndInfo1->ClassName[0], &pWndInfo2->ClassName[0]) != 0)
				return false;	
		}
		if(ReadBit(&SearchFlags, 2)){
			if(strcmp(&pWndInfo1->Caption[0],   &pWndInfo2->Caption[0]) != 0)
				return false;	
		}
		if(ReadBit(&SearchFlags, 3)){
			if(memcmp(&pWndInfo1->Styles,       &pWndInfo2->Styles,     sizeof(DWORD)) != 0)
				return false;	
		}
		if(ReadBit(&SearchFlags, 4)){
			if(memcmp(&pWndInfo1->Size,         &pWndInfo2->Size,       sizeof(CWindow::CWindowSize)) != 0)
				return false;	
		}
		if(ReadBit(&SearchFlags, 5)){
			if(memcmp(&pWndInfo1->ScreenPos,    &pWndInfo2->ScreenPos,  sizeof(CWindow::CWindowPos)) != 0)
				return false;	
		}
		if(ReadBit(&SearchFlags, 6)){
			if(memcmp(&pWndInfo1->ClientPos,    &pWndInfo2->ClientPos,  sizeof(CWindow::CWindowPos)) != 0)
				return false;	
		}
		return true;	
	}

	return false;	
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
HWND _EXTERNAL_LINKAGE SearchWnd(HWND hParentWnd, CWindow::CWindowInfo *pWindowInfo, DWORD SearchFlags, bool Recurse)
{
	CWindow::CWindowInfo WindowInfo;
	CWindow::CWindowInfo *pWndInfo1 = pWindowInfo;
	CWindow::CWindowInfo *pWndInfo2 = &WindowInfo;

	HWND hSearchWnd = hParentWnd == NULL ? GetDesktopWindow() : hParentWnd;
	hSearchWnd = GetWindow(hSearchWnd, GW_CHILD);

	while(hSearchWnd){
		if(!hSearchWnd){break;}

		if(Recurse){
			HWND hChildWnd = GetWindow(hSearchWnd, GW_CHILD);
			if(hChildWnd){
				CWindow::CWindowInfo ChildWindowInfo;
				FillSearchInfo(hChildWnd, &ChildWindowInfo);
				HWND Res = SearchWnd(hSearchWnd, &ChildWindowInfo, SearchFlags, Recurse);
				if(Res != NULL)
					return Res;
			}
		}

		FillSearchInfo(hSearchWnd, pWndInfo2);
		if(CompareWindowInfo(pWndInfo1, pWndInfo2, SearchFlags))
			return hSearchWnd;


		HWND hNewSearch = GetWindow(hSearchWnd, GW_HWNDNEXT);
		if(hNewSearch == hSearchWnd)
			break;
		else
			hSearchWnd = hNewSearch;
	}

	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void _EXTERNAL_LINKAGE CalcWindowSize(HWND hWnd, int *w, int *h)
{
	if(hWnd){
		RECT r;
		GetWindowRect(hWnd, &r);

		*w = r.right  - r.left;
		*h = r.bottom - r.top;
	} else {
		*w = 0;
		*h = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
void _EXTERNAL_LINKAGE CenterRectToDeskTop(RECT *r)
{
	HWND hDesktop = GetDesktopWindow();

	int ScrW, ScrH;
	CalcWindowSize(hDesktop, &ScrW, &ScrH);

	int InputW = r->right  - r->left;
	int InputH = r->bottom - r->top;

	int NewLeft = (ScrW - InputW) / 2;
	int NewTop  = (ScrH - InputH) / 2;
	
	r->left = NewLeft;
	r->top  = NewTop;
	r->right  = NewLeft + InputW;
	r->bottom = NewTop  + InputH;
}
///////////////////////////////////////////////////////////////////////////////////////////////
HWND _EXTERNAL_LINKAGE SearchWndEx(CWindow::CWindowInfo *pParentInfo, CWindow::CWindowInfo *pTargetInfo, DWORD SearchFlags)
{
	HWND hParent = SearchWnd(NULL, pParentInfo, SearchFlags, false);
	return SearchWnd(hParent, pTargetInfo, SearchFlags, true);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

