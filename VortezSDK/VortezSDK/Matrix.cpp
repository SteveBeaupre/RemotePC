#include "Matrix.h"

CMatrix::CMatrix()
{
	SetIdentity();
}

CMatrix::CMatrix(Matrix *mat)
{
	m = *mat;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMatrix::SetIdentityMatrix(Matrix *m)
{
	ZeroMemory(m, sizeof(Matrix));

	for(int i = 0; i < 4; i++)
		m->Mat[i][i] = 1.0f;
}

void CMatrix::ScaleMatrix(Matrix *m, float x, float y, float z)
{
	ZeroMemory(m, sizeof(Matrix));

	m->Mat[0][0] = x;
	m->Mat[1][1] = y;
	m->Mat[2][2] = z;
	m->Mat[3][3] = 1.0f;
}

void CMatrix::TranslateMatrix(Matrix *m, float x, float y, float z)
{
	SetIdentityMatrix(m);

	m->Mat[3][0] = x;
	m->Mat[3][1] = y;
	m->Mat[3][2] = z;
}

void CMatrix::RotateMatrixOnXAxis(Matrix *m, float x)
{
	SetIdentityMatrix(m);

	m->Mat[1][2] = sin(AngleToRadian(x));
	m->Mat[2][2] = m->Mat[1][1] = cos(AngleToRadian(x));
	m->Mat[2][1] = -m->Mat[1][2];
}

void CMatrix::RotateMatrixOnYAxis(Matrix *m, float y)
{
	SetIdentityMatrix(m);

	m->Mat[2][0] = sin(AngleToRadian(y));
	m->Mat[2][2] = m->Mat[0][0] = cos(AngleToRadian(y));
	m->Mat[0][2] = -m->Mat[2][0];
}

void CMatrix::RotateMatrixOnZAxis(Matrix *m, float z)
{
	SetIdentityMatrix(m);

	m->Mat[0][1] = sin(AngleToRadian(z));
	m->Mat[1][1] = m->Mat[0][0] = cos(AngleToRadian(z));
	m->Mat[1][0] = -m->Mat[0][1];
}

void CMatrix::RotateMatrix(Matrix *m, float x, float y, float z)
{
	CMatrix xMatrix, yMatrix, zMatrix;
	RotateMatrixOnXAxis(&xMatrix.m, x);
	RotateMatrixOnYAxis(&yMatrix.m, y);
	RotateMatrixOnZAxis(&zMatrix.m, z);

	CMatrix Res = zMatrix * yMatrix * xMatrix;
	*m = Res.m;
}

Matrix CMatrix::AddMatrix(Matrix *m1, Matrix *m2)
{
	Matrix Res;
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			Res.Mat[y][x] = m1->Mat[y][x] + m2->Mat[y][x];	
		}
	}

	return Res;
}

Matrix CMatrix::SubMatrix(Matrix *m1, Matrix *m2)
{
	Matrix Res;
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			Res.Mat[y][x] = m1->Mat[y][x] - m2->Mat[y][x];	
		}
	}

	return Res;
}

Matrix CMatrix::ScalarMultMatrix(Matrix *m, float s)
{
	Matrix Res;
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			Res.Mat[y][x] = m->Mat[y][x] * s;	
		}
	}

	return Res;
}

Matrix CMatrix::ScalarDivMatrix(Matrix *m, float s)
{
	Matrix Res;
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			Res.Mat[y][x] = m->Mat[y][x] / s;	
		}
	}

	return Res;
}

Matrix CMatrix::MulMatrix(Matrix *m1, Matrix *m2)
{
	Matrix Res;
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			Res.Mat[y][x] = (m1->Mat[y][0] * m2->Mat[0][x]) + 
							(m1->Mat[y][1] * m2->Mat[1][x]) + 
							(m1->Mat[y][2] * m2->Mat[2][x]) + 
							(m1->Mat[y][3] * m2->Mat[3][x]);	
		}
	}

	return Res;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

float* CMatrix::GetMatrix()
{
	return &m.Mat[0][0];
}

void CMatrix::SetIdentity()
{
	SetIdentityMatrix(&m);
}

void CMatrix::Scale(Vector vec)
{
	ScaleMatrix(&m, vec.x, vec.y, vec.z);
}

void CMatrix::Scale(float x, float y, float z)
{
	ScaleMatrix(&m, x, y, z);
}

void CMatrix::Translate(Vector vec)
{
	TranslateMatrix(&m, vec.x, vec.y, vec.z);
}

void CMatrix::Translate(float x, float y, float z)
{
	TranslateMatrix(&m, x, y, z);
}

