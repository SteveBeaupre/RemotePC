#include "NetErrors.h"

char* GetLastErrorMessage(int err)
{
	static const int BufSize = 256;
	static char ErrMsg[BufSize];
	ZeroMemory(ErrMsg, BufSize);

	switch(err)
	{
	case 0:                  sprintf(ErrMsg, "%s", ""); break;
	case WSAEINTR:           sprintf(ErrMsg, "%s", "Interrupted system call"); break;
    case WSAEBADF:           sprintf(ErrMsg, "%s", "Bad file number"); break;
    case WSAEACCES:          sprintf(ErrMsg, "%s", "Permission denied"); break;
    case WSAEFAULT:          sprintf(ErrMsg, "%s", "Bad address"); break;
    case WSAEINVAL:          sprintf(ErrMsg, "%s", "Invalid argument"); break;
    case WSAEMFILE:          sprintf(ErrMsg, "%s", "Too many open sockets"); break;
    case WSAEWOULDBLOCK:     sprintf(ErrMsg, "%s", "Operation would block"); break;
    case WSAEINPROGRESS:     sprintf(ErrMsg, "%s", "Operation now in progress"); break;
    case WSAEALREADY:        sprintf(ErrMsg, "%s", "Operation already in progress"); break;
    case WSAENOTSOCK:        sprintf(ErrMsg, "%s", "Socket operation on non-socket"); break;
    case WSAEDESTADDRREQ:    sprintf(ErrMsg, "%s", "Destination address required"); break;
    case WSAEMSGSIZE:        sprintf(ErrMsg, "%s", "Message too long"); break;
    case WSAEPROTOTYPE:      sprintf(ErrMsg, "%s", "Protocol wrong type for socket"); break;
    case WSAENOPROTOOPT:     sprintf(ErrMsg, "%s", "Bad protocol option"); break;
    case WSAEPROTONOSUPPORT: sprintf(ErrMsg, "%s", "Protocol not supported"); break;
    case WSAESOCKTNOSUPPORT: sprintf(ErrMsg, "%s", "Socket type not supported"); break;
    case WSAEOPNOTSUPP:      sprintf(ErrMsg, "%s", "Operation not supported on socket"); break;
    case WSAEPFNOSUPPORT:    sprintf(ErrMsg, "%s", "Protocol family not supported"); break;
    case WSAEAFNOSUPPORT:    sprintf(ErrMsg, "%s", "Address family not supported"); break;
    case WSAEADDRINUSE:      sprintf(ErrMsg, "%s", "Address already in use"); break;
    case WSAEADDRNOTAVAIL:   sprintf(ErrMsg, "%s", "Can't assign requested address"); break;
    case WSAENETDOWN:        sprintf(ErrMsg, "%s", "Network is down"); break;
    case WSAENETUNREACH:     sprintf(ErrMsg, "%s", "Network is unreachable"); break;
    case WSAENETRESET:       sprintf(ErrMsg, "%s", "Net connection reset"); break;
    case WSAECONNABORTED:    sprintf(ErrMsg, "%s", "Software caused connection abort"); break;
    case WSAECONNRESET:      sprintf(ErrMsg, "%s", "Connection reset by peer"); break;
    case WSAENOBUFS:         sprintf(ErrMsg, "%s", "No buffer space available"); break;
    case WSAEISCONN:         sprintf(ErrMsg, "%s", "Socket is already connected"); break;
    case WSAENOTCONN:        sprintf(ErrMsg, "%s", "Socket is not connected"); break;
    case WSAESHUTDOWN:       sprintf(ErrMsg, "%s", "Can't send after socket shutdown"); break;
    case WSAETOOMANYREFS:    sprintf(ErrMsg, "%s", "Too many references: can't splice"); break;
    case WSAETIMEDOUT:       sprintf(ErrMsg, "%s", "Connection timed out"); break;
    case WSAECONNREFUSED:    sprintf(ErrMsg, "%s", "Connection refused"); break;
    case WSAELOOP:           sprintf(ErrMsg, "%s", "Too many levels of symbolic links"); break;
    case WSAENAMETOOLONG:    sprintf(ErrMsg, "%s", "File name too long"); break;
    case WSAEHOSTDOWN:       sprintf(ErrMsg, "%s", "Host is down"); break;
    case WSAEHOSTUNREACH:    sprintf(ErrMsg, "%s", "No route to host"); break;
    case WSAENOTEMPTY:       sprintf(ErrMsg, "%s", "Directory not empty"); break;
    case WSAEPROCLIM:        sprintf(ErrMsg, "%s", "Too many processes"); break;
    case WSAEUSERS:          sprintf(ErrMsg, "%s", "Too many users"); break;
    case WSAEDQUOT:          sprintf(ErrMsg, "%s", "Disc quota exceeded"); break;
    case WSAESTALE:          sprintf(ErrMsg, "%s", "Stale NFS file handle"); break;
    case WSAEREMOTE:         sprintf(ErrMsg, "%s", "Too many levels of remote in path"); break;
    case WSASYSNOTREADY:     sprintf(ErrMsg, "%s", "Network system is unavailable"); break;
    case WSAVERNOTSUPPORTED: sprintf(ErrMsg, "%s", "Winsock version out of range"); break;
    case WSANOTINITIALISED:  sprintf(ErrMsg, "%s", "WSAStartup not yet called"); break;
    case WSAEDISCON:         sprintf(ErrMsg, "%s", "Graceful shutdown in progress"); break;
    case WSAHOST_NOT_FOUND:  sprintf(ErrMsg, "%s", "Host not found"); break;
    case WSANO_DATA:         sprintf(ErrMsg, "%s", "No host data of that type was found"); break;
	}

	return &ErrMsg[0];
}

