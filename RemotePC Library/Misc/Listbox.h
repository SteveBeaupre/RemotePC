#ifndef _LISTBOX_H_
#define _LISTBOX_H_
#ifdef __cplusplus

#include "stdio.h"

void AddListboxMessageArg(TListBox* pListBox, const char *fmt, ...)
{
	if(fmt == NULL)
		return;

	const int BufSize = 2048;

	char TextBuf[BufSize];
	char *pTxt = &TextBuf[0];
	ZeroMemory(pTxt, BufSize);

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(pTxt, BufSize, fmt, ap);
	va_end(ap);

    pListBox->Items->Add(pTxt);
}

#endif
#endif //_LISTBOX_H_
