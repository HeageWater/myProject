#include "Vector4D.h"
#include <cmath>

Vector4D::Vector4D() :
	x_(0.0f), y_(0.0f), z_(0.0f), w_(0.0f)
{
}

Vector4D::Vector4D(float x, float y, float z, float w) :
	x_(x), y_(y), z_(z), w_(w)
{
}

Vector4D::Vector4D(Vector3D v3d, float w) :
	x_(v3d.x_), y_(v3d.y_), z_(v3d.z_), w_(w)
{
}

float Vector4D::length() const
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
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
	return Vector4D(-x_, -y_, -z_, -w_);
}

Vector4D& Vector4D::operator-=(const Vector4D& v)
{
	x_ -= v.x_;
	y_ -= v.y_;
	z_ -= v.z_;
	w_ -= v.w_;
	return *this;
}

Vector4D& Vector4D::operator/=(float s)
{
	x_ /= s;
	y_ /= s;
	z_ /= s;
	w_ /= s;
	return *this;
}

Vector4D& Vector4D::operator*=(const Matrix& m)
{
	Vector4D ans;
	ans.x_ = x_ * m.m_[0][0] + y_ * m.m_[1][0] + z_ * m.m_[2][0] + w_ * m.m_[3][0];
	ans.y_ = x_ * m.m_[0][1] + y_ * m.m_[1][1] + z_ * m.m_[2][1] + w_ * m.m_[3][1];
	ans.z_ = x_ * m.m_[0][2] + y_ * m.m_[1][2] + z_ * m.m_[2][2] + w_ * m.m_[3][2];
	ans.w_ = x_ * m.m_[0][3] + y_ * m.m_[1][3] + z_ * m.m_[2][3] + w_ * m.m_[3][3];
	*this = ans;
	return *this;
}

const Vector4D operator-(const Vector4D& v1, const Vector4D& v2)
{
	Vector4D temp(v1);
	return temp -= v2;
}
