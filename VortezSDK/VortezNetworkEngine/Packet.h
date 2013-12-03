#ifndef _CPACKET_H_
#define _CPACKET_H_
#ifdef __cplusplus
//----------------------------------------------------------------------//
#ifndef EXP_FUNC
	#define EXP_FUNC __declspec(dllexport)
#endif
//----------------------------------------------------------------------//
#include "Windows.h"
//----------------------------------------------------------------------//
#include "Preproc.h"
#include <SafeKill.h>
//----------------------------------------------------------------------//
#define MAX_PACKET_SIZE     1024
//----------------------------------------------------------------------//
#define PACKET_HEADER_SIZE  8
//----------------------------------------------------------------------//
#define MSG_HEADER_BUFFER   1
#define MSG_DATA_BUFFER     2
//----------------------------------------------------------------------//

struct _EXP_FUNC PacketHeaderStruct {
	DWORD MsgSize;
	DWORD MsgID;
};

struct _EXP_FUNC CReadResult {
	UINT BytesReaden;
	BOOL IsPacketLimitReach;
	BOOL IsBufferLimitReach;
	BOOL Succes;
};

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

class _EXP_FUNC CPacket {
public:
	CPacket();
private:
	DWORD HeaderIndx;	
	DWORD DataIndx;

	void EraseDataBuffer();
	void FreeDataBuffer();
public:
	PacketHeaderStruct Header;
	BYTE *pHeader;
	BYTE *pData;

	void Init();
	void FillHeader(DWORD dwMsgSize, DWORD dwMsgID);

	void AllocateDataBuffer();

	bool IsHeaderDoneProcessing();
	bool IsDataDoneProcessing();

	DWORD  GetHeaderIndx()    {return  HeaderIndx;}
	DWORD *GetHeaderIndxPtr() {return &HeaderIndx;}
	DWORD  GetDataIndx()      {return  DataIndx;}
	DWORD *GetDataIndxPtr()   {return &DataIndx;}
	//void  IncDataIndx(DWORD Indx){DataIndx += Indx;}
};
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
#endif
#endif //_CPACKET_H_
