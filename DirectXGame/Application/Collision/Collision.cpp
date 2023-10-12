#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3D* inter)
{
	float distV = sphere.center.dot(plane.normal);

	float dist = distV - plane.distance;

	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}

	Vector3D intA;

	if (inter)
	{
		intA.x = -dist * plane.normal.x + sphere.center.x;
		intA.y = -dist * plane.normal.y + sphere.center.y;
		intA.z = -dist * plane.normal.z + sphere.center.z;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const Vector3D& point, const Triangle& triangle, Vector3D* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	Vector3D p0_p1 = triangle.p1 - triangle.p0;
	Vector3D p0_p2 = triangle.p2 - triangle.p0;
	Vector3D p0_pt = point - triangle.p0;

	Triangle returnP = triangle;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	Vector3D p1_pt = point - triangle.p1;

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p1.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);

		returnP.p0.x += v * p0_p1.x;
		returnP.p0.y += v * p0_p1.y;
		returnP.p0.z += v * p0_p1.z;
		*closest = returnP.p0;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3D p2_pt = point - triangle.p2;

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);

	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		returnP.p0.x += w * p0_p2.x;
		returnP.p0.y += w * p0_p2.y;
		returnP.p0.z += w * p0_p2.z;
		*closest = returnP.p0;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

		returnP.p1.x += w * (triangle.p2.x - triangle.p1.x);
		returnP.p1.y += w * (triangle.p2.y - triangle.p1.y);
		returnP.p1.z += w * (triangle.p2.z - triangle.p1.z);

		*closest = triangle.p1;
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	returnP.p0.x = returnP.p0.x + p0_p1.x * v + w * p0_p2.x;
	returnP.p0.y = returnP.p0.y + p0_p1.y * v + w * p0_p2.y;
	returnP.p0.z = returnP.p0.z + p0_p1.z * v + w * p0_p2.z;

	*closest = returnP.p0;
}

bool Collision::BoxCollision3D(Model model1, Model model2)
{
	float DisX = model1.mat.trans.x - model2.mat.trans.x;
	float DisY = model1.mat.trans.y - model2.mat.trans.y;
	float DisZ = model1.mat.trans.z - model2.mat.trans.z;

	DisX = abs(DisX);
	DisY = abs(DisY);
	DisZ = abs(DisZ);

	if (DisX <= model1.mat.scale.x + model2.mat.scale.x &&
		DisY <= model1.mat.scale.y + model2.mat.scale.y &&
		DisY <= model1.mat.scale.z + model2.mat.scale.z)
	{
		return true;
	}

	return false;
}

bool Collision::BoxCollision2D(Model model1, Model model2)
{
	float DisX = model1.mat.trans.x - model2.mat.trans.x;
	float DisY = model1.mat.trans.y - model2.mat.trans.y;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= model1.mat.scale.x + model2.mat.scale.x &&
		DisY <= model1.mat.scale.y + model2.mat.scale.y)
	{
		return true;
	}

	return false;
}

bool Collision::CheckSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3D* inter)
{
	Vector3D p;
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	Vector3D v = p - sphere.center;
	float a = v.dot(v);

	if (a > sphere.radius * sphere.radius)
	{
		return false;
	}

	if (inter)
	{
		*inter = p;
	}

	return true;
}
