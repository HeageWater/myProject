#include "Goal.h"

Goal::Goal()
{
	goal_.mat_.Initialize();
	goal_.mat_.scale_ = { 3,3,3 };
}

Goal::~Goal()
{
}

void Goal::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	goal_.Initialize(dx_, shader, "Resources\\Model\\Goal\\goal.obj", pipeline_);

	goal_.mat_.Initialize();
	goal_.mat_.scale_ = { 5,5,3 };
	goal_.mat_.trans_ = { 380,-160,0 };
}

void Goal::Draw(size_t tex)
{
	goal_.mat_.rotAngle_.y_ += 0.01f;

	goal_.Draw(tex);
}

void Goal::Update(Matrix matView, Matrix matProjection)
{
	goal_.MatUpdate(matView, matProjection);
}

bool Goal::BoxCollision(MyMath::ObjMatrix model)
{
	float a = (model.trans_.x_ - goal_.mat_.trans_.x_) * (model.trans_.x_ - goal_.mat_.trans_.x_);
	float b = (model.trans_.y_ - goal_.mat_.trans_.y_) * (model.trans_.y_ - goal_.mat_.trans_.y_);

	float c = model.scale_.x_ * goal_.mat_.scale_.x_;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;
}

void Goal::Reset()
{
	goal_.mat_.Initialize();
	goal_.mat_.scale_ = { 5,5,3 };
	goal_.mat_.trans_ = { 380,-160,0 };
}

void Goal::SetPos(Vector3D pos)
{
	goal_.mat_.trans_ = pos;
}