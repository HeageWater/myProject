#pragma once
#include "MyMath.h"
#include "Model.h"
#include <DirectXMath.h>

/// <summary>
/// 一定の形の当たり判定
/// </summary>

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	Vector3D center_ = { 0, 0, 0 };
	float radius_ = 0;
};

/// <summary>
/// 平面
/// </summary>
struct Plane
{
	Vector3D normal_ = { 0,1,0 };

	float distance_ = 0.0f;
};

/// <summary>
/// 三角形
/// </summary>
struct Triangle
{
	Vector3D p0_ = { 0,0,0 };
	Vector3D p1_ = { 0,0,0 };
	Vector3D p2_ = { 0,0,0 };

	Vector3D normal_ = { 0,0,0 };
};

/// <summary>
/// 当たり判定クラス
/// </summary>
class Collision
{
public:

	/// <summary>
	/// 球と平面
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="plane"></param>
	/// <param name="inter"></param>
	/// <returns></returns>
	static bool CheckSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3D* inter = { 0 });
	
	/// <summary>
	/// 球と三角形
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="triangle"></param>
	/// <param name="inter"></param>
	/// <returns></returns>
	static bool CheckSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3D* inter = { 0 });
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="point"></param>
	/// <param name="triangle"></param>
	/// <param name="closest"></param>
	static void ClosestPtPoint2Triangle(const Vector3D& point, const Triangle& triangle, Vector3D* closest);
	
	/// <summary>
	/// 立方体同士の判定
	/// </summary>
	/// <param name="model1"></param>
	/// <param name="model2"></param>
	/// <returns></returns>
	static bool BoxCollision3D(Model model1, Model model2);
	
	/// <summary>
	/// 平面同士の判定
	/// </summary>
	/// <param name="model1"></param>
	/// <param name="model2"></param>
	/// <returns></returns>
	static bool BoxCollision2D(Model model1, Model model2);

	//シングルトン
	static Collision* Get()
	{
		static Collision collision;
		return &collision;
	}
};
