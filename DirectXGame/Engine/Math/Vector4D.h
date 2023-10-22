#pragma once
#include "Vector3D.h"
#include "Matrix.h"

/// <summary>
/// 要素数四つの物
/// </summary>
class Vector4D
{
public:
	float x_;
	float y_;
	float z_;
	float w_;
public:
	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(Vector3D v3d, float w);

	float length() const;
	Vector4D& normalize();

	Vector4D operator+() const;

	Vector4D operator-() const;

	Vector4D& operator-=(const Vector4D& v);

	Vector4D& operator/=(float s);

	Vector4D& operator*=(const Matrix& m);
};

const Vector4D operator-(const Vector4D& v1, const Vector4D& v2);