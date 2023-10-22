#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphereToPlane(const Sphere& sphere, const Plane& plane, Vector3D* inter)
{
	float distV = sphere.center_.dot(plane.normal_);

	float dist = distV - plane.distance_;

	if (fabsf(dist) > sphere.radius_)
	{
		return false;
	}

	Vector3D intA;

	if (inter)
	{
		intA.x_ = -dist * plane.normal_.x_ + sphere.center_.x_;
		intA.y_ = -dist * plane.normal_.y_ + sphere.center_.y_;
		intA.z_ = -dist * plane.normal_.z_ + sphere.center_.z_;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const Vector3D& point, const Triangle& triangle, Vector3D* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	Vector3D p0_p1 = triangle.p1_ - triangle.p0_;
	Vector3D p0_p2 = triangle.p2_ - triangle.p0_;
	Vector3D p0_pt = point - triangle.p0_;

	Triangle returnP = triangle;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0_;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	Vector3D p1_pt = point - triangle.p1_;

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p1.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.p1_;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);

		returnP.p0_.x_ += v * p0_p1.x_;
		returnP.p0_.y_ += v * p0_p1.y_;
		returnP.p0_.z_ += v * p0_p1.z_;
		*closest = returnP.p0_;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3D p2_pt = point - triangle.p2_;

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);

	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2_;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		returnP.p0_.x_ += w * p0_p2.x_;
		returnP.p0_.y_ += w * p0_p2.y_;
		returnP.p0_.z_ += w * p0_p2.z_;
		*closest = returnP.p0_;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

		returnP.p1_.x_ += w * (triangle.p2_.x_ - triangle.p1_.x_);
		returnP.p1_.y_ += w * (triangle.p2_.y_ - triangle.p1_.y_);
		returnP.p1_.z_ += w * (triangle.p2_.z_ - triangle.p1_.z_);

		*closest = triangle.p1_;
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	returnP.p0_.x_ = returnP.p0_.x_ + p0_p1.x_ * v + w * p0_p2.x_;
	returnP.p0_.y_ = returnP.p0_.y_ + p0_p1.y_ * v + w * p0_p2.y_;
	returnP.p0_.z_ = returnP.p0_.z_ + p0_p1.z_ * v + w * p0_p2.z_;

	*closest = returnP.p0_;
}

bool Collision::BoxCollision3D(Model model1, Model model2)
{
	float DisX = model1.mat_.trans_.x_ - model2.mat_.trans_.x_;
	float DisY = model1.mat_.trans_.y_ - model2.mat_.trans_.y_;
	float DisZ = model1.mat_.trans_.z_ - model2.mat_.trans_.z_;

	DisX = abs(DisX);
	DisY = abs(DisY);
	DisZ = abs(DisZ);

	if (DisX <= model1.mat_.scale_.x_ + model2.mat_.scale_.x_ &&
		DisY <= model1.mat_.scale_.y_ + model2.mat_.scale_.y_ &&
		DisY <= model1.mat_.scale_.z_ + model2.mat_.scale_.z_)
	{
		return true;
	}

	return false;
}

bool Collision::BoxCollision2D(Model model1, Model model2)
{
	float DisX = model1.mat_.trans_.x_ - model2.mat_.trans_.x_;
	float DisY = model1.mat_.trans_.y_ - model2.mat_.trans_.y_;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= model1.mat_.scale_.x_ + model2.mat_.scale_.x_ &&
		DisY <= model1.mat_.scale_.y_ + model2.mat_.scale_.y_)
	{
		return true;
	}

	return false;
}

bool Collision::CheckSphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3D* inter)
{
	Vector3D p;
	ClosestPtPoint2Triangle(sphere.center_, triangle, &p);

	Vector3D v = p - sphere.center_;
	float a = v.dot(v);

	if (a > sphere.radius_ * sphere.radius_)
	{
		return false;
	}

	if (inter)
	{
		*inter = p;
	}

	return true;
}
