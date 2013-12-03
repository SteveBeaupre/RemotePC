#include "VortezNetworkEngine.h"

bool IsWinsockInitialized = false;

#ifndef DONT_COMPILE_AS_DLL
//-----------------------------------------------------------------------------
// Name : DllMain() (DllMain Entry Point)
//-----------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwreason,  LPVOID lpReserved)
{
    switch(fdwreason)
	{
    case DLL_PROCESS_ATTACH: break;
    case DLL_PROCESS_DETACH: break;
    case DLL_THREAD_ATTACH:  break;
    case DLL_THREAD_DETACH:  break;
    }
    return TRUE;
}
#endif

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

_EXP_FUNC bool _EXTERNAL_LINKAGE InitializeWinsock()
{
	if(!IsWinsockInitialized){
		WSADATA wsaData;
		IsWinsockInitialized = WSAStartup(MAKEWORD(1,1), &wsaData) == 0;
	}

	return IsWinsockInitialized;
}

_EXP_FUNC void _EXTERNAL_LINKAGE ShutdownWinsock()
{
	if(IsWinsockInitialized){
		WSACleanup();
		IsWinsockInitialized = false;
	}
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Constructor...
//-----------------------------------------------------------------------------
CVortezNetworkServer::CVortezNetworkServer()
{
	m_HostWnd = NULL;
	m_Socket  = INVALID_SOCKET;
	m_MaxPlayers = 32;
	ZeroMemory(&m_VerNum[0], MAX_VERNUM_LEN);
	ZeroMemory(&m_LoginPass[0], MAX_LOGIN_PASS_LEN);

	BypassLoginSystem          = false;
	AllowDuplicatedUserName    = false;
	AllowMismatchVersionNumber = false;
	AllowInvalidPassword       = false;

	Uploaded   = 0;
	Downloaded = 0;
}

//-----------------------------------------------------------------------------
// Destructor...
//-----------------------------------------------------------------------------
CVortezNetworkServer::~CVortezNetworkServer()
{
	Disconnect();
}

//-----------------------------------------------------------------------------
// Listen to an incoming connection on the selected port
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::Listen(HWND hWnd, WORD nPort, UINT MaxPlayers, char *pLoginPass, char *pVerNum)
{
	if(IsConnected())
		return false;

	Uploaded   = 0;
	Downloaded = 0;

	// Save the host windows handle
	m_HostWnd = hWnd;

	// Save the number of maximum clients connections allowed
	m_MaxPlayers = MaxPlayers <= 64 ? MaxPlayers : 64;

	// Erase and save the client server version
	ZeroMemory(&m_VerNum[0], MAX_VERNUM_LEN);
	strcpy(&m_VerNum[0], &pVerNum[0]);

	// Erase and save the server password
	ZeroMemory(&m_LoginPass[0], MAX_LOGIN_PASS_LEN);
	strcpy(&m_LoginPass[0], &pLoginPass[0]);

	// Create a TCP/IP stream socket to "listen" with
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Fill in the server address structure
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;            
	saServer.sin_port = htons(nPort);		
	
	// Bind socket
	if(bind(m_Socket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr)) == SOCKET_ERROR){
		closesocket(m_Socket); 
		return false;
	}
	
	// Make the socket asyncronous
	WSAAsyncSelect(m_Socket, m_HostWnd, ON_WINSOCK_MESSAGE, (FD_ACCEPT | FD_READ | FD_CLOSE));

	// Listen for incoming connection
	if(listen(m_Socket,	m_MaxPlayers) == SOCKET_ERROR){
		closesocket(m_Socket); 
		return false;
	}
	
	OnServerListening(m_Socket);

	return true;
}

//-----------------------------------------------------------------------------
// Disconnect client
//-----------------------------------------------------------------------------
void CVortezNetworkServer::DisconnectClient(SOCKET s)
{
	//Disconnect one of the server's client
	UINT Indx = -1;
	CSocketNode *pNode = SocketList.GetNodeBySocket(s, &Indx);
	if(pNode){
		// Close this socket
		closesocket(pNode->Socket);
		OnClientDisconnected(pNode);
		// Delete the node
		SocketList.DeleteNode(Indx);
	}
}

