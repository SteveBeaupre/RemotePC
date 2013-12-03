#include "Vector.h"

CVector::CVector()
{
	Initialize();
}

CVector::CVector(float x, float y, float z)
{
	Set(x,y,z, 0.0f);
}

CVector::CVector(float x, float y, float z, float w)
{
	Set(x,y,z,w);
}

CVector::CVector(Vector vec)
{
	Set(vec);
}

void CVector::Initialize()
{
	ZeroMemory(&v, sizeof(Vector));
}

void CVector::Set(float x, float y, float z)
{
	v.x = x;
	v.y = y;
	v.z = z;
}

void CVector::Set(float x, float y, float z, float w)
{
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
}

void CVector::Set(Vector vec)
{
	v = vec;
}

float CVector::Magnitude()
{
	return sqrt((v.x*v.x)+(v.y*v.y)+(v.z*v.z));
}

void CVector::Reverse()
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
}

void CVector::Normalize()
{
	float m = Magnitude();
	if(m <= EPSILON){m = 1.0f;}

	CVector Mag;
	v.x /= m;
	v.y /= m;
	v.z /= m;

	if(fabs(v.x) < EPSILON){v.x = 0.0f;}
	if(fabs(v.y) < EPSILON){v.y = 0.0f;}
	if(fabs(v.z) < EPSILON){v.z = 0.0f;}
}

CVector CVector::operator=(CVector vec)
{
	Set(vec.v);
	return *this;
}

bool CVector::operator==(CVector vec)
{
	return memcmp(&v, &vec.v, sizeof(Vector)) == 0;
}

bool CVector::operator!=(CVector vec)
{
	return memcmp(&v, &vec.v, sizeof(Vector)) != 0;
}

CVector CVector::operator+=(CVector vec)
{
	v.x += vec.v.x;
	v.y += vec.v.y;
	v.z += vec.v.z;
	return *this;
}

CVector CVector::operator-=(CVector vec)
{
	v.x -= vec.v.x;
	v.y -= vec.v.y;
	v.z -= vec.v.z;
	return *this;
}

CVector CVector::operator*=(CVector vec)
{
	v.x *= vec.v.x;
	v.y *= vec.v.y;
	v.z *= vec.v.z;
	return *this;
}

CVector CVector::operator/=(CVector vec)
{
	v.x /= vec.v.x;
	v.y /= vec.v.y;
	v.z /= vec.v.z;
	return *this;
}

CVector CVector::operator*=(float s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return *this;
}

CVector CVector::operator/=(float s)
{
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return *this;
}

CVector operator-(CVector vec)
{
	CVector Tmp(-vec.v.x, -vec.v.y, -vec.v.z);
	return Tmp;
}

CVector operator+(CVector v1, CVector v2)
{
	CVector Tmp(v1.v.x+v2.v.x, v1.v.y+v2.v.y, v1.v.z+v2.v.z);
	return Tmp;
}

CVector operator-(CVector v1, CVector v2)
{
	CVector Tmp(v1.v.x-v2.v.x, v1.v.y-v2.v.y, v1.v.z-v2.v.z);
	return Tmp;
}

CVector operator*(CVector v1, CVector v2)
{
	CVector Tmp(v1.v.x*v2.v.x, v1.v.y*v2.v.y, v1.v.z*v2.v.z);
	return Tmp;
}

CVector operator/(CVector v1, CVector v2)
{
	CVector Tmp(v1.v.x/v2.v.x, v1.v.y/v2.v.y, v1.v.z/v2.v.z);
	return Tmp;
}

CVector operator*(CVector vec, float s)
{
	CVector Tmp(vec.v.x*s, vec.v.y*s, vec.v.z*s);
	return Tmp;
}

CVector operator/(CVector vec, float s)
{
	CVector Tmp(vec.v.x/s, vec.v.y/s, vec.v.z/s);
	return Tmp;
}

CVector operator*(float s, CVector vec)
{
	CVector Tmp(vec.v.x*s, vec.v.y*s, vec.v.z*s);
	return Tmp;
}

CVector operator/(float s, CVector vec)
{
	CVector Tmp(vec.v.x/s, vec.v.y/s, vec.v.z/s);
	return Tmp;
}

float DotProduct(CVector v1, CVector v2)
{
	return ((v1.v.x*v2.v.x) + (v1.v.y*v2.v.y) + (v1.v.z*v2.v.z));
}

CVector CrossProduct(CVector v1, CVector v2)
{
	CVector Tmp(((v1.v.y*v2.v.z)-(v1.v.z*v2.v.y)), ((v1.v.x*v2.v.z)+(v1.v.z*v2.v.x)), ((v1.v.x*v2.v.y)-(v1.v.y*v2.v.x)));
	return Tmp;
}

float Magnitude(CVector vec)
{
	CVector Tmp(vec.v);
	return Tmp.Magnitude();
}

CVector Reverse(CVector vec)
{
	CVector Tmp(vec.v);
	Tmp.Reverse();
	return Tmp;
}

CVector Normalize(CVector vec)
{
	CVector Tmp(vec.v);
	Tmp.Normalize();
	return Tmp;
}

