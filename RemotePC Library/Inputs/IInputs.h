#pragma once
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "MsgIDs.h"
//----------------------------------------------------------------------//

#pragma pack(1)
struct CMouseInputMsgStruct {
	DWORD Data;
	BYTE  Msg;
};

struct CKeyboardInputMsgStruct {
	KBDLLHOOKSTRUCT Data;
	BYTE Msg;
};
#pragma pack()


class IClientInputs {
public:
	virtual DWORD EncodeMousePosition(int x, int y, int w, int h, int ImgW, int ImgH, bool Stretched) = 0;
	virtual BYTE  EncodeMouseButton(UINT Button, bool up) = 0;
	virtual BYTE  EncodeKeyboardMsg(DWORD wParam) = 0;
	virtual bool  IsValidMouseRange(int val, int max) = 0;
};

class IServerInputs {
public:
	virtual void ProcessMouseInput(CMouseInputMsgStruct *pInputMsg) = 0;
	virtual void ProcessKeyboardInput(CKeyboardInputMsgStruct *pInputMsg) = 0;
};




