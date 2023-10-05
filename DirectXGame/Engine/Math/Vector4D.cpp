#include "Vector4D.h"
#include <cmath>

Vector4D::Vector4D() :
	x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

Vector4D::Vector4D(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{
}

Vector4D::Vector4D(Vector3D v3d, float w) :
	x(v3d.x), y(v3d.y), z(v3d.z), w(w)
{
}

float Vector4D::length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

Vector4D& Vector4D::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

Vector4D Vector4D::operator+() const
{
	return *this;
}

Vector4D Vector4D::operator-() const
{
	return Vector4D(-x, -y, -z, -w);
}

Vector4D& Vector4D::operator-=(const Vector4D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4D& Vector4D::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

Vector4D& Vector4D::operator*=(const Matrix& m)
{
	Vector4D ans;
	ans.x = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0];
	ans.y = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1];
	ans.z = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2];
	ans.w = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3];
	*this = ans;
	return *this;
}

const Vector4D operator-(const Vector4D& v1, const Vector4D& v2)
{
	Vector4D temp(v1);
	return temp -= v2;
}
