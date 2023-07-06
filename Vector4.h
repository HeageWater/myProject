#pragma once

class Vector4
{
public:
	float x;
	float y;
	float z;
	float r;
public:
	Vector4();
	Vector4(float x, float y, float z,float r);

	float length()const;
	Vector4& normalize();
	float dot(const Vector4& v)const;
	float cross(const Vector4& v)const;

	Vector4 operator+()const;
	Vector4 operator-()const;

	//代入演算子オーバーロード
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};

const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);