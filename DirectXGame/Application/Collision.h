#pragma once
#include "MyMath.h"
#include "Model.h"
#include <DirectXMath.h>

struct Sphere
{
	Vector3D center = { 0, 0, 0 };
	float radius = 0;
};

struct Plane
{
	Vector3D normal = { 0,1,0 };

	float distance = 0.0f;
};

struct Triangle
{
	Vector3D p0 = { 0,0,0 };
	Vector3D p1 = { 0,0,0 };
	Vector3D p2 = { 0,0,0 };

	Vector3D normal = { 0,0,0 };
};

class Collision
{
public:

	static bool CheckSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3D* inter = { 0 });
	static bool CheckSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3D* inter = { 0 });
	static void ClosestPtPoint2Triangle(const Vector3D& point, const Triangle& triangle, Vector3D* closest);
	static bool BoxCollision3D(Model model1, Model model2);
	static bool BoxCollision2D(Model model1, Model model2);

	static Collision* Get()
	{
		static Collision collision;
		return &collision;
	}

};
