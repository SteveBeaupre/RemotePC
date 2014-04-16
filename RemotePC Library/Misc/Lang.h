#pragma once

#define REMOTEPC_LANG_ENGLISH  0
#define REMOTEPC_LANG_FRENCH   1

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

LPCSTR szEnglishMenuCaption[2] = {"English", "Anglais"};
LPCSTR szFrenchMenuCaption[2]  = {"French",  "Fran�ais"};

LPCSTR szButtonListenCaption[2]            = {"Listen...",  "Listen..."};
LPCSTR szButtonListenCaptionModeClient[2]  = {"Connect",    "Connecter"};
LPCSTR szButtonConnectCaption[2]           = {"Connect",    "Connecter"};
LPCSTR szButtonConnectCaptionModeServer[2] = {"Listen...",  "Listen..."};
LPCSTR szButtonDisconnectCaption[2]        = {"Disconnect", "D�connecter"};
LPCSTR szButtonPauseCaption1[2]            = {"Pause",      "Pause"};
LPCSTR szButtonPauseCaption2[2]            = {"Resume",     "R�sumer"};

LPCSTR szCheckBoxConnectAsClientCaption[2] = {"Reverse Connection", "Renverser Connection"};
LPCSTR szCheckBoxConnectAsServerCaption[2] = {"Reverse Connection", "Renverser Connection"};
LPCSTR szCheckBoxRemoveWallpaperCaption[2] = {"Remove Wallpaper",   "Effacer le papier paint"};
LPCSTR szCheckBoxAllowControlCaption[2]    = {"Allow Control",      "Autoriser le controle"};

LPCSTR szButtonCloseCaption[2]   = {"Close", "Fermer"};
LPCSTR szLabelPasswordCaption[2] = {"Password", "Mot de Passe"};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

LPCSTR szLogedInMsg[2] = {"Loged In.", "Log r�ussit."};
LPCSTR szWarningMsg[2] = {"Warning", "Attention"};
LPCSTR szErrorMsg[2]   = {"Error", "�rreur"};

LPCSTR szInvalidPortRangeMsg[2] = {"Invalid Port Range.", "Range du port invalide."};
LPCSTR szInvalidIPMsg[2] = {"IP Invalid.", "IP Invalide."};

LPCSTR szConnectionRefusedMsg[2] = {"Connection Refused: Invalid Password.", "Connection refus�: Mot de passe invalide."};
LPCSTR szUnableToStartServerErrMsg[2] = {"Unable to start server...", "Le d�marage du server a �chouer"};

LPCSTR szOneInstanceErrMsg[2] = {
			"Another instance of RemotePC Server is already running!\nThis application will terminate now...", 
			"Une autre instance de ce programe est en cours. Cette application va se fermer automatiquement."
	   };

/////////////////////////////////////////////////////////////////////////

LPCSTR szOnConnectionThreadStartedAsServer[2] = {"Listening...",                              "En attente de connection..."};
LPCSTR szOnConnectionThreadStartedAsClient[2] = {"Connecting...",                             "Connection..."};
LPCSTR szOnConnectionEstablished[2]           = {"Connection established.",                   "Connection �tablie."};
LPCSTR szOnDisconnect[2]                      = {"Disconnected.",                             "D�connecter."};
LPCSTR szOnConnectionLoss[2]                  = {"Connection closed by peer.",                "Connexion ferm�e Par l'h�te distant."};
LPCSTR szOnConnectionCanceled[2]              = {"Connection canceled.",                      "Connection cancel�e."};
LPCSTR szOnConnectionFailed[2]                = {"Connection failed. (attempt #%d of %d)",    "Connection �chouer. (essai #%d of %d)"};
LPCSTR szOnConnectionTimedOut[2]              = {"Unable to connect to server.",              "Server introuvable."};
LPCSTR szOnLoginSucess[2]                     = {"User loged In Sucessfully.",                "L'indentification de l'hote r�ussit."};
LPCSTR szOnLoginFailedInvPass[2]              = {"User login failed: Invalid Password.",      "L'indentification de l'hote a �chouer: Password invalide."};
LPCSTR szOnLoginFailedInvAuth[2]              = {"User login failed: Incompatible versions.", "L'indentification de l'hote a �chouer: Version incompatibles."};
LPCSTR szOnLoginFailedInvAuthTip[2]           = {"Please download the latest update.",        "Veuiller downloader la derniere version."};


