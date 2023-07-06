#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include <cmath>

namespace MyMath
{
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
	//const float PIx2 =6.2831853071f;
	// 
	//	視点座標、注視点座標、上方向ベクトル
	Matrix LookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);

	Matrix PerspectiveFovLH(const int winwidth, const int winheight, float fovY, float nearZ, float farZ);
	Matrix OrthoLH(const int winwidth, const int winheight, float nearZ, float farZ);

	float ConvertToRad(float angle);

	bool CollisionCircleLay(Vector3 startL, Vector3 endL, Vector3 pos, float rad);

	class MatView {
	public:
		Matrix mat;
		Vector3 eye;		//	視点座標
		Vector3 target;	//	注視点座標
		Vector3 up;		//	上方向ベクトル
	public:
		MatView();
		void Init(Vector3 _eye, Vector3 _target, Vector3 _up);
		void MatUpdate();
	};

	class ObjMatrix {
	public:
		Matrix matWorld;

		Matrix matScale;
		Vector3 scale;

		Matrix matRot;
		Vector3 rotAngle;

		Matrix matTrans;
		Vector3 trans;

	private:
		void SetMatScaling();
		void SetMatRotation();
		void SetMatTransform();
	public:
		void Initialize();
		void Update(Matrix billboard = Matrix());
	};
};