#pragma once
#include "Re/Vector2D.h"
#include "Re/Vector3D.h"
#include "Re/Vector4D.h"

#include <DirectXMath.h>

struct Sphere
{
	Vector3D center_ = { 0, 0, 0 };
	float radius_ = 0;
};

struct Plane
{
	Vector3D normal_ = { 0,1,0 };

	float distance_ = 0.0f;
};

struct Triangle
{
	Vector3D p0_ = { 0,0,0 };
	Vector3D p1_ = { 0,0,0 };
	Vector3D p2_ = { 0,0,0 };

	Vector3D normal_ = { 0,0,0 };
};

class Collision
{
public:

	static bool CheckSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3D* inter = { 0 });
	static bool CheckSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3D* inter = { 0 });
	static void ClosestPtPoint2Triangle(const Vector3D& point, const Triangle& triangle, Vector3D* closest);

};