//-----------------------------------------------------------------------------
// Disconnect clients
//-----------------------------------------------------------------------------
void CVortezNetworkServer::DisconnectClients()
{
	//Disconnect the server's clients
	int NumNodes = SocketList.GetNodeCount();
	for(int Cpt = NumNodes - 1; Cpt >= 0; Cpt--){
		// Get the last node 
		CSocketNode *pNode = SocketList.GetNode(Cpt);
		// Close this socket
		closesocket(pNode->Socket);
		OnClientDisconnected(pNode);
		// Delete the node
		SocketList.DeleteNode(Cpt);
	}
}

//-----------------------------------------------------------------------------
// Disconnect from the network
//-----------------------------------------------------------------------------
void CVortezNetworkServer::Disconnect()
{
	// Disconnect every clients connected
	DisconnectClients();

	//Disconnect the server
	if(IsConnected()){
		closesocket(m_Socket);
		OnServerDisconnected(m_Socket);

		m_HostWnd = NULL;
		m_Socket  = INVALID_SOCKET;
		m_MaxPlayers = 2;
		ZeroMemory(&m_LoginPass[0], MAX_LOGIN_PASS_LEN);
	}
}

bool CVortezNetworkServer::IsConnected()
{
	return m_Socket != INVALID_SOCKET;
}