/*
LPCSTR szClient_OnConnectionThreadStarted[2] = {"Connecting to %s Port %d...",                                   "Connection a %s Port %d..."};
LPCSTR szClient_OnConnectionEstablished[2]   = {"Connection Established. (Socket: 0x%8.8X)",                     "Connection r�ussit. (Socket: 0x%8.8X)"};
LPCSTR szClient_OnConnectionLoss[2]          = {"Connection Loss. (Socket: 0x%8.8X Error: %d)",                  "Connection perdu. (Socket: 0x%8.8X Error: %d)"};
LPCSTR szClient_OnConnectionFailed[2]        = {"Connection Failed. (Attempt #%d of %d)",                        "Connection �chouer. (Essai #%d of %d)"};
LPCSTR szClient_OnConnectionTimedOut[2]      = {"Connection Timed Out.",                                         "Server introuvable."};
LPCSTR szClient_OnConnectionCanceled[2]      = {"Connection Canceled.",                                          "Connection canceler."};
LPCSTR szClient_OnLoginFailedErr1[2]         = {"Login Failed. (Socket: 0x%8.8X) Error: Invalid password.",      "Erreur de login. (Socket: 0x%8.8X) Error: Mot de passe invalide."};
LPCSTR szClient_OnLoginFailedErr2[2]         = {"Login Failed. (Socket: 0x%8.8X) Error: UserName already used.", "Erreur de login. (Socket: 0x%8.8X) Error: Nom d'utilisateur deja utiliser."};
LPCSTR szClient_OnLoginFailedErr3[2]         = {"Login Failed. (Socket: 0x%8.8X) Error: Version mismatch.",      "Erreur de login. (Socket: 0x%8.8X) Error: No de version incompatible."};
LPCSTR szClient_OnDisconnect[2]              = {"Disconnected.",                                                 "D�connecter."};

LPCSTR szServer_OnConnectionAccepted[2]    = {"Connection Accepted. (%s Port: %d)",                                                  "Connection accept�. (%s Port: %d)"};
LPCSTR szServer_OnConnectionRefusedErr1[2] = {"Connection Refused: Server full.",                                                    "Connection Refus�: Server plein."};
LPCSTR szServer_OnConnectionRefusedErr2[2] = {"Connection Refused: Invalid socket.",                                                 "Connection Refus�: Socket invalide."};
LPCSTR szServer_OnConnectionRefusedErr3[2] = {"Connection Refused: Socket already in use.",                                          "Connection Refus�: Socket deja utilis�."};
LPCSTR szServer_OnConnectionClosed[2]      = {"Connection Closed. (%s Port: %d Name: \"%s\" ID: %d)",                                "Connection Fermer. (%s Port: %d Nom: \"%s\" ID: %d)"};
LPCSTR szServer_OnClientLoginFailedErr1[2] = {"Client Login Failed (%s Port: %d Name: \"%s\" ID: %d) Error: Invalid password.",      "Erreur de login. (%s Port: %d Name: \"%s\" ID: %d) Error: Mot de passe invalide."};
LPCSTR szServer_OnClientLoginFailedErr2[2] = {"Client Login Failed (%s Port: %d Name: \"%s\" ID: %d) Error: UserName already used.", "Erreur de login. (%s Port: %d Name: \"%s\" ID: %d) Error: Nom d'utilisateur deja utiliser."};
LPCSTR szServer_OnClientLoginFailedErr3[2] = {"Client Login Failed (%s Port: %d Name: \"%s\" ID: %d) Error: Version mismatch.",      "Erreur de login. (%s Port: %d Name: \"%s\" ID: %d) Error: No de version incompatible."};
LPCSTR szServer_OnServerDisconnected[2]    = {"Server Offline.",                                                                     "Server ferm�."};
LPCSTR szServer_OnServerListening[2]       = {"Listening on socket 0x%8.8X...",                                                      "Attendre conn. sur le socket 0x%8.8X..."};
LPCSTR szServer_OnClientDisconnected[2]    = {"Client Disconnected. (%s Name: \"%s\" ID: %d)",                                       "Client D�connecter. (%s Name: \"%s\" ID: %d)"};
*/
















