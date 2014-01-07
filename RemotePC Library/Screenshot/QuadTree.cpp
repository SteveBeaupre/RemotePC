#include "QuadTree.h"

CQuadTree::CQuadTree()
{
	pBackBuffer  = NULL;
	pFrontBuffer = NULL;
	pUncBuffer   = NULL;
		
	MaxDepth = 0;

	NumLines  = 0;
	LinePitch = 0;

	InitNode(&QT, NULL, 0,0,0);
}

CQuadTree::~CQuadTree()
{
	DelTree();
}

void CQuadTree::SetScreenshotBuffers(BYTE *BackBuffer, BYTE *FrontBuffer)
{
	pBackBuffer  = BackBuffer;
	pFrontBuffer = FrontBuffer;
}

void CQuadTree::SetUncompressedBuffer(BYTE *UncBuffer)
{
	pUncBuffer = UncBuffer;
}

void CQuadTree::InitNode(QuadTreeStruct *pNode, QuadTreeStruct *pParentNode, int BlockStart, int BlockWidth, int BlockHeight, int Level)
{
	// Inititialise the node type
	if(Level == 0){
		pNode->NodeType = RootNode;
	} else if(Level == MaxDepth){
		pNode->NodeType = LeafNode;
	} else {
		pNode->NodeType = LinkNode;
	}
	
	// Inititialise the node's data
	pNode->BlockInfo.BlockStart  = BlockStart;
	pNode->BlockInfo.BlockWidth  = BlockWidth;
	pNode->BlockInfo.BlockHeight = BlockHeight;
	
	// Setup a pointer to the parent node
	pNode->Parent = pParentNode;
	// Initialise the childs nodes to NULL
	for(int i = 0; i < 4; i++)
		pNode->Child[i] = NULL;
}

void CQuadTree::InitTree(int Pitch, int nLines, int Depth)
{
	MaxDepth = Depth;
	if(MaxDepth > 8)
		MaxDepth = 8;
	
	NumLines  = nLines;
	LinePitch = Pitch;
	
	InitNode(&QT, NULL, 0, Pitch, nLines);
}

void CQuadTree::GenTree(QuadTreeStruct *pNode, int Level)
{
	int i = 0;

	// Return if MaxDepth = 0
	if(MaxDepth == 0){return;}
	
	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;
	
	// Allocate memory for the new nodes
	for(i = 0; i < 4; i++)
		pNode->Child[i] = new QuadTreeStruct;
	
	// Divide our block width and heigh by 2
	int wDiv2 = pNode->BlockInfo.BlockWidth  / 2;
	int hDiv2 = pNode->BlockInfo.BlockHeight / 2;

	// Add +1 to odd width and height parts
	bool IsWOdd = pNode->BlockInfo.BlockWidth  % 2 > 0;
	bool IsHOdd = pNode->BlockInfo.BlockHeight % 2 > 0;

	int bw[4];
	int bh[4];

	for(i = 0; i < 4; i++){
		bw[i] = wDiv2;
		bh[i] = hDiv2;
	}

	if(IsWOdd){
		bw[1]++;
		bw[2]++;
	}
	if(IsHOdd){
		bh[2]++;
		bh[3]++;
	}

	// Calculate buffer offset
	int bs = pNode->BlockInfo.BlockStart;
	int bsOffsetW = wDiv2;
	int bsOffsetH = (LinePitch * hDiv2);
	
	int o[4];
	o[0] = bs;
	o[1] = bs + bsOffsetW;
	o[2] = bs + bsOffsetW + bsOffsetH;
	o[3] = bs + bsOffsetH;

	// Initialize Them
	for(i = 0; i < 4; i++)
		InitNode(pNode->Child[i], pNode, o[i], bw[i], bh[i], Level+1);
	
	// Generate more childs nodes recursively
	if(Level+1 < MaxDepth){
		for(i = 0; i < 4; i++)
			GenTree(pNode->Child[i], Level+1);
	}
}

void CQuadTree::DelTree(QuadTreeStruct *pNode, int Level)
{
	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;
	
	// Kill the sub-nodes recursively
	if(Level < MaxDepth){
		// Loop through all the child nodes
		for(int i = 3; i >= 0; i--){
			
			// Skip this node if it is already NULL
			if(pNode->Child[i] == NULL){continue;}
			
			// Delete this child node and all is sub nodes recursively...
			DelTree(pNode->Child[i], Level+1);
			
			// Set this child node pointer to NULL
			pNode->Child[i] = NULL;
		}
	}
	
	if(pNode != &QT){
		SAFE_DELETE_OBJECT(pNode);
	} else {
		InitNode(&QT, NULL, 0,0,0);
	}
}

