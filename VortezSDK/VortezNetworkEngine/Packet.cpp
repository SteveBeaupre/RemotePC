#include "Packet.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CPacket::CPacket()
{
	pHeader = (BYTE*)&Header;
	pData   = NULL;
	Init();
}

//-----------------------------------------------------------------------------
// Initialise the object
//-----------------------------------------------------------------------------
void CPacket::Init()
{
	HeaderIndx     = 0;
	DataIndx       = 0;
	Header.MsgSize = 0;
	Header.MsgID   = 0;
	FreeDataBuffer();
}

//-----------------------------------------------------------------------------
// Allocate our Data buffer
//-----------------------------------------------------------------------------
void CPacket::AllocateDataBuffer()
{
	if(!pData && IsHeaderDoneProcessing()){
		pData = new BYTE[Header.MsgSize];
		EraseDataBuffer();
	}
}

//-----------------------------------------------------------------------------
// 0's out the buffer
//-----------------------------------------------------------------------------
void CPacket::EraseDataBuffer()
{
	if(pData && IsHeaderDoneProcessing())
		ZeroMemory(&pData[0], Header.MsgSize);	
}

//-----------------------------------------------------------------------------
// Free our Data buffer
//-----------------------------------------------------------------------------
void CPacket::FreeDataBuffer()
{
	SAFE_DELETE_ARRAY(pData);	
}

//-----------------------------------------------------------------------------
// Used to fill the Header structure
//-----------------------------------------------------------------------------
void CPacket::FillHeader(DWORD dwMsgSize, DWORD dwMsgID)
{
	Header.MsgSize = dwMsgSize;
	Header.MsgID   = dwMsgID;
}

//-----------------------------------------------------------------------------
// Tell if we have recv/send all the Header bytes
//-----------------------------------------------------------------------------
bool CPacket::IsHeaderDoneProcessing()
{
	return HeaderIndx == PACKET_HEADER_SIZE;
}

//-----------------------------------------------------------------------------
// Tell if we have recv/send all the Data bytes
//-----------------------------------------------------------------------------
bool CPacket::IsDataDoneProcessing()
{
	return (IsHeaderDoneProcessing()) && (DataIndx == Header.MsgSize);
}


