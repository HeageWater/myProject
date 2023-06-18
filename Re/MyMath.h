#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include <cmath>

namespace MyMath {
	struct float4
	{
		float x_;
		float y_;
		float z_;
		float w_;

		float4();
		float4(float x, float y, float z, float w);
	};

const float PI_ = 3.14159265358979f;
const float PIx2_ = 6.2831853071f;

//	���_���W�A�����_���W�A������x�N�g��
Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);

Matrix PerspectiveFovLH(const size_t winwidth, const size_t winheight, float fovY, float nearZ, float farZ);
Matrix OrthoLH(const size_t winwidth, const size_t winheight, float nearZ, float farZ);

float ConvertToRad(float angle);

bool CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad);

class MatView{
public:
	Matrix mat_;
	Vector3D eye_;		//	���_���W
	Vector3D target_;	//	�����_���W
	Vector3D up_;		//	������x�N�g��
public:
	MatView();
	void Init(Vector3D _eye, Vector3D _target, Vector3D _up);
	void MatUpdate();
};

class ObjMatrix {
public:
	Matrix matWorld_;

	Matrix matScale_;
	Vector3D scale_;

	Matrix matRot_;
	Vector3D rotAngle_;

	Matrix matTrans_;
	Vector3D trans_;

private:
	void SetMatScaling();
	void SetMatRotation();
	void SetMatTransform();
public:
	void Initialize();
	void Update(Matrix billboard = Matrix());
};
}

