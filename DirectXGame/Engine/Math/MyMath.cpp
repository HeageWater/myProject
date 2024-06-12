#include "MyMath.h"
#include <cassert>
#include <random>

Matrix MyMath::LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	Vector3D baseX = up;
	Vector3D baseY;
	Vector3D baseZ = target - eye;

	baseZ.normalize();

	baseX = baseX.cross(baseZ);
	baseX.normalize();

	baseY = baseZ;
	baseY = baseY.cross(baseX);

	Matrix matView;

	matView.m_[0][0] = baseX.x_;
	matView.m_[1][0] = baseX.y_;
	matView.m_[2][0] = baseX.z_;

	matView.m_[0][1] = baseY.x_;
	matView.m_[1][1] = baseY.y_;
	matView.m_[2][1] = baseY.z_;

	matView.m_[0][2] = baseZ.x_;
	matView.m_[1][2] = baseZ.y_;
	matView.m_[2][2] = baseZ.z_;

	matView.m_[3][0] = -baseX.dot(eye);
	matView.m_[3][1] = -baseY.dot(eye);
	matView.m_[3][2] = -baseZ.dot(eye);

	return matView;
}

float MyMath::ConvertToRad(float angle)
{
	return angle / 180.0f * PI;
}

bool MyMath::CollisionCircleLay(Vector3D startL, Vector3D endL, Vector3D pos, float rad)
{
	Vector3D start_to_center = Vector3D(pos.x_ - startL.x_, pos.y_ - startL.y_, pos.z_ - startL.z_);
	Vector3D end_to_center = Vector3D(pos.x_ - endL.x_, pos.y_ - endL.y_, pos.z_ - endL.z_);
	Vector3D start_to_end = Vector3D(endL.x_ - startL.x_, endL.y_ - startL.y_, endL.z_ - startL.z_);
	// 単位ベクトル化する
	start_to_end.normalize();
	Vector3D dis = start_to_end.cross(start_to_center);

	float distance_projection = dis.length();

	if (fabs(distance_projection) >= rad) return false;

	// 始点 => 終点と始点 => 円の中心の内積を計算する
	float dot01 = start_to_center.dot(start_to_end);
	// 始点 => 終点と終点 => 円の中心の内積を計算する
	float dot02 = end_to_center.dot(start_to_end);

	// 二つの内積の掛け算結果が0以下なら当たり
	if (dot01 * dot02 <= 0.0f)
	{
		return true;
	}

	if (start_to_center.length() < rad ||
		end_to_center.length() < rad)
	{
		return true;
	}

	return false;
}

Matrix MyMath::PerspectiveFovLH(const float winwidth, const float winheight, float fovY, float nearZ, float farZ)
{
	assert(nearZ > 0.f && farZ > 0.f);
	//assert(!XMScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
	//assert(!XMScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
	//assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

	float aspect = (float)winwidth / winheight;

	//float tan = (float)sin(fovY/2.0f) / cos(fovY/2.0f) * aspect;
	float height = 1.0f / tanf(fovY / 2.0f);

	Matrix matProjection;
	matProjection.Identity();
	matProjection.m_[0][0] = height;
	matProjection.m_[1][1] = height * aspect;
	matProjection.m_[2][2] = (float)(farZ + nearZ) / (farZ - nearZ);
	matProjection.m_[2][3] = 1.0f;
	matProjection.m_[3][2] = -2.0f * farZ * nearZ / (float)(farZ - nearZ);
	matProjection.m_[3][3] = 0.0f;

	return matProjection;
}

Matrix MyMath::OrthoLH(const float winwidth, const float winheight, float nearZ, float farZ)
{
	Matrix matProjection;
	matProjection.Identity();
	matProjection.m_[0][0] = 2 / (float)winwidth;
	matProjection.m_[1][1] = 2 / (float)winheight;
	matProjection.m_[2][2] = 1 / (float)(farZ - nearZ);
	matProjection.m_[3][2] = nearZ / (float)(nearZ - farZ);

	return matProjection;
}

