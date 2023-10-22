#pragma once

/// <summary>
/// 要素数三つの物
/// </summary>
class Vector3D
{
public:
	float x_;
	float y_;
	float z_;
public:
	Vector3D();
	Vector3D(float x, float y, float z);

	float length() const;

	Vector3D& normalize();
	float dot(const Vector3D& v) const;
	Vector3D cross(const Vector3D& v) const;

	Vector3D operator+() const;
	Vector3D& ope(const float& v);

	Vector3D operator-() const;

	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);

	Vector3D& operator/=(float s);
	Vector3D& operator*=(float s);

	void SetVector3(Vector3D vec) { x_ = vec.x_; y_ = vec.y_; z_ = vec.z_; };
};

const Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
const Vector3D operator*(const Vector3D& v, float s);
const Vector3D operator*(float s, const Vector3D& v);

//頂点ABCで作られたポリゴンから法線を計算する。
Vector3D CreatePolygonNormal(Vector3D a, Vector3D b, Vector3D c);