//-----------------------------------------------------------------------------
// Call some virtual functions for external winsock messages processing,,,
//-----------------------------------------------------------------------------
void CVortezNetworkServer::ProcessWinsockMsg(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/*if(m_Socket != wParam)
		return;*/

	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
		{
			sockaddr_in sinRemote;
			int nAddrSize = sizeof(sinRemote);

			// Connect with him on a new socket...
			SOCKET TmpSocket = accept(wParam, (sockaddr*)&sinRemote, &nAddrSize);
			if(TmpSocket == INVALID_SOCKET){
				closesocket(TmpSocket);
				OnConnectionRefused(ERR_INVALID_SOCKET);
				break;
			}

			// Make sure this socket isn't already used
			UINT Indx = 0;
			if(SocketList.GetNodeBySocket(TmpSocket, &Indx) == NULL){

				// Check if the maximum number of players insn't already reached
				if(SocketList.GetNodeCount() < m_MaxPlayers){
					CSocketNode *pNode = SocketList.PushNode(TmpSocket);
					// Get the client ip and port #
					strcpy(pNode->ip.GetStrBuffer(), inet_ntoa(sinRemote.sin_addr)); 
					pNode->Port = ntohs(sinRemote.sin_port);
					//
					OnConnectionAccepted(pNode);
					if(!BypassLoginSystem){
						SendLoginRequestMsg(pNode->Socket);
					} else {
						pNode->IsLogedIn = true;
					}
				} else {
					// The server is full...
					closesocket(TmpSocket);
					OnConnectionRefused(ERR_SERVER_FULL);
				}
			} else {
				// This socket already used (shouldn't happen...)
				closesocket(TmpSocket);
				OnConnectionRefused(ERR_SOCKET_ALREADY_IN_USE);
			}
		}
		break;
	case FD_READ:
		// Automatically process the network messages...
		{
			// Try to find the current socket node from the list
			UINT Indx = 0;
			CSocketNode *pNode = SocketList.GetNodeBySocket((SOCKET)wParam, &Indx);
			// If the node is found, read it's data...
			if(pNode)
				pNode->MsgReader.ReadMsg(&pNode->Socket, hWnd, &Downloaded);
		}
		break;
	case FD_CLOSE:
		// Try to find the current socket node from the list
		UINT Indx = 0;
		CSocketNode *pNode = SocketList.GetNodeBySocket((SOCKET)wParam, &Indx);
		// If the node is found, read it's data...
		if(pNode){
			// Close  the client's socket
			closesocket(wParam);
			// 
			OnConnectionClosed(pNode);
			// Send a message to notify all others clients of this event
			UINT NumPlayers = SocketList.GetNodeCount();
			for(UINT Cpt = 0; Cpt < NumPlayers; Cpt++){
				CSocketNode *pTmpNode = SocketList.GetNode(Cpt);
				if(pTmpNode != pNode && pTmpNode->IsLogedIn)
					SendPlayerLogedOffMsg(pTmpNode->Socket, pTmpNode->PlayerName.GetStrBuffer());
			}
			// Delete the node
			SocketList.DeleteNode(Indx);
			
		} else {
			// Close the socket anyway...
			closesocket(wParam);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Process our network messages...
//-----------------------------------------------------------------------------
void CVortezNetworkServer::ProcessNetworkMsg(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/*if(m_Socket != wParam)
		return;*/

	CNetworkMsg *pMsg = (CNetworkMsg*)lParam;
	SOCKET s = wParam;

	// Try to find the current socket node from the list
	UINT Indx = 0;
	CSocketNode *pNode = SocketList.GetNodeBySocket(s, &Indx);
	// If the node is found...
	if(pNode){
		switch(pMsg->MsgID)
		{
		case MSG_LOGIN_DATA:
			{
				// If the player is already logged in, we'll just ignore him...
				if(pNode->IsLogedIn != false)
					break;

				CLoginRequestMsg LoginMsg;
				LoginMsg.MsgID = pMsg->MsgID;
				LoginMsg.MsgSize = pMsg->MsgSize;

				UINT Indx = 0;
				memcpy(&LoginMsg.VerNum[0], &pMsg->pData[Indx], MAX_VERNUM_LEN);
				Indx += MAX_VERNUM_LEN;
				memcpy(&LoginMsg.LoginPass[0], &pMsg->pData[Indx], MAX_LOGIN_PASS_LEN);
				Indx += MAX_LOGIN_PASS_LEN;
				memcpy(&LoginMsg.PlayerName[0], &pMsg->pData[Indx], MAX_PLAYER_NAME_LEN);
				Indx += MAX_PLAYER_NAME_LEN;
							
				// Set the player's name
				pNode->PlayerName.Erase();
				strcpy(pNode->PlayerName.GetStrBuffer(), &LoginMsg.PlayerName[0]);

				// Make sure the player's name isn't already used...
				if(!AllowDuplicatedUserName){
					UINT NumOccurenceFound = 0;
					UINT NumPlayers = SocketList.GetNodeCount();
					for(UINT Cpt = 0; Cpt < NumPlayers; Cpt++){
						CSocketNode *pTmpNode = SocketList.GetNode(Cpt);
						if(memcmp(pTmpNode->PlayerName.GetBuffer(), &LoginMsg.PlayerName[0], MAX_PLAYER_NAME_LEN) == 0)
							NumOccurenceFound++;
					}
					// If another player has this name already...
					if(NumOccurenceFound > 1){
						// Send the failed login result back to the client
						SendLoginReplyMsg(s, 0, ERR_PLAYER_NAME_IN_USE);

						// Let's the server know when someone's login failed
						OnClientLoginFailed(pNode, ERR_PLAYER_NAME_IN_USE);

						break;
					}
				}

				
				if(!AllowMismatchVersionNumber){
					// Is the version number is invalid?
					if(memcmp(&LoginMsg.VerNum[0], &m_VerNum[0], MAX_VERNUM_LEN) != 0){
						// Send the failed login result back to the client
						SendLoginReplyMsg(s, 0, ERR_VERSION_DONT_MATCH);
						// Let's the server know when someone's login failed
						OnClientLoginFailed(pNode, ERR_VERSION_DONT_MATCH);
						break;
					}
				}


				// Is the password valid?
				if(AllowInvalidPassword || memcmp(&LoginMsg.LoginPass[0], &m_LoginPass[0], MAX_LOGIN_PASS_LEN) == 0){
					// Set the login state to true
					pNode->IsLogedIn = true;

					// Send the login result back to the client
					SendLoginReplyMsg(s, pNode->PlayerID, 0);

					// Let's the used know when someone's login successfully
					OnClientLogedIn(pNode);

					// Send a message to notify all others clients of this event
					UINT NumPlayers = SocketList.GetNodeCount();
					UINT NumLogedPlayers = 0;
					for(UINT Cpt = 0; Cpt < NumPlayers; Cpt++){
						CSocketNode *pTmpNode = SocketList.GetNode(Cpt);
						if(pTmpNode->IsLogedIn){
							NumLogedPlayers++;
							if(pTmpNode != pNode)
								SendPlayerLogedInMsg(pTmpNode->Socket, pTmpNode->PlayerName.GetStrBuffer());
						}
					}

					// Check if everyone is loged in and send a message to everyone if so...
					if(NumPlayers == m_MaxPlayers && NumLogedPlayers == m_MaxPlayers){
						OnMaxPlayersLoged();
						for(UINT Cpt = 0; Cpt < NumPlayers; Cpt++){
							CSocketNode *pTmpNode = SocketList.GetNode(Cpt);
							SendEveryoneReadyMsg(pTmpNode->Socket);
						}
					}
					
				} else {
					// Send the login result back to the client
					SendLoginReplyMsg(s, 0, ERR_LOGIN_PASS_INVALID);
					// Let's the used know when someone's login failed
					OnClientLoginFailed(pNode, ERR_LOGIN_PASS_INVALID);
				}
			}
			break;

		case MSG_ON_CHAT_MSG:
			{
				UINT Indx = 0;
				UINT PlayerID = (UINT)pMsg->pData[Indx++];

				UINT PlayerNameLen = strlen((char*)&pMsg->pData[Indx]);
				CRawBuffer PlayerName(PlayerNameLen+1);
				strcpy(PlayerName.GetStrBuffer(), (char*)&pMsg->pData[Indx]);
				Indx += PlayerNameLen+1;

				UINT TextDataLen = strlen((char*)&pMsg->pData[Indx]);
				CRawBuffer TextData(TextDataLen+1);
				strcpy(TextData.GetStrBuffer(), (char*)&pMsg->pData[Indx]);

				OnChatMsg(PlayerID, PlayerName.GetStrBuffer(), TextData.GetStrBuffer());

				// Send the chat message to every client execpt the sender
				/*UINT NumPlayers = SocketList.GetNodeCount();
				for(UINT Cpt = 0; Cpt < NumPlayers; Cpt++){
					CSocketNode *pTmpNode = SocketList.GetNode(Cpt);
					if(pTmpNode->IsLogedIn)
						SendChatMsg(pTmpNode->Socket, PlayerID, PlayerName.GetStrBuffer(), TextData.GetStrBuffer());
				}*/
			}
			break;		

		default:
			// Protect us from processing unknow data from player's not logged in yet
			if(pNode->IsLogedIn){
				// An unknow message id was received, so it's not our job to process it... 
				OnCustomNetworkMsgRecv(pNode, pMsg);
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Send a message to the server
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::SendMsg(SOCKET s, CNetworkMsg *pMsg)
{
	// Error handling...
	if(s != INVALID_SOCKET && !pMsg){return false;}

	// Create a buffer to hold our full msg into
	DWORD BufSize = 8 + pMsg->MsgSize;
	CRawBuffer Buffer(BufSize);
	BYTE *pBuf = Buffer.GetBuffer();

	// Copy the msg data into one contiguous buffer
	memcpy(&pBuf[0], &pMsg->MsgSize, sizeof(DWORD));
	memcpy(&pBuf[sizeof(DWORD)], &pMsg->MsgID, sizeof(DWORD));
	if(pMsg->MsgSize > 0)
		memcpy(&pBuf[sizeof(DWORD)*2], &pMsg->pData[0], pMsg->MsgSize);


	// Store the number of bytes read
	DWORD NumBytesSended = 0;

	// Send the buffer data (might cause problem for 2k+ bytes messages)
	while(NumBytesSended < BufSize){

		// Send the buffer or the remaining portion of it
		DWORD Res = send(s, (char*)&pBuf[NumBytesSended], BufSize - NumBytesSended, 0);

		// If send() return 0, that mean the connection has been lost
		if(Res == 0)
			return false;

		if(Res == WSAEWOULDBLOCK){
			// Sleep a little bit...
			//MessageBeep(0);
			Sleep(0);
		} else {
			// Inc. our bytes read counter
			NumBytesSended += Res;
			Uploaded += Res;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Send a login request message to the client
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::SendLoginRequestMsg(SOCKET s)
{
	CNetworkMsg Msg;
	Msg.MsgSize = 0;
	Msg.MsgID = MSG_LOGIN_REQUEST;
	Msg.pData = NULL;

	bool Res = SendMsg(s, &Msg);

	return Res;
}

//-----------------------------------------------------------------------------
// Send a login result message to the client
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::SendLoginReplyMsg(SOCKET s, WORD PlayerID, WORD Err)
{
	CNetworkMsg Msg;
	Msg.MsgSize = sizeof(UINT);
	Msg.MsgID = MSG_LOGIN_REPLY;

	UINT Buf = (PlayerID << 16) | Err;

	Msg.pData = (BYTE*)&Buf;
	bool Res = SendMsg(s, &Msg);

	return Res;
}

//-----------------------------------------------------------------------------
// Tell a client that another player have loged in...
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::SendPlayerLogedInMsg(SOCKET s, char *pPlayerName)
{
	CNetworkMsg Msg;
	Msg.MsgSize = MAX_PLAYER_NAME_LEN;
	Msg.MsgID = MSG_ON_PLAYER_LOGED_IN;

	CRawBuffer Buf(MAX_PLAYER_NAME_LEN);
	strcpy(Buf.GetStrBuffer(), pPlayerName);

	Msg.pData = (BYTE*)Buf.GetBuffer();
	bool Res = SendMsg(s, &Msg);

	return Res;
}

//-----------------------------------------------------------------------------
// Tell a client that another player have loged off...
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::SendPlayerLogedOffMsg(SOCKET s, char *pPlayerName)
{
	CNetworkMsg Msg;
	Msg.MsgSize = MAX_PLAYER_NAME_LEN;
	Msg.MsgID = MSG_ON_PLAYER_LOGED_OFF;

	CRawBuffer Buf(MAX_PLAYER_NAME_LEN);
	strcpy(Buf.GetStrBuffer(), pPlayerName);

	Msg.pData = (BYTE*)Buf.GetBuffer();
	bool Res = SendMsg(s, &Msg);

	return Res;
}

//-----------------------------------------------------------------------------
// Tell a client that everyone is loged in...
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::SendEveryoneReadyMsg(SOCKET s)
{
	CNetworkMsg Msg;
	Msg.MsgSize = 0;
	Msg.MsgID = MSG_EVERYONE_READY;

	Msg.pData = NULL;
	bool Res = SendMsg(s, &Msg);

	return Res;
}

//-----------------------------------------------------------------------------
// Send a text messages to the other players...
//-----------------------------------------------------------------------------
bool CVortezNetworkServer::SendChatMsg(SOCKET s, UINT PlayerID, char* pPlayerName, char *pTextData)
{
	CNetworkMsg Msg;
	Msg.MsgSize = 1 + (strlen(pPlayerName)+1) + (strlen(pTextData)+1);
	Msg.MsgID = MSG_ON_CHAT_MSG;

	CRawBuffer Buf(Msg.MsgSize);
	BYTE *pBuf = Buf.GetBuffer();
	pBuf[0] = (BYTE)PlayerID;
	strcpy((char*)&pBuf[1], pPlayerName);
	strcpy((char*)&pBuf[strlen(pPlayerName)+2], pTextData);

	Msg.pData = pBuf;

	bool Res = SendMsg(s, &Msg);
	return Res;
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Constructor...
//-----------------------------------------------------------------------------
CVortezNetworkClient::CVortezNetworkClient()
{
	ConnTimeOutLen  = 1000;  // 1 sec. between conn. attempts
	NumConnAttempts = 15; // Try NumConnAttempts times to connect

	m_HostWnd = NULL;
	m_Socket  = INVALID_SOCKET;
	m_PlayerName.Allocate(MAX_PLAYER_NAME_LEN);
	m_LoginPass.Allocate(MAX_LOGIN_PASS_LEN);
	m_VerNum.Allocate(MAX_VERNUM_LEN);

	Uploaded   = 0;
	Downloaded = 0;
}

//-----------------------------------------------------------------------------
// Destructor...
//-----------------------------------------------------------------------------
CVortezNetworkClient::~CVortezNetworkClient()
{
	Disconnect();
	m_VerNum.Free();
	m_LoginPass.Free();
	m_PlayerName.Free();
}

//-----------------------------------------------------------------------------
// Start a thread that attemp to connect with the server
//-----------------------------------------------------------------------------
DWORD WINAPI ConnectToServerThread(void *param)
{
	CVortezNetworkClient *pClient = (CVortezNetworkClient*)param;
	HWND hWnd = pClient->GetHostWnd();
	SOCKET *pSocket = pClient->GetSocketPtr();

	// Get the number of attemp we shall make
	const int NumConnAtmp = pClient->NumConnAttempts;
	// Not supposed to happen, but better be safe to avoid an infinite loop
	if(NumConnAtmp < 0){return 0;}

	// Loop the number of requested times
	for(int Cpt = NumConnAtmp; Cpt >= 0; Cpt--){

		// Couldn't connect				
		if(Cpt <= 0){
			closesocket(*pSocket);
			*pSocket = INVALID_SOCKET;
			// Conn. timed out notification
			pClient->OnConnectionTimedOut();
			return 0; 
		}

		// Get the time
		DWORD Time = GetTickCount();

		// Try to connect
		int Res = connect(*pSocket, (SOCKADDR*)pClient->GetSockAddrInPtr(), sizeof(struct sockaddr));
		if(Res == SOCKET_ERROR){
			// Break the loop when we receive the stop event message
			if(pClient->Thread.MustExitThread()){
				closesocket(*pSocket);
				*pSocket = INVALID_SOCKET;
				return 0;
			}
			// Conn. failed notification
			pClient->OnConnectionFailed((UINT)Cpt, (UINT)NumConnAtmp);
		} else {
			break; // Break the connect loop if we are connected
		}
			
		// Wait until at least 1 sec have passed...
		DWORD ElapsedTime = GetTickCount() - Time;
		while(ElapsedTime < pClient->ConnTimeOutLen){
			Sleep(50);
			ElapsedTime = GetTickCount() - Time;
		}
	}

	// Set the socket n asyncronous mode (non-blocking) 
	WSAAsyncSelect(*pSocket, hWnd, ON_WINSOCK_MESSAGE, (FD_READ | FD_CLOSE));
	
	// Simulate a FD_CONNECT message, since we've done it ourselve
	SendMessage(hWnd, ON_WINSOCK_MESSAGE, *pSocket, (LPARAM)(DWORD)FD_CONNECT);

	return 0;
}


//-----------------------------------------------------------------------------
// Listen to an incoming connection on the selected port
//-----------------------------------------------------------------------------
bool CVortezNetworkClient::ConnectToServer(HWND hWnd, char *ip, WORD nPort, char *pPlayerName, char *pLoginPass, char *pVerNum, UINT uConnTimeOutLen, UINT uNumConnAttempts)
{
	if(Thread.IsThreadRunning())
		return false;

	if(IsConnected())
		return false;

	Uploaded   = 0;
	Downloaded = 0;

	ConnTimeOutLen  = uConnTimeOutLen;
	NumConnAttempts = uNumConnAttempts; 

	// Save the host windows handle
	m_HostWnd = hWnd;

	// Erase and save the player's name and login password
	m_PlayerName.Erase();
	strcpy(m_PlayerName.GetStrBuffer(), &pPlayerName[0]);
	m_LoginPass.Erase();
	strcpy(m_LoginPass.GetStrBuffer(), &pLoginPass[0]);
	m_VerNum.Erase();
	strcpy(m_VerNum.GetStrBuffer(), &pVerNum[0]);

	///////////////////////////////////////////////////////////////

	// Create a TCP/IP stream socket to "listen" with
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Fill in the address structure
	ZeroMemory(&saServer, sizeof(SOCKADDR_IN));
	saServer.sin_family      = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(ip); 
	saServer.sin_port        = htons(nPort);

	//////////////////////////////////////////////
	
	//WSAAsyncSelect(m_Socket, m_HostWnd, ON_WINSOCK_MESSAGE, (FD_READ | FD_CONNECT | FD_CLOSE));
	//connect(m_Socket, (SOCKADDR*)&saServer, sizeof(struct sockaddr));

	OnConnectionThreadStarted(ip, nPort);

	Thread.StartThread(ConnectToServerThread, (void*)this, "ConnectToServerThreadEvent");
	return true;
}

//-----------------------------------------------------------------------------
// Disconnect from the network
//-----------------------------------------------------------------------------
void CVortezNetworkClient::Disconnect()
{
	bool ConnCanceled = false;

	// Wait for the connection thread to finish if he's running...
	if(Thread.IsThreadRunning()){
		Thread.StopThread();
		ConnCanceled = true;
		OnConnectionCanceled();
	}

	//Disconnect the server
	if(IsConnected()){
		closesocket(m_Socket);
		if(!ConnCanceled)
			OnDisconnect(m_Socket);
	}

	m_HostWnd = NULL;
	m_Socket  = INVALID_SOCKET;
	m_PlayerName.Erase();
	m_LoginPass.Erase();
}

bool CVortezNetworkClient::IsConnected()
{
	return m_Socket != INVALID_SOCKET;
}

//-----------------------------------------------------------------------------
// Call some virtual functions for external winsock messages processing,,,
//-----------------------------------------------------------------------------
void CVortezNetworkClient::ProcessWinsockMsg(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/*if(m_Socket != wParam)
		return;*/

	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		OnConnectionEstablished(wParam);
		break;
	case FD_READ:
		// Automatically process the network messages...
		MsgReader.ReadMsg(&m_Socket, hWnd, &Downloaded);
		break;
	case FD_CLOSE:
		OnConnectionLoss(wParam, WSAGETSELECTERROR(lParam));
		break;
	}
}

//-----------------------------------------------------------------------------
// Process our network messages...
//-----------------------------------------------------------------------------
void CVortezNetworkClient::ProcessNetworkMsg(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/*if(m_Socket != wParam)
		return;*/

	CNetworkMsg *pMsg = (CNetworkMsg*)lParam;
	SOCKET s = wParam;

	switch(pMsg->MsgID)
	{
	// The server requested us to send our login information...
	case MSG_LOGIN_REQUEST:
		SendLoginDataMsg();
		break;
	// The server tell us if the login procedure was sucessful or not...
	case MSG_LOGIN_REPLY:
		{
			CLoginReplyMsg ReplyMsg;
			ReplyMsg.MsgID = pMsg->MsgID;
			ReplyMsg.MsgSize = pMsg->MsgSize;

			memcpy(&ReplyMsg.Res, pMsg->pData, sizeof(DWORD));

			UINT Err = (ReplyMsg.Res & 0x0000FFFF);

			// Is the password valid?
			if(Err == 0){
				// Set the player ID
				UINT PlayerID = (ReplyMsg.Res >> 16);
				// Let's the used know that we've loged in successfully
				OnLoginSuccess(s, PlayerID);
			} else {
				// Let's the used know that the login procedure has failed
				OnLoginFailed(s, ReplyMsg.Res);
			}
		}
		break;
	case MSG_EVERYONE_READY:
		{
			OnEveryoneReady();
		}
		break;
	case MSG_ON_PLAYER_LOGED_IN:
		{
			OnPlayerLogedIn((char*)pMsg->pData);
		}
		break;
	case MSG_ON_PLAYER_LOGED_OFF:
		{
			OnPlayerLogedOff((char*)pMsg->pData);
		}
		break;
	case MSG_ON_CHAT_MSG:
			{
				UINT Indx = 0;
				UINT PlayerID = (UINT)pMsg->pData[Indx++];

				UINT PlayerNameLen = strlen((char*)&pMsg->pData[Indx]);
				CRawBuffer PlayerName(PlayerNameLen+1);
				strcpy(PlayerName.GetStrBuffer(), (char*)&pMsg->pData[Indx]);
				Indx += PlayerNameLen+1;

				UINT TextDataLen = strlen((char*)&pMsg->pData[Indx]);
				CRawBuffer TextData(TextDataLen+1);
				strcpy(TextData.GetStrBuffer(), (char*)&pMsg->pData[Indx]);

				OnChatMsg(PlayerID, PlayerName.GetStrBuffer(), TextData.GetStrBuffer());
			}
			break;		
	default:
		// An unknow message id was received, so it's not our job to process it... 
		OnCustomNetworkMsgRecv(s, pMsg);
		break;
	}
}

//-----------------------------------------------------------------------------
// Send a message to the server
//-----------------------------------------------------------------------------
bool CVortezNetworkClient::SendMsg(CNetworkMsg *pMsg)
{
	// Error handling...
	if(!pMsg){return false;}

	// Create a buffer to hold our full msg into
	DWORD BufSize = 8 + pMsg->MsgSize;
	CRawBuffer Buffer(BufSize);
	BYTE *pBuf = Buffer.GetBuffer();

	// Copy the msg data into one contiguous buffer
	memcpy(&pBuf[0], &pMsg->MsgSize, sizeof(DWORD));
	memcpy(&pBuf[sizeof(DWORD)], &pMsg->MsgID, sizeof(DWORD));
	if(pMsg->MsgSize > 0)
		memcpy(&pBuf[sizeof(DWORD)*2], &pMsg->pData[0], pMsg->MsgSize);

	// Store the number of bytes read
	DWORD NumBytesSended = 0;

	// Send the buffer data (might cause problem for 2k+ bytes messages)
	while(NumBytesSended < BufSize){

		// Send the buffer or the remaining portion of it
		DWORD Res = send(m_Socket, (char*)&pBuf[NumBytesSended], BufSize - NumBytesSended, 0);

		// If send() return 0, that mean the connection has been lost
		if(Res == 0)
			return false;

		if(Res == WSAEWOULDBLOCK){
			// Sleep a little bit...
			//MessageBeep(0);
			Sleep(0);
		} else {
			// Inc. our bytes read counter
			NumBytesSended += Res;
			Uploaded += Res;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Send a login data message to the server
//-----------------------------------------------------------------------------
bool CVortezNetworkClient::SendLoginDataMsg()
{
	CNetworkMsg Msg;
	Msg.MsgSize = MAX_VERNUM_LEN + MAX_LOGIN_PASS_LEN + MAX_PLAYER_NAME_LEN;
	Msg.MsgID = MSG_LOGIN_DATA;

	CRawBuffer Buffer(Msg.MsgSize);
	char *pBuf = Buffer.GetStrBuffer();
	strcpy(&pBuf[0], m_VerNum.GetStrBuffer());
	strcpy(&pBuf[MAX_VERNUM_LEN], m_LoginPass.GetStrBuffer());
	strcpy(&pBuf[MAX_VERNUM_LEN + MAX_LOGIN_PASS_LEN], m_PlayerName.GetStrBuffer());
	
	Msg.pData = Buffer.GetBuffer();
	bool Res = SendMsg(&Msg);

	return Res;
}

//-----------------------------------------------------------------------------
// Send a text messages to the other players...
//-----------------------------------------------------------------------------
bool CVortezNetworkClient::SendChatMsg(UINT PlayerID, char* pPlayerName, char *pTextData)
{
	CNetworkMsg Msg;
	Msg.MsgSize = 1 + (strlen(pPlayerName)+1) + (strlen(pTextData)+1);
	Msg.MsgID = MSG_ON_CHAT_MSG;

	CRawBuffer Buf(Msg.MsgSize);
	BYTE *pBuf = Buf.GetBuffer();
	pBuf[0] = (BYTE)PlayerID;
	strcpy((char*)&pBuf[1], pPlayerName);
	strcpy((char*)&pBuf[strlen(pPlayerName)+2], pTextData);

	Msg.pData = pBuf;

	bool Res = SendMsg(&Msg);
	return Res;
}



#ifdef COMPILE_FOR_BORLAND
_EXP_FUNC AVortezNetworkClient* _EXTERNAL_LINKAGE NewVortezNetworkClient()
{
	return new CVortezNetworkClient;
}

_EXP_FUNC AVortezNetworkServer* _EXTERNAL_LINKAGE NewVortezNetworkServer()
{
	return new CVortezNetworkServer;
}
#endif