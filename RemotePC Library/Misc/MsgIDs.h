#ifndef _MSG_IDS_H_
#define _MSG_IDS_H_
#ifdef __cplusplus
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//

#define MSG_CLIENT_LOGIN_REQUEST    0x0F00
#define MSG_CLIENT_LOGIN_COMPLETED  0x0F01
#define MSG_CLIENT_LOGIN_FAILED     0x0F02

//----------------------------------------------------------------------//

#define MSG_SCREENSHOT_REQUEST    0x1001
#define MSG_SCREENSHOT_REPLY      0x1002

//----------------------------------------------------------------------//

#define MSG_MOUSE_INPUT_DATA    0x2001 

#define MSG_MOUSE_MOVE            0x02 
#define MSG_MOUSE_ROLL            0x04 
#define MSG_MOUSE_LB              0x08
#define MSG_MOUSE_MB              0x10
#define MSG_MOUSE_RB              0x20
#define MSG_MOUSE_BTNUP           0x40 

#define MSG_KB_INPUT_DATA       0x4001 

#define MSG_KEY_DOWN              0x02 
#define MSG_KEY_UP                0x04 
#define MSG_SYS_KEY_DOWN          0x08
#define MSG_SYS_KEY_UP            0x10

//----------------------------------------------------------------------//

#define MSG_REMOTEPC_CLIENT_LOGIN_REQUEST    0xFF00
#define MSG_REMOTEPC_CLIENT_LOGIN_COMPLETED  0xFF01
#define MSG_REMOTEPC_CLIENT_LOGIN_FAILED     0xFF02

//----------------------------------------------------------------------//

#endif
#endif //--_MSG_IDS_H_