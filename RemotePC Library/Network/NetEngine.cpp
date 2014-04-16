#include "NetEngine.h"

bool g_WinSockInitialized = false;     // Tell if WinSock has benn initialized

//-----------------------------------------------------------------------------
// Name : Initialize()
// Desc : Init. Winsock
//-----------------------------------------------------------------------------
bool InitializeWinSock()
{
	WSADATA wsaData;
	g_WinSockInitialized = WSAStartup(MAKEWORD(2,2), &wsaData) == 0;

	return g_WinSockInitialized;
}

//-----------------------------------------------------------------------------
// Name : Shutdown()
// Desc : Shutdown Winsock
//-----------------------------------------------------------------------------
void ShutdownWinSock()
{
	if(g_WinSockInitialized)
		WSACleanup();
}

//-----------------------------------------------------------------------------
// Name : IsInitialized()
// Desc : ...
//-----------------------------------------------------------------------------
bool IsWinSockInitialized(){
	return g_WinSockInitialized;
}

//-----------------------------------------------------------------------------
// Name : SetFDStuffs(SOCKET *s, fd_set *r, fd_set *w, fd_set *e)
// Desc : Prepare flags on the socket to be used by select()
//-----------------------------------------------------------------------------
void SetFDStuffs(SOCKET *s, fd_set *r, fd_set *w, fd_set *e)
{
	FD_ZERO(r);
	FD_ZERO(w);
	FD_ZERO(e);
	FD_SET(*s, r);
	FD_SET(*s, w);
	FD_SET(*s, e);
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CNetServer::CNetServer()
{
	#ifdef REMOTEPC_SERVER_PROJECT
	#ifdef NO_LOG
	Log.Disable();
	#endif
	Log.Create("C:\\Temp\\Logs\\Server Log.txt");
	#endif
	Initialize();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CNetServer::~CNetServer()
{
	Log.Close();
	Disconnect();
}

//-----------------------------------------------------------------------------
// Name : Initialize()
// Desc : ...
//-----------------------------------------------------------------------------
void CNetServer::Initialize()
{
	SetHWND(NULL);
	m_Socket = INVALID_SOCKET;
	m_Connected = false;
}

//-----------------------------------------------------------------------------
// Name : WaitForClient(HWND hWnd, short nPort, int DefTimeOut)
// Desc : Listen to an incoming connection on the selected port
//-----------------------------------------------------------------------------
bool CNetServer::WaitForClient(HWND hHostWnd, short nPort)
{
	// Return if we are already connected or trying to...
	if(WaitThread.IsThreadRunning() || IsConnected())
		return false;

	Stats.Reset();

	// The Handle of the host App.
	SetHWND(hHostWnd);

	// Create a TCP/IP stream socket to "listen" with
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Fill in the address structure
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;            
	saServer.sin_port = htons(nPort);		
	
	// Bind socket
	if(bind(m_Socket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr)) == SOCKET_ERROR){
		int err = WSAGetLastError();
		char c[64];
		sprintf(c, "Error Code: %d", err);
		MessageBoxA(GetHWND(), c, "Unable To Bind Socket.", 0);
		closesocket(m_Socket); 
		return false;
	}
	
	// Listen for incoming connection
	if(listen(m_Socket,	1) == SOCKET_ERROR){
		closesocket(m_Socket); 
		return false;
	}

	WaitThread.StartThread(WaitForClientThread, (void*)this);
	return false;
}

/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Name : WaitForClientThread(void *param)
// Desc : 
//-----------------------------------------------------------------------------
DWORD WINAPI WaitForClientThread(void *param)
{
	CNetServer *pNetEngine = (CNetServer*)param;
	HWND hWnd = pNetEngine->GetHWND();
	SOCKET *pSocket = pNetEngine->GetSocket();

	// Set the time out value
	struct timeval TimeOut;
	TimeOut.tv_sec  = 0;
	TimeOut.tv_usec = 500;

	// Notify msg
	PostMessage(hWnd, ON_THREAD_START, 0, 0);
	
	// Start an infinite loop
	while(1){

		// Break the loop when we receive the stop event message
		if(pNetEngine->WaitThread.MustExitThread()){
			closesocket(*pSocket);
			PostMessage(pNetEngine->GetHWND(), ON_CONNECTION_CANCELED, 0, 0);
			return 0;
		}

		// Prepare the flags used by select()
		fd_set ReadFDs, WriteFDs, ExceptFDs;
		SetFDStuffs(pSocket, &ReadFDs, &WriteFDs, &ExceptFDs);

		// Block 1 sec until a connection is requested
		if(select(0, &ReadFDs, &WriteFDs, &ExceptFDs, &TimeOut) != 0)
			break;
	}

	// Accept the incoming connection...
	SOCKET remoteSocket;
	remoteSocket = accept(*pSocket, NULL, NULL);
	// check for error(s)
	if(remoteSocket == INVALID_SOCKET){
		closesocket(remoteSocket);
		return 0;
	}
	closesocket(*pSocket);
	*pSocket = remoteSocket;

	// We are connected sussesfully...
	pNetEngine->SetConnectionStatus(true);
	
	WSAAsyncSelect(*pSocket, hWnd, WM_SOCKET, FD_CLOSE);
	
	// Notify msg
	PostMessage(hWnd, ON_CONNECTED, 0, 0);
	
	return 0;
}

//-----------------------------------------------------------------------------
// Name : Disconnect()
// Desc : Disconnect from the network
//-----------------------------------------------------------------------------
void CNetServer::Disconnect()
{
	// Wait for the connection thread to finish if he's running...
	if(WaitThread.IsThreadRunning()){
		WaitThread.StopThread();
		m_Connected = true; // Make sure we do a clean disconnect...
	}
	// Disconnect
	if(m_Connected){

		//Close the socket
		shutdown(m_Socket, 0);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		
		//Reset connection status flag
		m_Connected = false;

		// Notify Messages...
		PostMessage(GetHWND(), ON_DISCONNECTED, 0, 0); 
				
		// Reset the program's main window handle
		SetHWND(NULL);
	}
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CNetClient::CNetClient()
{
	#ifdef REMOTEPC_CLIENT_PROJECT
	#ifdef NO_LOG
	Log.Disable();
	#endif
	Log.Create("C:\\Temp\\Logs\\Client Log.txt");
	#endif
	Initialize();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CNetClient::~CNetClient()
{
	Log.Close();
	Disconnect();
}


//-----------------------------------------------------------------------------
// Name : Initialize()
// Desc : ...
//-----------------------------------------------------------------------------
void CNetClient::Initialize()
{
	SetHWND(NULL);
	m_Socket = INVALID_SOCKET;

	m_Connected     = false;
	m_ConTimeOutLen = 1;
	m_MaxConAttempt = 1;
}


//-----------------------------------------------------------------------------
// Name : ConnectToServer(HWND hHostWnd, short nPort, int DefTimeOut, int DefMaxAttempt)
// Desc : Connect to the network
//-----------------------------------------------------------------------------
bool CNetClient::ConnectToServer(HWND hHostWnd, char *ip, short nPort, int DefTimeOut, int DefMaxAttempt)
{
	// Return if we are already connected or trying to...
	if(ConnectThread.IsThreadRunning() || IsConnected())
		return false;

	Stats.Reset();

	// 
	m_ConTimeOutLen = DefTimeOut;
	m_MaxConAttempt = DefMaxAttempt;

	// The Handle of the host App.
	SetHWND(hHostWnd);
	
	// Create a TCP/IP stream socket to "listen" with
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Fill in the address structure
	m_saServer.sin_family = AF_INET;
	m_saServer.sin_addr.s_addr = inet_addr(ip); 
	m_saServer.sin_port = htons(nPort);

	//////////////////////////////////////////////
	
	ConnectThread.StartThread(ConnectToServerThread, (void*)this);
	return false;
}

/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Name : ConnectToServerThread(void *param)
// Desc : Start listening for an incoming connection(threaded) 
//-----------------------------------------------------------------------------
DWORD WINAPI ConnectToServerThread(void *param)
{
	CNetClient *pNetEngine = (CNetClient*)param;
	HWND hWnd = pNetEngine->GetHWND();
	SOCKET *pSocket = pNetEngine->GetSocket();

	WSAAsyncSelect(*pSocket, hWnd, WM_SOCKET, FD_CLOSE);

	// Notify msg
	PostMessage(hWnd, ON_THREAD_START, 0, 0);
	
	// Get the number of attemp we shall make
	const int NumConnAtmp = pNetEngine->GetConAttempt();
	// Not supposed to happen, but better be safe to avoid an infinite loop
	if(NumConnAtmp < 0){return 0;}

	// Loop the number of requested times
	for(int Cpt = 0; Cpt < NumConnAtmp; Cpt++){

		// Get the time
		DWORD Time = GetTickCount();

		// Try to connect
		int Res = connect(*pSocket, (SOCKADDR*)pNetEngine->GetSockAddrIn(), sizeof(struct sockaddr));
		if(Res == SOCKET_ERROR){

			Sleep(500);
			if(pNetEngine->CanSend())
				break;

			// Break the loop when we receive the stop event message
			if(pNetEngine->ConnectThread.MustExitThread()){
				closesocket(*pSocket);
				PostMessage(pNetEngine->GetHWND(), ON_CONNECTION_CANCELED, 0, 0);
				return 0;
			}

			// Test if we must abort the connection loop...
			bool LastTry = Cpt + 1 == NumConnAtmp;

			// Couldn't connect				
			if(LastTry){
				closesocket(*pNetEngine->GetSocket());
				PostMessage(pNetEngine->GetHWND(), ON_CONNECTION_TIMED_OUT, Cpt + 1, NumConnAtmp);
				return 0; 
			}
			PostMessage(pNetEngine->GetHWND(), ON_CONNECTION_TIMED_OUT, Cpt + 1, NumConnAtmp);

		} else 
			break; // Break the connect loop if we are connected
			
		// Wait until at least 1 sec have passed...
		DWORD ElapsedTime = GetTickCount() - Time;
		while(ElapsedTime < 1000){
			Sleep(50);
			ElapsedTime = GetTickCount() - Time;
		}
	}

	// We are connected sussesfully...
	pNetEngine->SetConnectionStatus(true);
	
	// Notify msg
	PostMessage(pNetEngine->GetHWND(), ON_CONNECTED, 0, 0);

	return 0;
}

//-----------------------------------------------------------------------------
// Name : Disconnect()
// Desc : Disconnect from the network
//-----------------------------------------------------------------------------
void CNetClient::Disconnect()
{
	// Wait for the connection thread to finish if he's running...
	if(ConnectThread.IsThreadRunning()){
		ConnectThread.StopThread();
		m_Connected = true; // Make sure we do a clean disconnect...
	}

	// Disconnect
	if(m_Connected){

		//Disconnect the socket
		closesocket(m_Socket);

		//Reset connection status flag
		m_Connected = false;

		// Notify Message...
		PostMessage(GetHWND(), ON_DISCONNECTED, 0, 0);

		// Reset the program's main window handle
		SetHWND(NULL);
	}
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Tell if we are ready to read data
//-----------------------------------------------------------------------------
bool CNetBase::CanRecv()
{
	// Setup the timeout for the select() call
	timeval waitd;
	waitd.tv_sec  = 0; // Make select wait up to 1 second for data
	waitd.tv_usec = 1; // and 0 milliseconds.

	// Zero the flags ready for using
	fd_set read_flags;
	FD_ZERO(&read_flags);
	
	// Set the read flag to check the write status of the socket
	FD_SET(m_Socket, &read_flags);

	// Now call select
	int Res = select(m_Socket, &read_flags,(fd_set*)0,(fd_set*)0,&waitd);
	if(Res < 0) {  // If select breaks then pause for 5 seconds
		//Sleep(3);  // then continue
		// Socket not ready to read
		return false;
	}

	// If we are ready to read...
	if(FD_ISSET(m_Socket, &read_flags)){
		// Clear the read flag
		FD_CLR(m_Socket, &read_flags);
		// Socket is ready to read
		return true;
	} else {
		// Socket not ready to read
		return false;
	}
}

//-----------------------------------------------------------------------------
// Tell if we are ready to write data
//-----------------------------------------------------------------------------
bool CNetBase::CanSend()
{
	// Setup the timeout for the select() call
	timeval waitd;
	waitd.tv_sec  = 0; // Make select wait up to 1 second for data
	waitd.tv_usec = 1; // and 0 milliseconds.

	// Zero the flags ready for using
	fd_set write_flags;
	FD_ZERO(&write_flags);
	
	// Set the write flag to check the write status of the socket
	FD_SET(m_Socket, &write_flags);

	// Now call select
	int Res = select(m_Socket, (fd_set*)0,&write_flags,(fd_set*)0,&waitd);
	if(Res < 0) {  // If select breaks then pause for 5 seconds
		//Sleep(3);  
		// Socket not ready to write
		return false;
	}

	// If we are ready to write...
	if(FD_ISSET(m_Socket, &write_flags)){
		FD_CLR(m_Socket, &write_flags);
		// Socket is ready to write
		return true;
	} else {
		// Socket not ready to write
		return false;
	}
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Recv data
//-----------------------------------------------------------------------------
bool CNetBase::Recv(BYTE *buf, int size, int *indx)
{
	return NetIO(OP_IO_READ, buf, size, indx, MAX_PACKET_SIZE);
}

//-----------------------------------------------------------------------------
// Send data
//-----------------------------------------------------------------------------
bool CNetBase::Send(BYTE *buf, int size, int *indx)
{
	return NetIO(OP_IO_WRITE, buf, size, indx, MAX_PACKET_SIZE);
}

/*****************************************************************************************/
/*****************************************************************************************/

//-----------------------------------------------------------------------------
// Recv or send data
//-----------------------------------------------------------------------------
bool CNetBase::NetIO(DWORD Op, BYTE *buf, int bufsize, int *bufindx, int MaxPacketSize)
{
	// Set Size to PacketSize if 0
	if(bufsize == 0)
		bufsize = MaxPacketSize;

	// Find how many bytes we have to read, in chunk of MaxPacketSize
	int MaxBytesToProcess = (bufsize - *bufindx);
	if(MaxBytesToProcess > MaxPacketSize)
		MaxBytesToProcess = MaxPacketSize;

	/////////////////////////////////////////////////

	int res = 0;
	// Read/Write data
	switch(Op)
	{
	case OP_IO_READ : res = recv(m_Socket, (char*)&buf[*bufindx], MaxBytesToProcess, 0); break;
	case OP_IO_WRITE: res = send(m_Socket, (char*)&buf[*bufindx], MaxBytesToProcess, 0); break;
	}
	
	/////////////////////////////////////////////////

	// Error handler
	if(res <= 0){

		int err = WSAGetLastError();

		// Print current error message, if any...
		if(err != 0)
			Log.Add("%s\n", GetLastErrorMessage(err));

		if(Op == OP_IO_WRITE){

			// Only return false on error except for WSAEWOULDBLOCK
			if(err != 0 && err != WSAEWOULDBLOCK){
				Log.Add("Send() returned false\n");
				return false;
			}
			// Make sure res is 0 (and not -1) to avoid decrementing the counter
			res = 0;
			
		} else {
			Log.Add("Recv() returned false\n");
			return false;
		}
	}

	/////////////////////////////////////////////////

	// Inc. our counters
	*bufindx += res;
	
	Log.Add("%s %d bytes (%d of %d bytes done.)\n", Op == OP_IO_READ ? "recv() read" : "send() sent", res, *bufindx, bufsize);
	
	switch(Op)
	{
	case OP_IO_READ : Stats.IncNumBytesDownloaded(res); break;
	case OP_IO_WRITE: Stats.IncNumBytesUploaded(res);   break;
	}

	/////////////////////////////////////////////////

	return true;
}
