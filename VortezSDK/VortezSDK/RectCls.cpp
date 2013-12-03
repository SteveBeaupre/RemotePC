#include "RectCls.h"

CRect::CRect()
{
	Erase();
}

CRect::CRect(RECT *pRect)
{
	memcpy(&r, pRect, sizeof(RECT));
}

CRect::CRect(int l, int t, int _r, int b)
{
	SetRect(l,t,_r,b);
}

void CRect::Erase()
{
	ZeroMemory(&r, sizeof(RECT));
}

int CRect::Left()
{
	return r.left;
}

int CRect::Top()
{
	return r.top;
}

int CRect::Right()
{
	return r.right;
}

int CRect::Bottom()
{
	return r.top;
}

int CRect::Width()
{
	return r.right - r.left;
}

int CRect::Height()
{
	return r.bottom - r.top;
}

void CRect::SetLeft(int n)
{
	r.left = n;
}

void CRect::SetTop(int n)
{
	r.top = n;
}

void CRect::SetRight(int n)
{
	r.right = n;
}

void CRect::SetBottom(int n)
{
	r.bottom = n;
}

void CRect::SetWidth(int n)
{
	r.right = r.left + n;
}

void CRect::SetHeight(int n)
{
	r.bottom = r.top + n;
}


void CRect::Crop(int l, int t, int _r, int b)
{
	if(r.left < l){r.left = l;}
	if(r.top  < t){r.top  = t;}
	if(r.right  > _r){r.right = _r;}
	if(r.bottom > b){r.bottom = b;}
}

void CRect::Crop(int w, int h)
{
	Crop(r.left, r.right, r.left + w, r.top + h);
}

void CRect::Crop(RECT *pCropRect)
{
	Crop(pCropRect->left, pCropRect->top, pCropRect->right, pCropRect->bottom);
}

void CRect::GetRect(int *l, int *t, int *_r, int *b)
{
	*l = r.left;
	*t = r.top;
	*_r = r.right;
	*b = r.bottom;
}

RECT* CRect::GetRect()
{
	return &r;
}

void CRect::GetRect(RECT *pRect)
{
	memcpy(pRect, &r, sizeof(RECT));
}

void CRect::GetPos(int *l, int *t)
{
	*l = r.left;
	*t = r.top;
}

void CRect::GetPos(CRectPos *pRectPos)
{
	pRectPos->l = r.left;
	pRectPos->t = r.top;
}

CRectPos CRect::GetPos()
{
	CRectPos RectPos;
	RectPos.l = r.left;
	RectPos.t = r.top;
	return RectPos;
}

void CRect::GetSize(int *w, int *h)
{
	*w = this->Width();
	*h = this->Height();
}

void CRect::GetSize(CRectSize *pRectSize)
{
	pRectSize->w = this->Width();
	pRectSize->h = this->Height();
}

CRectSize CRect::GetSize()
{
	CRectSize RectSize;
	RectSize.w = this->Width();
	RectSize.h = this->Height();
	return RectSize;
}


void CRect::SetRect(int l, int t, int _r, int b)
{
	r.left = l;
	r.top  = t;
	r.right  = _r;
	r.bottom = b;
}

void CRect::SetRect(RECT *pRect)
{
	memcpy(&r, pRect, sizeof(RECT));
}

void CRect::SetPos(int l, int t)
{
	r.left = l;
	r.top  = t;
}

void CRect::SetPos(CRectPos *pRectPos)
{
	SetPos(pRectPos->l, pRectPos->t);
}

void CRect::SetSize(int w, int h)
{
	r.right  = r.left + w;
	r.bottom = r.top  + h;
}

void CRect::SetSize(CRectSize *pRectSize)
{
	SetSize(pRectSize->w, pRectSize->h);
}

void CRect::GetRectFromWindow(HWND hWnd, bool ClientArea)
{
	if(!hWnd)
		return;

	RECT tmp;
	switch(ClientArea)
	{
	case CRECT_WINDOW_AREA: GetWindowRect(hWnd, &tmp); break;
	case CRECT_CLIENT_AREA: GetClientRect(hWnd, &tmp); break;
	}
	
	this->SetRect(&tmp);
}

void CRect::CenterRects(CRect *Target, CRect *AlignRect)
{
	int w = Target->Width();
	int h = Target->Height();

	int DifWidth  = AlignRect->Width()  - Target->Width();
	int DifHeight = AlignRect->Height() - Target->Height();

	Target->SetLeft(AlignRect->Left() + (DifWidth/2));
	Target->SetTop(AlignRect->Top()  + (DifHeight/2));
	Target->SetWidth(w);
	Target->SetHeight(h);
}


