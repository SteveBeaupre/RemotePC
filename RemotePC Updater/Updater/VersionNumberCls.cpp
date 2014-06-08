#include "VersionNumberCls.h"

CVersionNumber::CVersionNumber()
{
	Erase();
}

void CVersionNumber::Erase()
{
	Set(0, 0, 0);
}

void CVersionNumber::Get(int *h, int *m, int *l)
{
	*h = Hi;
	*m = Md;
	*l = Lo;
}

int CVersionNumber::GetHi()
{
	return Hi;
}

int CVersionNumber::GetMd()
{
	return Md;
}

int CVersionNumber::GetLo()
{
	return Lo;
}

void CVersionNumber::Set(int h, int m, int l)
{
	Hi = h;
	Md = m;
	Lo = l;
}

void CVersionNumber::Scan(char *s)
{
	sscanf(s, "%d.%d.%d", &Hi, &Md, &Lo);
}

int CVersionNumber::Compare(CVersionNumber *pVerNum)
{
	if(pVerNum->GetHi() > Hi){
		return -1;
	} else if(pVerNum->GetHi() < Hi){
		return 1;
	} else {
		if(pVerNum->GetMd() > Md){
			return -1;
		} else if(pVerNum->GetMd() < Md){
			return 1;
		} else {
			if(pVerNum->GetLo() > Lo){
				return -1;
			} else if(pVerNum->GetLo() < Lo){
				return 1;
			} else {
				return 0;
			}
		}
	}
}