void CMatrix::Rotate(Vector vec)
{
	RotateMatrix(&m, vec.x, vec.y, vec.z);
}

void CMatrix::Rotate(float x, float y, float z)
{
	RotateMatrix(&m, x, y, z);
}

void CMatrix::Add(Matrix *mat)
{
	m = AddMatrix(&m, mat);
}

void CMatrix::Substract(Matrix *mat)
{
	m = SubMatrix(&m, mat);
}

void CMatrix::Multiply(Matrix *mat)
{
	m = MulMatrix(&m, mat);
}

void CMatrix::ScalarMultiply(float s)
{
	m = ScalarMultMatrix(&m, s);
}

void CMatrix::ScalarDivide(float s)
{
	m = ScalarDivMatrix(&m, s);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CMatrix::operator==(CMatrix mat)
{
	return memcmp(&m, &mat.m, sizeof(Matrix)) == 0;
}

bool CMatrix::operator!=(CMatrix mat)
{
	return memcmp(&m, &mat.m, sizeof(Matrix)) != 0;
}

CMatrix CMatrix::operator=(CMatrix mat)
{
	m = mat.m;
	return *this;
}

CMatrix CMatrix::operator+=(CMatrix mat)
{
	m = AddMatrix(&m, &mat.m);
	return *this;
}

CMatrix CMatrix::operator-=(CMatrix mat)
{
	m = SubMatrix(&m, &mat.m);
	return *this;
}

CMatrix CMatrix::operator*=(CMatrix mat)
{
	m = MulMatrix(&m, &mat.m);
	return *this;
}

CMatrix CMatrix::operator*=(float s)
{
	m = ScalarMultMatrix(&m, s);
	return *this;
}

CMatrix CMatrix::operator/=(float s)
{
	m = ScalarDivMatrix(&m, s);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMatrix operator+(CMatrix m1, CMatrix m2)
{
	CMatrix Tmp(&m1.m);
	Tmp.Add(&m2.m);
	return Tmp;
}

CMatrix operator-(CMatrix m1, CMatrix m2)
{
	CMatrix Tmp(&m1.m);
	Tmp.Substract(&m2.m);
	return Tmp;
}

CMatrix operator*(CMatrix m1, CMatrix m2)
{
	CMatrix Tmp(&m1.m);
	Tmp.Multiply(&m2.m);
	return Tmp;
}

CMatrix operator*(CMatrix m1, float s)
{
	CMatrix Tmp(&m1.m);
	Tmp.ScalarMultiply(s);
	return Tmp;
}

CMatrix operator*(float s, CMatrix m1)
{
	CMatrix Tmp(&m1.m);
	Tmp.ScalarMultiply(s);
	return Tmp;
}

CMatrix operator/(CMatrix m1, float s)
{
	CMatrix Tmp(&m1.m);
	Tmp.ScalarDivide(s);
	return Tmp;
}

CMatrix operator/(float s, CMatrix m1)
{
	CMatrix Tmp(&m1.m);
	Tmp.ScalarDivide(s);
	return Tmp;
}

CVector operator*(CMatrix m1, Vector v1)
{
	CVector Tmp(v1);
	ApplyMatrixToVector(&v1, &m1.m);
	return Tmp;
}

CVector operator*(Vector v1, CMatrix m1)
{
	CVector Tmp(v1);
	ApplyMatrixToVector(&v1, &m1.m);
	return Tmp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

float AngleToRadian(float Angle)
{
	static const float PIx2 = PI * 2.0f;
	return (Angle / 360.0f) * PIx2;
}

void ApplyMatrixToVector(Vector *v, Matrix *m)
{
	Vector Tmp = *v;
	v->x = (m->Mat[0][0] * Tmp.x) + (m->Mat[1][0] * Tmp.y) + (m->Mat[2][0] * Tmp.z) + (m->Mat[3][0] * Tmp.w);
	v->y = (m->Mat[0][1] * Tmp.x) + (m->Mat[1][1] * Tmp.y) + (m->Mat[2][1] * Tmp.z) + (m->Mat[3][1] * Tmp.w);
	v->z = (m->Mat[0][2] * Tmp.x) + (m->Mat[1][2] * Tmp.y) + (m->Mat[2][2] * Tmp.z) + (m->Mat[3][2] * Tmp.w);
	v->w = (m->Mat[0][3] * Tmp.x) + (m->Mat[1][3] * Tmp.y) + (m->Mat[2][3] * Tmp.z) + (m->Mat[3][3] * Tmp.w);
}