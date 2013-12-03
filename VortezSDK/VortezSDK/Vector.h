#ifndef _VECTOR_H_
#define _VECTOR_H_
#ifdef __cplusplus

#include <Windows.h>
#include <Math.h>

#include "Preproc.h"

#define EPSILON 0.0001f

#ifdef COMPILE_FOR_BORLAND
struct Vector {float x,y,z,w;};
#else
struct _EXP_FUNC Vector {float x,y,z,w;};
#endif

#ifdef COMPILE_FOR_BORLAND
class AVector {
public:
	virtual void _INTERNAL_LINKAGE Initialize() = 0;

	virtual void _INTERNAL_LINKAGE Set(float x, float y, float z) = 0;
	virtual void _INTERNAL_LINKAGE Set(float x, float y, float z, float w) = 0;
	virtual void _INTERNAL_LINKAGE Set(Vector vec) = 0;

	virtual float _INTERNAL_LINKAGE Magnitude() = 0;
	virtual void  _INTERNAL_LINKAGE Reverse() = 0;
	virtual void  _INTERNAL_LINKAGE Normalize() = 0;
	
	virtual void  _INTERNAL_LINKAGE Free() = 0;
};
#endif


#ifdef COMPILE_FOR_BORLAND
class CVector : public AVector
#else
class _EXP_FUNC CVector 
#endif
{
public:
	CVector();
	CVector(float x, float y, float z);
	CVector(float x, float y, float z, float w);
	CVector(Vector vec);

	Vector v;

	void _INTERNAL_LINKAGE Initialize();

	void _INTERNAL_LINKAGE Set(float x, float y, float z);
	void _INTERNAL_LINKAGE Set(float x, float y, float z, float w);
	void _INTERNAL_LINKAGE Set(Vector vec);

	float _INTERNAL_LINKAGE Magnitude();
	void  _INTERNAL_LINKAGE Reverse();
	void  _INTERNAL_LINKAGE Normalize();

	bool operator==(CVector vec);
	bool operator!=(CVector vec);

	CVector operator=(CVector vec);

	CVector operator+=(CVector vec);
	CVector operator-=(CVector vec);
	CVector operator*=(CVector vec);
	CVector operator/=(CVector vec);

	CVector operator*=(float s);
	CVector operator/=(float s);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

CVector _EXP_FUNC operator-(CVector vec);

CVector _EXP_FUNC operator+(CVector v1, CVector v2);
CVector _EXP_FUNC operator-(CVector v1, CVector v2);
CVector _EXP_FUNC operator*(CVector v1, CVector v2);
CVector _EXP_FUNC operator/(CVector v1, CVector v2);

CVector _EXP_FUNC operator*(CVector vec, float s);
CVector _EXP_FUNC operator/(CVector vec, float s);
CVector _EXP_FUNC operator*(float s, CVector vec);
CVector _EXP_FUNC operator/(float s, CVector vec);

float   _EXP_FUNC DotProduct(CVector v1, CVector v2);
CVector _EXP_FUNC CrossProduct(CVector v1, CVector v2);

float   _EXP_FUNC Magnitude(CVector vec);
CVector _EXP_FUNC Reverse(CVector vec);
CVector _EXP_FUNC Normalize(CVector vec);

#endif
#endif //_VECTOR_H_/*