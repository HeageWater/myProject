#pragma once

class Vector3
{
public:
	float x;
	float y;
	float z;
public:
	Vector3();
	Vector3(float x, float y, float z);

	float length()const;
	Vector3& normalize();
	float dot(const Vector3& v)const;
	//float cross(const Vector3& v)const;
	Vector3 cross(const Vector3& v)const;

	Vector3 operator+()const;
	Vector3 operator-()const;

	//������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

//���_ABC�ō��ꂽ�|���S������@�����v�Z����B
Vector3 CreatePolygonNormal(Vector3 a, Vector3 b, Vector3 c);