#pragma once
//////////////////////////////////////////////
#define MAX_PACKET_SIZE				4096
//////////////////////////////////////////////
#define WM_SOCKET					WM_USER + 150
//////////////////////////////////////////////
// Shared
#define ON_THREAD_START				WM_USER + 151
#define ON_CONNECTED				WM_USER + 152
#define ON_DISCONNECTED				WM_USER + 153
#define ON_CONNECTION_LOST			WM_USER + 154
//////////////////////////////////////////////
// Server only
#define ON_CONNECTION_CANCELED		WM_USER + 155
//////////////////////////////////////////////
// Client only
#define ON_CONNECTION_TIMED_OUT		WM_USER + 156
#define ON_LOGIN					WM_USER + 157
//////////////////////////////////////////////
#define ON_READLOOP_MSG				WM_USER + 158
//////////////////////////////////////////////
#define ON_UPDATE_SCROLLBARS_MSG	WM_USER + 159
//////////////////////////////////////////////
//#define WM_AVAILDRIVE_CB_CLEAR		WM_USER + 160
//#define WM_AVAILDRIVE_CB_ADDSTR		WM_USER + 161
//////////////////////////////////////////////
#define READLOOP_START				1
#define READLOOP_END				2
#define READLOOP_SS_HEADER_SEND		3
#define READLOOP_SS_HEADER_RECV		3
#define READLOOP_SS_DATA_SEND		4
#define READLOOP_SS_DATA_RECV		4
#define READLOOP_KB_DATA_SEND		5
#define READLOOP_KB_DATA_RECV		5
#define READLOOP_MS_DATA_SEND		6
#define READLOOP_MS_DATA_RECV		6
//////////////////////////////////////////////
//////////////////////////////////////////////
#define OP_IO_READ     1
#define OP_IO_WRITE    2
//////////////////////////////////////////////
