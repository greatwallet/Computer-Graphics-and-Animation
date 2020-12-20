#include "transform.h"
#include <math.h>
#include "cfg.h"

float deg2rad(float deg)
{
	return deg * PI / 180.0;
}
Matrix3f translate(float tx, float ty)
{
	Matrix3f T;
	T << 1, 0, tx,
		0, 1, ty,
		0, 0, 1;
	return T;
}

// Generate rotation matrix
Matrix3f rotate(float deg)
{
	float rad = deg2rad(deg);
	Matrix3f R;
	R << cos(rad), -sin(rad), 0,
		sin(rad), cos(rad), 0,
		0, 0, 1;
	return R;
}
// Generate scale matrix
Matrix3f scale(float sx, float sy)
{
	Matrix3f S;
	S << sx, 0, 0,
		0, sy, 0,
		0, 0, 1;
	return S;
}

Vector3f genVec(int x, int y)
{
	return Vector3f(float(x),float(y),1);
}