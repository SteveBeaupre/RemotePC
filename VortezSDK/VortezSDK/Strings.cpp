#include "Strings.h"
 
Strings::Strings(void)
{
	size = 1;
	p = new char[size];
	if(!p)
		exit(1);

	*p = '\0';
}

Strings::Strings(char *str)
{
	size = strlen(str) + 1;
	p = new char[size];
	if(!p)
		exit(1);

	strcpy(p, str);
}

Strings::Strings(const Strings &obj)
{
	size = obj.size;
	p = new char[size];
	if(!p)
		exit(1);

	strcpy(p, obj.p);
}

Strings Strings::operator=(Strings &obj)
{
	Strings temp(obj.p);
	
	if(obj.size > size)
	{
		delete p;
		p = new char[obj.size];
		size = obj.size;
		if(!p)
			exit(1);
	}
	strcpy(p, obj.p);
	strcpy(temp.p, obj.p);
	return temp;
}

Strings Strings::operator=(char *s)
{
	int len = strlen(s) + 1;
	
	if(size < len)
	{
		delete p;
		p = new char[len];
		size = len;
		if(!p)
			exit(1);
	}
	strcpy(p, s);
	return *this;
}

Strings Strings::operator+(Strings &obj)
{
	int len;
	Strings temp;
	
	delete temp.p;
	len = strlen(obj.p) + strlen(p) + 1;
	temp.p = new char[len];
	temp.size = len;
	if(!temp.p)
		exit(1);
	strcpy(temp.p, p);
	strcat(temp.p, obj.p);
	return temp;
}

Strings Strings::operator+(char *s)
{
	int len;
	Strings temp;
	
	delete temp.p;
	len = strlen(s) + strlen(p) + 1;
	temp.p = new char[len];
	temp.size = len;
	if(!temp.p)
		exit(1);
	strcpy(temp.p, p);
	strcat(temp.p, s);
	return temp;
}

Strings operator+(char *s, Strings &obj)
{
	int len;
	Strings temp;
	
	delete temp.p;
	len = strlen(s) + strlen(obj.p) + 1;
	temp.p = new char[len];
	temp.size = len;
	if(!temp.p)
		exit(1);
	strcpy(temp.p, s);
	strcat(temp.p, obj.p);
	return temp;
}

Strings Strings::operator+=(Strings &obj)
{
	int tmplen = strlen(p) + 1;
	int len = strlen(p) + strlen(obj.p) + 1;
	
	Strings temp;
	delete temp.p;
	temp.p = new char[tmplen];
	temp.size = tmplen;
	if(!temp.p)
		exit(1);

	strcpy(temp.p, p);

	if(size < len)
	{
		delete p;
		p = new char[len];
		size = len;
		if(!p)
			exit(1);
	}
	strcpy(p, temp.p);
	strcat(p, obj.p);
	return *this;
}

Strings Strings::operator+=(char *s)
{
	int tmplen = strlen(p) + 1;
	int len = strlen(p) + strlen(s) + 1;
	
	Strings temp;
	delete temp.p;
	temp.p = new char[tmplen];
	temp.size = tmplen;
	if(!temp.p)
		exit(1);
	strcpy(temp.p, p);

	if(size < len)
	{
		delete p;
		p = new char[len];
		size = len;
		if(!p)
			exit(1);
	}
	strcpy(p, temp.p);
	strcat(p, s);
	return *this;
}

Strings Strings::operator-(Strings &substr)
{
	Strings temp(p);
	char *s1;
	int i,j;
	
	s1 = p;
	for(i=0; *s1; i++)
	{
		if(*s1!=*substr.p)
		{
			temp.p[i] = *s1;
			s1++;
		}
		else
		{
			for(j=0; substr.p[j]==s1[j] && substr.p[j]; j++)
				;
			if(!substr.p[j])
			{
				s1 += j;
				i--;
			}
			else
			{
				temp.p[i] = *s1;
				s1++;
			}
		}
	}
	temp.p[i] = '\0';
	return temp;
}

Strings Strings::operator-(char *substr)
{
	Strings temp(p);
	char *s1;
	int i,j;
	
	s1 = p;
	for(i=0; *s1; i++)
	{
		if(*s1!=*substr)
		{
			temp.p[i] = *s1;
			s1++;
		}
		else
		{
			for(j=0; substr[j]==s1[j] && substr[j]; j++)
				;
			if(!substr[j])
			{
				s1 += j;
				i--;
			}
			else
			{
				temp.p[i] = *s1;
				s1++;
			}
		}
	}
	temp.p[i] = '\0';
	return temp;
}
