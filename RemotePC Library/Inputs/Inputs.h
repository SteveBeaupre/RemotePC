#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "IInputs.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CClientInputs : public IClientInputs {
public:
	DWORD EncodeMousePosition(int x, int y, int w, int h, int ImgW, int ImgH, bool Stretched);
	BYTE  EncodeMouseButton(UINT Button, bool up);
	BYTE  EncodeKeyboardMsg(DWORD wParam);
	bool  IsValidMouseRange(int val, int max);
};

class EXP_FUNC CServerInputs : public IServerInputs {
public:
	void ProcessMouseInput(CMouseInputMsgStruct *pInputMsg);
	void ProcessKeyboardInput(CKeyboardInputMsgStruct *pInputMsg);
};
