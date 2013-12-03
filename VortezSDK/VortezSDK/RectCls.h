#ifndef _C_RECT_H_
#define _C_RECT_H_
#ifdef __cplusplus

#include <Windows.h>
#include <stdio.h>

struct CRectSize {
	int w,h;
};

struct CRectPos {
	int l,t;
};

#define CRECT_WINDOW_AREA   false
#define CRECT_CLIENT_AREA    true

class CRect {
public:
	CRect();
	CRect(RECT *pRect);
	CRect(int l, int t, int _r, int b);
private:
	RECT r;
public:
	void Erase();

	int Left();
	int Top();
	int Right();
	int Bottom();
	int Width();
	int Height();

	void SetLeft(int n);
	void SetTop(int n);
	void SetRight(int n);
	void SetBottom(int n);
	void SetWidth(int n);
	void SetHeight(int n);

	void Crop(int l, int t, int _r, int b);
	void Crop(int w, int h);
	void Crop(RECT *pCropRect);

	void GetRect(int *l, int *t, int *_r, int *b);
	RECT* GetRect();
	void GetRect(RECT *pRect);
	
	void GetPos(int *l, int *t);
	void GetPos(CRectPos *pRectPos);
	CRectPos GetPos();

	void GetSize(int *w, int *h);
	void GetSize(CRectSize *pRectSize);
	CRectSize GetSize();


	void SetRect(int l, int t, int _r, int b);
	void SetRect(RECT *pRect);

	void SetPos(int l, int t);
	void SetPos(CRectPos *pRectPos);

	void SetSize(int w, int h);
	void SetSize(CRectSize *pRectSize);


	void GetRectFromWindow(HWND hWnd, bool ClientArea = CRECT_WINDOW_AREA);
	void CenterRects(CRect *Target, CRect *AlignRect);
};


#endif
#endif //_C_RECT_H_
