#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <Stdio.h>
//----------------------------------------------------------------------//
#include "SafeKill.h"
//----------------------------------------------------------------------//

enum QTNodeType {
	RootNode = 0, 
	LinkNode, 
	LeafNode, 
};

struct BlockInfoStruct {
	int BlockStart;
	int BlockWidth;
	int BlockHeight;
};

struct QuadTreeStruct {
	QTNodeType NodeType;
	BlockInfoStruct BlockInfo;

	struct QuadTreeStruct *Parent;	
	struct QuadTreeStruct *Child[4];
};

//----------------------------------------------------------------------//
// The base class used to control the QuadTreeStruct
//----------------------------------------------------------------------//
class CQuadTree {
public:
	CQuadTree();
	~CQuadTree();
private:
	QuadTreeStruct QT;
	
	int MaxDepth;

	int NumLines;
	int LinePitch;

	BYTE *pBackBuffer;
	BYTE *pFrontBuffer;
	BYTE *pUncBuffer;

	void InitNode(QuadTreeStruct *pNode, QuadTreeStruct *pParentNode, int BlockStart, int BlockWidth, int BlockHeight, int Level = 0);
public:
	void InitTree(int Pitch, int nLines, int Depth = 4);
	void GenTree(QuadTreeStruct *pNode = NULL, int Level = 0);
	void DelTree(QuadTreeStruct *pNode = NULL, int Level = 0);

	int TrimTree(QuadTreeStruct *pNode = NULL, int MaxChanged = 0, int Level = 0);

	int ExtractTreeData(QuadTreeStruct *pNode = NULL, int OutputBufferIndx = 0, int Level = 0);
	int CalcOutputBufferSize(QuadTreeStruct *pNode = NULL, int NodeSize = 0, int Level = 0);
	
	void SetScreenshotBuffers(BYTE *BackBuffer, BYTE *FrontBuffer);
	void SetUncompressedBuffer(BYTE *UncBuffer);
};

