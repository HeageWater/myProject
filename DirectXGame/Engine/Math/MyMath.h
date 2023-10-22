#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include <cmath>

/// <summary>
/// カメラ関係の物をいろいろまとめたもの
/// </summary>
namespace MyMath
{
	struct float4
	{
		float x_;
		float y_;
		float z_;
		float w_;

		float4();
		float4(float x, float y, float z, float w);
	};

	const float PI = 3.14159265358979f;
	const float PIx2 = 6.2831853071f;

	//	視点座標、注視点座標、上方向ベクトル
	Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="winwidth"></param>
	/// <param name="winheight"></param>
	/// <param name="fovY"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	Matrix PerspectiveFovLH(const float winwidth, const float winheight, float fovY, float nearZ, float farZ);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="winwidth"></param>
	/// <param name="winheight"></param>
	/// <param name="nearZ"></param>
	/// <param name="farZ"></param>
	/// <returns></returns>
	Matrix OrthoLH(const float winwidth, const float winheight, float nearZ, float farZ);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	float ConvertToRad(float angle);

	/// <summary>
	/// 視点の判定
	/// </summary>
	/// <param name="startL"></param>
	/// <param name="endL"></param>
	/// <param name="pos"></param>
	/// <param name="rad"></param>
	/// <returns></returns>
	bool CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad);

	/// <summary>
	/// ランダム関数
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	size_t GetRandom(size_t min, size_t max);

	class MatView
	{
	public:
		Matrix mat_;
		Vector3D eye_;		//	視点座標
		Vector3D target_;	//	注視点座標
		Vector3D up_;		//	上方向ベクトル
	public:
		MatView();

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="_eye"></param>
		/// <param name="_target"></param>
		/// <param name="_up"></param>
		void Init(Vector3D _eye, Vector3D _target, Vector3D _up);

		/// <summary>
		/// 更新
		/// </summary>
		void MatUpdate();
	};

	class ObjMatrix
	{
	public:
		Matrix matWorld_;

		Matrix matScale_;
		Vector3D scale_;

		Matrix matRot_;
		Vector3D rotAngle_;

		Matrix matTrans_;
		Vector3D trans_;

	private:

		/// <summary>
		/// スケールセット
		/// </summary>
		void SetMatScaling();

		/// <summary>
		/// ローテーションセット
		/// </summary>
		void SetMatRotation();

		/// <summary>
		/// トランスフォームセット
		/// </summary>
		void SetMatTransform();
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="billboard"></param>
		void Update(Matrix billboard = Matrix());
	};
}

