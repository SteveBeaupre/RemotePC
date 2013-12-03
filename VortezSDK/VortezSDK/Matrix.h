#ifndef _MATRIX_H_
#define _MATRIX_H_
#ifdef __cplusplus

#include "Vector.h"

#define PI  3.141592653589793f

#ifdef COMPILE_FOR_BORLAND
typedef struct {float Mat[4][4];} Matrix;
#else 
typedef struct _EXP_FUNC {float Mat[4][4];} Matrix;
#endif

float _EXP_FUNC AngleToRadian(float Angle);
void  _EXP_FUNC ApplyMatrixToVector(Vector *v, Matrix *m);


#ifdef COMPILE_FOR_BORLAND
class AMatrix {
public:
	virtual float* _INTERNAL_LINKAGE GetMatrix() = 0;
	virtual void   _INTERNAL_LINKAGE SetIdentity() = 0;

	virtual void _INTERNAL_LINKAGE Scale(Vector vec) = 0;
	virtual void _INTERNAL_LINKAGE Scale(float x, float y, float z) = 0;
	virtual void _INTERNAL_LINKAGE Translate(Vector vec) = 0;
	virtual void _INTERNAL_LINKAGE Translate(float x, float y, float z) = 0;
	virtual void _INTERNAL_LINKAGE Rotate(Vector vec) = 0;
	virtual void _INTERNAL_LINKAGE Rotate(float x, float y, float z) = 0;

	virtual void _INTERNAL_LINKAGE Add(Matrix *mat) = 0;
	virtual void _INTERNAL_LINKAGE Substract(Matrix *mat) = 0;
	virtual void _INTERNAL_LINKAGE Multiply(Matrix *mat) = 0;
	virtual void _INTERNAL_LINKAGE ScalarMultiply(float s) = 0;
	virtual void _INTERNAL_LINKAGE ScalarDivide(float s) = 0;
	
	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif


#ifdef COMPILE_FOR_BORLAND
class CMatrix : public AMatrix
#else
class _EXP_FUNC CMatrix 
#endif
{
public:
	CMatrix();
	CMatrix(Matrix *mat);
private:
	void SetIdentityMatrix(Matrix *m);
	void ScaleMatrix(Matrix *m, float x, float y, float z);
	void TranslateMatrix(Matrix *m, float x, float y, float z);
	void RotateMatrixOnXAxis(Matrix *m, float x);
	void RotateMatrixOnYAxis(Matrix *m, float y);
	void RotateMatrixOnZAxis(Matrix *m, float z);
	void RotateMatrix(Matrix *m, float x, float y, float z);

	Matrix AddMatrix(Matrix *m1, Matrix *m2);
	Matrix SubMatrix(Matrix *m1, Matrix *m2);
	Matrix MulMatrix(Matrix *m1, Matrix *m2);
	Matrix ScalarMultMatrix(Matrix *m, float s);
	Matrix ScalarDivMatrix(Matrix *m, float s);
public:
	Matrix m;

	float* _INTERNAL_LINKAGE GetMatrix();

	void _INTERNAL_LINKAGE SetIdentity();

	void _INTERNAL_LINKAGE Scale(Vector vec);
	void _INTERNAL_LINKAGE Scale(float x, float y, float z);
	void _INTERNAL_LINKAGE Translate(Vector vec);
	void _INTERNAL_LINKAGE Translate(float x, float y, float z);
	void _INTERNAL_LINKAGE Rotate(Vector vec);
	void _INTERNAL_LINKAGE Rotate(float x, float y, float z);

	void _INTERNAL_LINKAGE Add(Matrix *mat);
	void _INTERNAL_LINKAGE Substract(Matrix *mat);
	void _INTERNAL_LINKAGE Multiply(Matrix *mat);
	void _INTERNAL_LINKAGE ScalarMultiply(float s);
	void _INTERNAL_LINKAGE ScalarDivide(float s);

	bool operator==(CMatrix mat);
	bool operator!=(CMatrix mat);

	CMatrix operator=(CMatrix mat);

	CMatrix operator+=(CMatrix mat);
	CMatrix operator-=(CMatrix mat);
	CMatrix operator*=(CMatrix mat);
	
	CMatrix operator*=(float s);
	CMatrix operator/=(float s);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

CMatrix _EXP_FUNC operator+(CMatrix m1, CMatrix m2);
CMatrix _EXP_FUNC operator-(CMatrix m1, CMatrix m2);
CMatrix _EXP_FUNC operator*(CMatrix m1, CMatrix m2);

CMatrix _EXP_FUNC operator*(CMatrix m1, float s);
CMatrix _EXP_FUNC operator*(float s, CMatrix m1);
CMatrix _EXP_FUNC operator/(CMatrix m1, float s);
CMatrix _EXP_FUNC operator/(float s, CMatrix m1);

CVector _EXP_FUNC operator*(CMatrix m1, Vector v1);
CVector _EXP_FUNC operator*(Vector v1, CMatrix m1);

#endif
#endif //_MATRIX_H_/*