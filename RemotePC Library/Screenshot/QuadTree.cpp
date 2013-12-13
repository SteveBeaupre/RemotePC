#include "QuadTree.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CQuadTree::CQuadTree()
{
	pBackBuffer  = NULL;
	pFrontBuffer = NULL;
	pUncBuffer   = NULL;

	MaxDepth = 0;
	BPP = 0;
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CQuadTree::~CQuadTree()
{
	KillTree(&QT, 0);
}

//-----------------------------------------------------------------------------
// Set the screenshot buffers pointers
//-----------------------------------------------------------------------------
void CQuadTree::SetScreenshotBuffers(BYTE *BackBuffer, BYTE *FrontBuffer)
{
	pBackBuffer  = BackBuffer;
	pFrontBuffer = FrontBuffer;
}

//-----------------------------------------------------------------------------
// Set the buffers pointers
//-----------------------------------------------------------------------------
void CQuadTree::SetUncompressedBuffer(BYTE *UncBuffer)
{
	pUncBuffer = UncBuffer;
}

//-----------------------------------------------------------------------------
// Init. the root node 
//-----------------------------------------------------------------------------
void CQuadTree::InitTree(WORD w, WORD h, DWORD BytesPerPixel, DWORD dwMaxDepth)
{
	WORD l = 0;
	WORD t = 0;

	MaxDepth = dwMaxDepth;
	if(MaxDepth > 10){MaxDepth = 10;}

	BPP = BytesPerPixel;
	if(BPP < 1){BPP = 3;}

	InitNode(&QT, NULL, 0, l, t, w, h);
}

//-----------------------------------------------------------------------------
// Init. a node 
//-----------------------------------------------------------------------------
void CQuadTree::InitNode(QuadTreeStruct *pNode, QuadTreeStruct *pParentNode, DWORD Level, WORD l, WORD t, WORD w, WORD h)
{
	// Inititialise the node type
	pNode->NodeType = 0;
	// Inititialise the node
	if(Level == 0)
		pNode->NodeType = ROOT_NODE;
	else if(Level == MaxDepth)
		pNode->NodeType = LEAF_NODE;
	else
		pNode->NodeType = LINK_NODE;

	// Inititialise the dimension variables of that node
	pNode->L = l;
	pNode->T = t;
	pNode->W = w;
	pNode->H = h;

	// Setup a pointer to the parent node
	pNode->Parent = pParentNode;
	// Initialise the childs nodes to NULL 
	for(int Cpt = 0; Cpt < 4; Cpt++)
		pNode->Child[Cpt] = NULL;
}

//-----------------------------------------------------------------------------
// Generate the QuadTree recursively
//-----------------------------------------------------------------------------
void CQuadTree::GenTree(QuadTreeStruct *pNode, DWORD Level)
{
	// Return if MaxDepth = 0
	if(MaxDepth == 0){return;}

	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;

	// Allocate memory for the new nodes
	pNode->Child[0] = new QuadTreeStruct;
	pNode->Child[1] = new QuadTreeStruct;
	pNode->Child[2] = new QuadTreeStruct;
	pNode->Child[3] = new QuadTreeStruct;

	// Divide our width and heigh by 2
	WORD W = pNode->W >> 1;
	WORD H = pNode->H >> 1;
	
	WORD OffsetW = (pNode->W & 0x0001);
	WORD OffsetH = (pNode->H & 0x0001);

	// Initialize Them
	InitNode(pNode->Child[0], pNode, Level+1, pNode->L,     pNode->T,     W,           H);
	InitNode(pNode->Child[1], pNode, Level+1, pNode->L + W, pNode->T,     W + OffsetW, H);
	InitNode(pNode->Child[2], pNode, Level+1, pNode->L + W, pNode->T + H, W + OffsetW, H + OffsetH);
	InitNode(pNode->Child[3], pNode, Level+1, pNode->L,     pNode->T + H, W,           H + OffsetH);

	// Generate more childs nodes recursively
	if(Level+1 < MaxDepth){
		for(int Cpt = 0; Cpt < 4; Cpt++)
			GenTree(pNode->Child[Cpt], Level+1);
	}
}

//-----------------------------------------------------------------------------
// Delete a nodes and it's sub nodes
//-----------------------------------------------------------------------------
void CQuadTree::KillTree(QuadTreeStruct *pNode, DWORD Level)
{
	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;

	// Kill the sub-nodes recursively
	if(Level < MaxDepth){
		// Loop through all the child nodes
		for(int Cpt = 3; Cpt >= 0; Cpt--){

			// Skip this node if it is already NULL
			if(pNode->Child[Cpt] == NULL){continue;}

			// Delete this child node and all is sub nodes recursively...
			KillTree(pNode->Child[Cpt], Level+1);

			// Set this child node pointer to NULL
			pNode->Child[Cpt] = NULL;
		}
	}

	if(pNode != &QT){
		SAFE_DELETE_OBJECT(pNode);
	} else {
		InitNode(&QT, NULL, 0, 0, 0, 0, 0);
	}
}

//-----------------------------------------------------------------------------
// Analyse all the leafs node and check if the data is the same in the 2 buffers
//-----------------------------------------------------------------------------
DWORD CQuadTree::TrimTree(QuadTreeStruct *pNode, DWORD Level, DWORD MaxChanged)
{
	int Cpt = 0;
	DWORD dwChanged = 0;
	
	// Make pNode point to the root node if Level = 0
	if(Level == 0){
		pNode = &QT;

		MaxChanged = 1;
		for(DWORD Cpt = 0; Cpt < MaxDepth; Cpt++)
			MaxChanged <<= 2;
	} else {
		MaxChanged >>= 2;
	}

	// Analyse the sub-nodes recursively
	if(Level < MaxDepth){

		// Analyse a sub node
		for(Cpt = 0; Cpt < 4; Cpt++){
			DWORD NumChanged = TrimTree(pNode->Child[Cpt], Level+1, MaxChanged);
			switch(NumChanged)
			{
			case 0:  SAFE_DELETE_OBJECT(pNode->Child[Cpt]); break; // Delete the node if there is no change...
			default: dwChanged += NumChanged; break;   // Accumulate the number of changed childs node in dwChanged...
			}
		}

		//  If none or all of the childs nodes tested for this node have changed
		if(dwChanged == 0 || dwChanged == MaxChanged){

			// Change only the root node status if all the node have changed
			if(pNode->NodeType == ROOT_NODE){
				if(dwChanged == MaxChanged)
					pNode->NodeType = LEAF_NODE;
			} else 
				pNode->NodeType = LEAF_NODE;
			
			// Delete all the nodes childs nodes if they've all changed
			if(dwChanged == MaxChanged){
				for(Cpt = 0; Cpt < 4; Cpt++){
					SAFE_DELETE_OBJECT(pNode->Child[Cpt]);
				}
			}
		}

	} else {
		// Get the region info to scan
		DWORD NodeLineOffset  = pNode->L * BPP;
		DWORD NodeLineWidth   = pNode->W * BPP;
		DWORD BufferLineWidth = QT.W * BPP;
		
		// Find the offset in our image buffer to start with
		DWORD BufferIndx = (BufferLineWidth * pNode->T) + NodeLineOffset;

		// This will tell us if we've found a change
		BOOL HaveChanged = FALSE;

		// Check if the data is the same or not, lines per lines
		Cpt = 0;
		while(Cpt < pNode->H){

			HaveChanged = memcmp(&pBackBuffer[BufferIndx], &pFrontBuffer[BufferIndx], NodeLineWidth);
			
			if(HaveChanged){
				dwChanged = 1;
				break;
			}

			BufferIndx += BufferLineWidth;
			Cpt++;
		}
	}

	return dwChanged;
}

//-----------------------------------------------------------------------------
// Calculate the final size taken by our data
//-----------------------------------------------------------------------------
DWORD CQuadTree::CalcOutputBufferSize(QuadTreeStruct *pNode, DWORD Level, DWORD NodeSize)
{
	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;

	// Recurse the nodes
	if(Level < MaxDepth){
		for(int Cpt = 0; Cpt < 4; Cpt++){
			if(pNode->Child[Cpt] != NULL)
				NodeSize = CalcOutputBufferSize(pNode->Child[Cpt], Level+1, NodeSize);
		}
	}

	// Save a block of memory in the buffer
	if(pNode->NodeType & LEAF_NODE){

		DWORD NodeLineWidth = pNode->W * BPP;
		
		NodeSize += sizeof(DWORD) * 2; 
		NodeSize += NodeLineWidth * pNode->H;
	}

	return NodeSize;
}

//-----------------------------------------------------------------------------
// Extract the uncompressed data into our buffer
//-----------------------------------------------------------------------------
DWORD CQuadTree::ExtractTreeData(QuadTreeStruct *pNode, DWORD Level, DWORD UncBufferCpt)
{
	// Make pNode point to the root node if Level = 0
	if(Level == 0)
		pNode = &QT;

	// Recurse the nodes
	if(Level < MaxDepth){
		for(int Cpt = 0; Cpt < 4; Cpt++){
			if(pNode->Child[Cpt] != NULL)
				UncBufferCpt = ExtractTreeData(pNode->Child[Cpt], Level+1, UncBufferCpt);
		}
	}

	// Save a block of memory in the buffer
	if(pNode->NodeType & LEAF_NODE){

		// Get the region info to copy
		DWORD BufferLineWidth = QT.W     * BPP;
		DWORD NodeLineOffset  = pNode->L * BPP;
		DWORD NodeLineWidth   = pNode->W * BPP;

		// Find the offset in our image buffer to start with
		DWORD BufferIndx = (BufferLineWidth * pNode->T) + NodeLineOffset;
	
		// Write the origin of the data block
		memcpy(&pUncBuffer[UncBufferCpt], &BufferIndx, sizeof(DWORD));
		UncBufferCpt += sizeof(DWORD);
		
		// Write the size of the data block
		memcpy(&pUncBuffer[UncBufferCpt], &pNode->W, sizeof(WORD));
		UncBufferCpt += sizeof(WORD);
		memcpy(&pUncBuffer[UncBufferCpt], &pNode->H, sizeof(WORD));
		UncBufferCpt += sizeof(WORD);

		// Copy the data into our uncompressed buffer
		int Cpt = 0;
		while(Cpt < pNode->H){

			memcpy(&pUncBuffer[UncBufferCpt], &pFrontBuffer[BufferIndx], NodeLineWidth);
			UncBufferCpt += NodeLineWidth;

			BufferIndx += BufferLineWidth;
			Cpt++;
		}
	}

	return UncBufferCpt;
}