//MyMath::MatView::MatView(Vector3D _eye, Vector3D _target, Vector3D _up)
//{
//	Init(_eye, _target, _up);
//}

MyMath::MatView::MatView()
{
	Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
}

void MyMath::MatView::Init(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	eye_ = _eye;
	target_ = _target;
	up_ = _up;

	MatUpdate();
}

void MyMath::MatView::MatUpdate()
{
	////float back = 120;
	//float back = 50;

	//if (eye_.x_ < back)
	//{
	//	if (eye_.z_ > -back)
	//	{
	//		eye_.z_--;
	//	}
	//}
	//else if (eye_.x_ > 33 && eye_.x_ < 114)
	//{
	//	if (eye_.z_ > -150)
	//	{
	//		eye_.z_--;
	//	}
	//	else if(eye_.z_ < -150)
	//	{
	//		eye_.z_++;
	//	}
	//	else
	//	{
	//		eye_.z_ = -150;
	//	}
	//}
	///*else if (eye.x > 119)
	//{
	//	if (eye.z > -250)
	//	{
	//		eye.z--;
	//	}
	//}*/

	//if (eye_.x_ < 30)
	//{
	//	if (eye_.z_ > -100)
	//		eye_.z_--;
	//}

	mat_ = LookAtLH(eye_, target_, up_);
}

MyMath::float4::float4() :
	x_(0.0f), y_(0.0f), z_(0.0f), w_(0.0f)
{
}

MyMath::float4::float4(float x, float y, float z, float w) :
	x_(x), y_(y), z_(z), w_(w)
{
}

void MyMath::ObjMatrix::SetMatScaling()
{
	matScale_.Identity();
	matScale_.m_[0][0] = scale_.x_;
	matScale_.m_[1][1] = scale_.y_;
	matScale_.m_[2][2] = scale_.z_;
}

void MyMath::ObjMatrix::SetMatRotation()
{
	matRot_.Identity();
	Matrix matRotX;
	matRotX.m_[1][1] = cos(rotAngle_.x_);
	matRotX.m_[1][2] = sin(rotAngle_.x_);
	matRotX.m_[2][1] = -sin(rotAngle_.x_);
	matRotX.m_[2][2] = cos(rotAngle_.x_);
	Matrix matRotY;
	matRotY.m_[0][0] = cos(rotAngle_.y_);
	matRotY.m_[2][0] = sin(rotAngle_.y_);
	matRotY.m_[0][2] = -sin(rotAngle_.y_);
	matRotY.m_[2][2] = cos(rotAngle_.y_);
	Matrix matRotZ;
	matRotZ.m_[0][0] = cos(rotAngle_.z_);
	matRotZ.m_[0][1] = sin(rotAngle_.z_);
	matRotZ.m_[1][0] = -sin(rotAngle_.z_);
	matRotZ.m_[1][1] = cos(rotAngle_.z_);

	matRot_ = matRotZ;
	matRot_ *= matRotX;
	matRot_ *= matRotY;
}

void MyMath::ObjMatrix::SetMatTransform()
{
	matTrans_.Identity();
	matTrans_.m_[3][0] = trans_.x_;
	matTrans_.m_[3][1] = trans_.y_;
	matTrans_.m_[3][2] = trans_.z_;
}

void MyMath::ObjMatrix::Initialize()
{
	scale_ = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle_ = Vector3D(0.0f, 0.0f, 0.0f);
	trans_ = Vector3D(0.0f, 0.0f, 0.0f);
}

void MyMath::ObjMatrix::Update(Matrix billboard)
{
	matWorld_.Identity();

	matWorld_ *= billboard;

	//	スケーリング
	SetMatScaling();
	matWorld_ *= matScale_;

	//	回転
	SetMatRotation();
	matWorld_ *= matRot_;

	//	平行移動
	SetMatTransform();
	matWorld_ *= matTrans_;
}

size_t MyMath::GetRandom(size_t min, size_t max)
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<> distr((int32_t)min, (int32_t)max);
	return (size_t)distr(eng);
}