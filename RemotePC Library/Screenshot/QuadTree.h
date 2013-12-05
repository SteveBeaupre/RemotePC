#ifndef C_QUADTREE_H
#define C_QUADTREE_H
#ifdef __cplusplus

#include <Windows.h>
#include <Stdio.h>
#include "SafeKill.h"

#define ROOT_NODE    0
#define LEAF_NODE    1
#define LINK_NODE    2

//----------------------------------------------------------------------//
// The  QuadTreeStruct Structure Def.
//----------------------------------------------------------------------//
struct QuadTreeStruct {
	
	BYTE NodeType;

	WORD L,T,W,H;
	
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
	BYTE MaxDepth;
	
	BYTE BPP;

	void InitNode(QuadTreeStruct *pNode, QuadTreeStruct *pParentNode, BYTE Level, WORD l, WORD t, WORD w, WORD h);
private:
	BYTE *pBackBuffer;
	BYTE *pFrontBuffer;
	BYTE *pUncBuffer;
public:
	void SetScreenshotBuffers(BYTE *BackBuffer, BYTE *FrontBuffer);
	void SetUncompressedBuffer(BYTE *UncBuffer);
public:
	void InitTree(WORD w, WORD h, BYTE BytesPerPixel, BYTE bMaxDepth);
	void GenTree(QuadTreeStruct *pNode = NULL, BYTE Level = 0);
	void KillTree(QuadTreeStruct *pNode = NULL, BYTE Level = 0);

	DWORD TrimTree(QuadTreeStruct *pNode = NULL, BYTE Level = 0, DWORD MaxChanged = 0);
	DWORD CalcOutputBufferSize(QuadTreeStruct *pNode = NULL, BYTE Level = 0, DWORD NodeSize = 0);
	DWORD ExtractTreeData(QuadTreeStruct *pNode = NULL, BYTE Level = 0, DWORD UncBufferCpt = 0);
};


#endif
#endif //C_QUADTREE_H