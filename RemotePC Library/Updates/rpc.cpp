#include "rpc.h"

UINT CalcNumPieces(UINT PatchSize, UINT PiecesSize)
{
	if(PatchSize == 0)
		return 0;

	UINT NumPieces = PatchSize / PiecesSize;
	if(PatchSize % PiecesSize != 0)
		NumPieces++;

	return NumPieces;
}

UINT CalcPieceSize(UINT PatchSize, UINT PiecesSize, UINT n)
{
	if(PatchSize == 0)
		return 0;

	UINT Size = PatchSize - (PiecesSize*n);
	if(Size > PiecesSize)
		Size = PiecesSize;

	return Size;
}
