#include "Inputs.h"

//----------------------------------------------------------------------//

DWORD CClientInputs::EncodeMousePosition(int x, int y, int w, int h, int ImgW, int ImgH, bool Stretched)
{
	DWORD res, xabs, yabs;

	if(Stretched){

		xabs = (DWORD)(((float)x / (float)(w-1)) * 0xFFFF);
		yabs = (DWORD)(((float)y / (float)(h-1)) * 0xFFFF);

		res = (yabs << 16) + xabs;
	} else {

		if(ImgW >= w){
			xabs = (DWORD)(((float)x / (float)(w-1)) * 0xFFFF);
		} else {
			int realx = x - ((w - ImgW) / 2);
			if(!IsValidMouseRange(realx, ImgW)){return false;}
			xabs = (DWORD)(((float)realx / (float)(ImgW-1)) * 0xFFFF);
		}

		if(ImgH >= h){
			yabs = (DWORD)(((float)y / (float)(h-1)) * 0xFFFF);
		} else {
			int realy = y - ((h - ImgH) / 2);
			if(!IsValidMouseRange(realy, ImgH)){return false;}
			yabs = (DWORD)(((float)realy / (float)(ImgH-1)) * 0xFFFF);
		}

		res = (yabs << 16) + xabs;
	}

	return res;
}

BYTE CClientInputs::EncodeMouseButton(UINT Button, bool up)
{
	BYTE res = up ? MSG_MOUSE_BTNUP : 0;

	switch(Button)
	{
	case 0: res |= MSG_MOUSE_LB; break; // mbLeft
	case 1: res |= MSG_MOUSE_RB; break; // mbRight
	case 2: res |= MSG_MOUSE_MB; break; // mbMiddle
	}

	return res;
}

BYTE CClientInputs::EncodeKeyboardMsg(DWORD wParam)
{
	BYTE res = 0;

	switch(wParam)
	{
	case WM_KEYDOWN:    res = MSG_KEY_DOWN;     break;
	case WM_KEYUP:      res = MSG_KEY_UP;       break;
	case WM_SYSKEYDOWN: res = MSG_SYS_KEY_DOWN; break;
	case WM_SYSKEYUP:   res = MSG_SYS_KEY_UP;   break;
	}

	return res;
}

bool CClientInputs::IsValidMouseRange(int val, int max)
{
	return !(val < 0 || val >= max);
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CServerInputs::ProcessMouseInput(CMouseInputMsgStruct *pInputMsg)
{
	INPUT Input;
	ZeroMemory(&Input, sizeof(INPUT));

	Input.type = INPUT_MOUSE;
	
	if(pInputMsg->Msg & MSG_MOUSE_ROLL){
		Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		Input.mi.mouseData = pInputMsg->Data;
	} else if(pInputMsg->Msg & MSG_MOUSE_MOVE){
		Input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		Input.mi.dx = LOWORD(pInputMsg->Data);
		Input.mi.dy = HIWORD(pInputMsg->Data);
	} else if(pInputMsg->Msg & MSG_MOUSE_LB || pInputMsg->Msg & MSG_MOUSE_MB || pInputMsg->Msg & MSG_MOUSE_RB){ // <<-- check if pInputMsg->Msg & (MSG_MOUSE_LB | MSG_MOUSE_MB | MSG_MOUSE_RB) is equivalent later
		Input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
		if(pInputMsg->Msg & MSG_MOUSE_LB){
			if(pInputMsg->Msg & MSG_MOUSE_BTNUP){
				Input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
			} else {
				Input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
			}
		} else if(pInputMsg->Msg & MSG_MOUSE_MB){
			if(pInputMsg->Msg & MSG_MOUSE_BTNUP){
				Input.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP;
			} else {
				Input.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
			}
		} else if(pInputMsg->Msg & MSG_MOUSE_RB){
			if(pInputMsg->Msg & MSG_MOUSE_BTNUP){
				Input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
			} else {
				Input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
			}
		}

		// Set the x/y coord.
		Input.mi.dx = LOWORD(pInputMsg->Data);
		Input.mi.dy = HIWORD(pInputMsg->Data);
	}

	// Set the time of the message to... right now!
	Input.mi.time = GetTickCount();

	// Emulate the received input msg
	SendInput(1, &Input, sizeof(INPUT));
}

void CServerInputs::ProcessKeyboardInput(CKeyboardInputMsgStruct *pInputMsg)
{
	INPUT Input;
	ZeroMemory(&Input, sizeof(INPUT));

	Input.type = INPUT_KEYBOARD;

	Input.ki.wVk = (WORD)pInputMsg->Data.vkCode;
	Input.ki.wScan = (WORD)pInputMsg->Data.scanCode;
	Input.ki.dwExtraInfo = 0;

	DWORD kbFlags = pInputMsg->Data.flags;
	DWORD siFlags = (kbFlags & 1) | ((kbFlags & 0x80) >> 6);

	Input.ki.dwFlags = siFlags;

	Input.ki.time = GetTickCount();

	// Emulate the received input msg
	SendInput(1, &Input, sizeof(INPUT));
}