int CQuadTree::TrimTree(QuadTreeStruct *pNode, int MaxChanged, int Level)
{
	int i = 0;
	int dwChanged = 0;
	
	// Make pNode point to the root node if Level = 0
	if(Level == 0){
		pNode = &QT;
		
		MaxChanged = 1;
		for(i = 0; i < MaxDepth; i++)
			MaxChanged *= 4;
	} else {
		MaxChanged /= 4;
	}
	
	// Analyse the sub-nodes recursively
	if(Level < MaxDepth){
		
		// Analyse a sub node
		for(i = 0; i < 4; i++){
			
			int NumChanged = TrimTree(pNode->Child[i], MaxChanged, Level+1);
			
			switch(NumChanged)
			{
			case 0:  SAFE_DELETE_OBJECT(pNode->Child[i]); break; // Delete the node if there is no change...
			default: dwChanged += NumChanged; break; // Accumulate the number of changed childs node in dwChanged...
			}
		}
		
		// If none or all of the childs nodes tested for this node have changed
		if(dwChanged == 0 || dwChanged == MaxChanged){
			
			// Change only the root node status if all the node have changed
			if(pNode->NodeType == RootNode){
				if(dwChanged == MaxChanged)
					pNode->NodeType = LeafNode;
			} else
				pNode->NodeType = LeafNode;
			
			// Delete all the nodes childs nodes if they've all changed
			if(dwChanged == MaxChanged){
				for(i = 0; i < 4; i++){
					SAFE_DELETE_OBJECT(pNode->Child[i]);
				}
			}
		}
		
	} else {

		int Offset = pNode->BlockInfo.BlockStart;

		for(i = 0; i < pNode->BlockInfo.BlockHeight; i++){
		
			if(memcmp(&pBackBuffer[Offset], &pFrontBuffer[Offset], pNode->BlockInfo.BlockWidth) != 0){
				dwChanged = 1;
				break;
			}
		
			Offset += LinePitch;
		}

	}
	
	return dwChanged;
}

int CQuadTree::ExtractTreeData(QuadTreeStruct *pNode, int OutputBufferIndx, int Level)
{
	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;
	
	// Recurse the nodes
	if(Level < MaxDepth){
		for(int i = 0; i < 4; i++){
			if(pNode->Child[i] != NULL)
				OutputBufferIndx = ExtractTreeData(pNode->Child[i], OutputBufferIndx, Level+1);
		}
	}
	
	// Save a block of memory in the buffer
	if(pNode->NodeType & LeafNode){
		
		// Find the offset in our image buffer to start with
		int InputBufferIndx = pNode->BlockInfo.BlockStart;

		// Copy the block header
		memcpy(&pUncBuffer[OutputBufferIndx], &pNode->BlockInfo, sizeof(BlockInfoStruct));
		OutputBufferIndx += sizeof(BlockInfoStruct);
		
		// Copy the data into our uncompressed buffer
		for(int i = 0; i < pNode->BlockInfo.BlockHeight; i++){	
			
			memcpy(&pUncBuffer[OutputBufferIndx], &pFrontBuffer[InputBufferIndx], pNode->BlockInfo.BlockWidth);
			
			InputBufferIndx  += LinePitch;
			OutputBufferIndx += pNode->BlockInfo.BlockWidth;
		}
	}
	
	return OutputBufferIndx;
}

int CQuadTree::CalcOutputBufferSize(QuadTreeStruct *pNode, int NodeSize, int Level)
{
	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;
	
	// Recurse the nodes
	if(Level < MaxDepth){
		for(int i = 0; i < 4; i++){
			if(pNode->Child[i] != NULL)
				NodeSize = CalcOutputBufferSize(pNode->Child[i], NodeSize, Level+1);
		}
	}
	
	// Accum. the space taken by this node
	if(pNode->NodeType & LeafNode){
		NodeSize += sizeof(BlockInfoStruct);
		NodeSize += pNode->BlockInfo.BlockWidth * pNode->BlockInfo.BlockHeight;
	}

	return NodeSize;
}
