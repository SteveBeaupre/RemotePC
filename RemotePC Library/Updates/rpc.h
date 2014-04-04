#pragma once
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//

#define SIGNATURE_SIZE	16
#define VERSION_SIZE	8

#define MAX_PIECES_SIZE	(1024*1024)

#pragma pack(1)
struct RPCHeader {
	BYTE Signature[SIGNATURE_SIZE];
	BYTE Version[VERSION_SIZE];
	UINT PatchHash;
	UINT PatchSize;
	UINT PiecesSize;
	UINT PatchNameSize;
};
#pragma pack()

UINT CalcNumPieces(UINT PatchSize, UINT PiecesSize);
UINT CalcPieceSize(UINT PatchSize, UINT PiecesSize, UINT n);
