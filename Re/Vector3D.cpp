#include "Vector3D.h"

#include <cmath>

Vector3D::Vector3D()
	:x_(0), y_(0), z_(0)
{
}

Vector3D::Vector3D(float x, float y, float z)
	: x_(x), y_(y), z_(z)
{
}

float Vector3D::length() const
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

Vector3D& Vector3D::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float Vector3D::dot(const Vector3D& v) const
{
	return x_ * v.x_ + y_ * v.y_ + z_ * v.z_;
}

Vector3D Vector3D::cross(const Vector3D& v) const
{
	return Vector3D(y_ * v.z_ - z_ * v.y_, z_ * v.x_ - x_ * v.z_, x_ * v.y_ - y_ * v.x_);
}

Vector3D Vector3D::operator+() const
{
	return *this;
}

Vector3D Vector3D::operator-() const
{
	return Vector3D(-x_, -y_, -z_);
}

Vector3D& Vector3D::operator+=(const Vector3D& v)
{
	x_ += v.x_;
	y_ += v.y_;
	z_ += v.z_;
	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& v)
{
	x_ -= v.x_;
	y_ -= v.y_;
	z_ -= v.z_;
	return *this;
}

Vector3D& Vector3D::operator/=(float s)
{
	x_ /= s;
	y_ /= s;
	z_ /= s;
	return *this;
}

Vector3D& Vector3D::operator*=(float s)
{
	x_ *= s;
	y_ *= s;
	z_ *= s;
	return *this;
}

Vector3D& Vector3D::ope(const float& v)
{
	x_ -= v;
	y_ -= v;
	z_ -= v;
	return *this;
}

const Vector3D operator-(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D temp(v1);
	return temp -= v2;
}

const Vector3D operator*(const Vector3D& v, float s)
{
	Vector3D temp(v);
	return temp *= s;
}

const Vector3D operator*(float s, const Vector3D& v)
{
	return v * s;
}

Vector3D CreatePolygonNormal(Vector3D a, Vector3D b, Vector3D c)
{
	Vector3D AB(b - a);
	Vector3D BC(c - b);

	Vector3D normal = AB;
	normal.cross(BC);
	normal.normalize();

	return normal;
}
