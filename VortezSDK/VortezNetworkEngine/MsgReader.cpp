#include "MsgReader.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CMsgReader::CMsgReader()
{
	this->Reset();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CMsgReader::Reset()
{
	MsgBuffer.Init();
}

//-----------------------------------------------------------------------------
// Read data
//-----------------------------------------------------------------------------
CReadResult CMsgReader::Read(SOCKET *pSock, UINT Source, DWORD BufSize, __int64 *pByteRecv)
{
	BYTE  *pBuf = NULL; 
	DWORD *pBufIndx = NULL;

	if(Source == MSG_HEADER_BUFFER){
		pBuf     = MsgBuffer.pHeader;
		pBufIndx = MsgBuffer.GetHeaderIndxPtr();
		BufSize  = PACKET_HEADER_SIZE;
	} else {
		pBuf     = MsgBuffer.pData;
		pBufIndx = MsgBuffer.GetDataIndxPtr();

	}

	// Find how many bytes we have to read, in chunk of MAX_PACKET_SIZE
	DWORD MaxBytesToProcess = (BufSize - *pBufIndx);
	if(MaxBytesToProcess > MAX_PACKET_SIZE)
		MaxBytesToProcess = MAX_PACKET_SIZE;

	/////////////////////////////////////////////////

	// Read data
	DWORD NumBytesRecv = recv(*pSock, (char*)&pBuf[*pBufIndx], MaxBytesToProcess, 0);
	
	/////////////////////////////////////////////////

	CReadResult Res;
	ZeroMemory(&Res, sizeof(CReadResult));

	/////////////////////////////////////////////////

	if(NumBytesRecv != 0 && NumBytesRecv != 0xFFFFFFFF){
		// Inc. our counters
		*pBufIndx  += NumBytesRecv;
		*pByteRecv += NumBytesRecv;
		// Store how many bytes we've read
		Res.BytesReaden = NumBytesRecv; 
		// Have we finished reading/writing data?   
		Res.IsBufferLimitReach = *pBufIndx == BufSize;
		// Have we hit M or Size limit?
		Res.IsPacketLimitReach = NumBytesRecv == MAX_PACKET_SIZE;
		// Return sucess...
		Res.Succes = TRUE;
	}

	return Res;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CMsgReader::ReadMsg(SOCKET *pSock, HWND hHostWnd, __int64 *pByteRecv)
{
	// Read the Header of the message
	if(!MsgBuffer.IsHeaderDoneProcessing()){
		
		// Begin or resume reading the message header
		CReadResult Res = Read(pSock, MSG_HEADER_BUFFER, PACKET_HEADER_SIZE, pByteRecv);
		if(!Res.Succes)
			return FALSE;

		// Are we done reading the msg header?
		if(MsgBuffer.IsHeaderDoneProcessing()){

			// Do we have some msg data left or available to read?
			if(MsgBuffer.IsDataDoneProcessing()){
			
				CNetworkMsg Msg;
				Msg.MsgID = MsgBuffer.Header.MsgID;
				Msg.MsgSize = MsgBuffer.Header.MsgSize;
				Msg.pData = MsgBuffer.pData;
				
				//ProcessMsg(&Msg);
				SendMessage(hHostWnd, ON_NETWORK_MESSAGE, (WPARAM)*pSock, (LPARAM)&Msg);
				
				MsgBuffer.Init();
				return TRUE;
			} else {
				MsgBuffer.AllocateDataBuffer();
			}				
		}
	} else if(!MsgBuffer.IsDataDoneProcessing()){   // The header is completely readen, 
			                                        // but not the data
		// Begin or resume reading the message data
		CReadResult Res = Read(pSock, MSG_DATA_BUFFER, MsgBuffer.Header.MsgSize, pByteRecv);
		if(!Res.Succes)
			return FALSE;

		// Do we have some msg data left or available to read?
		if(MsgBuffer.IsDataDoneProcessing()){
			
			CNetworkMsg Msg;
			Msg.MsgID = MsgBuffer.Header.MsgID;
			Msg.MsgSize = MsgBuffer.Header.MsgSize;
			Msg.pData = MsgBuffer.pData;
			
			//ProcessMsg(&Msg);
			SendMessage(hHostWnd, ON_NETWORK_MESSAGE, (WPARAM)*pSock, (LPARAM)&Msg);

			MsgBuffer.Init();
			return TRUE;
		}
	}

	return 0;
}
