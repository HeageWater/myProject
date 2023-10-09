#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include <cmath>

namespace MyMath {
	struct float4
	{
		float x;
		float y;
		float z;
		float w;

		float4();
		float4(float x, float y, float z, float w);
	};

const float PI = 3.14159265358979f;
const float PIx2 = 6.2831853071f;

//	視点座標、注視点座標、上方向ベクトル
Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);

Matrix PerspectiveFovLH(const float winwidth, const float winheight, float fovY, float nearZ, float farZ);
Matrix OrthoLH(const float winwidth, const float winheight, float nearZ, float farZ);

float ConvertToRad(float angle);

bool CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad);

size_t GetRandom(size_t min, size_t max);

class MatView{
public:
	Matrix mat;
	Vector3D eye;		//	視点座標
	Vector3D target;	//	注視点座標
	Vector3D up;		//	上方向ベクトル
public:
	MatView();
	void Init(Vector3D _eye, Vector3D _target, Vector3D _up);
	void MatUpdate();
};

class ObjMatrix {
public:
	Matrix matWorld;

	Matrix matScale;
	Vector3D scale;

	Matrix matRot;
	Vector3D rotAngle;

	Matrix matTrans;
	Vector3D trans;

private:
	void SetMatScaling();
	void SetMatRotation();
	void SetMatTransform();
public:
	void Initialize();
	void Update(Matrix billboard = Matrix());
};
}

