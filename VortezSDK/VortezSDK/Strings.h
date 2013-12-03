#ifndef _CSTRINGS_H_
#define _CSTRINGS_H_
#ifdef __cplusplus

#include "Preproc.h"

#include <Windows.h>

#ifdef COMPILE_FOR_BORLAND
class AStrings {
public:
	virtual int   _INTERNAL_LINKAGE strsize(void) = 0;
	virtual void  _INTERNAL_LINKAGE makestr(char *s) = 0;
	virtual char* _INTERNAL_LINKAGE c_str(void) = 0;
	
	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class Strings : public AStrings
#else
class _EXP_FUNC Strings 
#endif
{
	char *p;
	int size;
public:
	Strings(char *str);
	Strings(void);
	Strings(const Strings &obj);   // Constructeur de copie.
	~Strings(void){delete [] p;}
	
	int   _INTERNAL_LINKAGE strsize(void) {return strlen(p);}   // Retourne la taille de la cha�ne.
	void  _INTERNAL_LINKAGE makestr(char *s) {strcpy(s, p);}    // Fait de l'objet Strings une cha�ne entre guillemets.
	char* _INTERNAL_LINKAGE c_str(void) {return p;}          	  // Conversion en char.

	// Operateur d'assignement
	Strings operator=(Strings &obj);  // Affectation d'un objet Strings.
	Strings operator=(char *s);       // Affectation d'une cha�ne entre guillemets.
	Strings operator+(Strings &obj);  // Concat�nation d'un objet Strings.
	Strings operator+(char *s);       // Concat�nation d'une cha�ne entre guillemets.
	Strings operator+=(Strings &obj); // Addition d'un objet Strings
	Strings operator+=(char *s);      // Addition d'une cha�ne entre guillemets.
	friend Strings operator+(char *s, Strings &obj); // Concat�nation d'une cha�ne entre guillemets et d'un objet Strings
	
	Strings operator-(Strings &obj);  // Soustraction d'un objet Strings.
	Strings operator-(char *s);       // Soubtraction d'une cha�ne entre guillemets.
	
	// Op�rateurs relationnels entre objets Strings. 
	bool operator==(Strings &obj){return strcmp(p, obj.p) == 0;}
	bool operator!=(Strings &obj){return strcmp(p, obj.p) != 0;}
	bool operator< (Strings &obj){return strcmp(p, obj.p)  < 0;}
	bool operator> (Strings &obj){return strcmp(p, obj.p)  > 0;}
	bool operator<=(Strings &obj){return strcmp(p, obj.p) <= 0;}
	bool operator>=(Strings &obj){return strcmp(p, obj.p) >= 0;}
	
	// Op�rateurs relationnels entre un objet Strings et une cha�ne entre guillemets.
	bool operator==(char *s){return strcmp(p, s) == 0;}
	bool operator!=(char *s){return strcmp(p, s) != 0;}
	bool operator< (char *s){return strcmp(p, s)  < 0;}
	bool operator> (char *s){return strcmp(p, s)  > 0;}
	bool operator<=(char *s){return strcmp(p, s) <= 0;}
	bool operator>=(char *s){return strcmp(p, s) >= 0;}

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_CSTRINGS_H_/*