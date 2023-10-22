#include "Warp.h"

Warp::Warp()
{
	for (size_t i = 0; i < size_; i++)
	{
		warp_[i].mat_.Initialize();
		warp_[i].mat_.scale_ = { 3,3,3 };
	}
}

Warp::~Warp()
{
}

void Warp::Initialize(Shader shader, GPipeline* pipeline_)
{
	for (size_t i = 0; i < size_; i++)
	{
		warp_[i].Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\kyu\\kyu.obj", pipeline_);

		warp_[i].mat_.Initialize();
		warp_[i].mat_.scale_ = { 5,5,5 };
		warp_[0].mat_.trans_ = { 280,-160,0 };
		warp_[1].mat_.trans_ = { 350,-160,0 };
	}
}

void Warp::Draw(size_t tex)
{
	for (size_t i = 0; i < size_; i++)
	{
		//warp_.mat.rotAngle.y += 0.01f;

		warp_[i].Draw(tex);
	}
}

void Warp::Update(Matrix matView, Matrix matProjection)
{
	float spd = 0.01f;

	for (size_t i = 0; i < size_; i++)
	{
		warp_[i].mat_.rotAngle_.y_ += spd;
		warp_[i].MatUpdate(matView, matProjection);
	}
}

bool Warp::BoxCollision(Model model)
{
	float a = (model.mat_.trans_.x_ - warp_[0].mat_.trans_.x_) * (model.mat_.trans_.x_ - warp_[0].mat_.trans_.x_);
	float b = (model.mat_.trans_.y_ - warp_[0].mat_.trans_.y_) * (model.mat_.trans_.y_ - warp_[0].mat_.trans_.y_);

	float c = model.mat_.scale_.x_ * warp_[0].mat_.scale_.x_;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;
}

void Warp::CheckMode(uint32_t mode)
{
	float spd = 0.3f;

	if (mode == 2)
	{
		warp_[0].mat_.scale_ -= {spd, spd, spd};
		if (warp_[0].mat_.scale_.x_ < 0)
		{
			warp_[0].mat_.scale_ = { 0,0,0 };
		}
	}
	else if (mode == 3)
	{
		warp_[1].mat_.scale_ -= {spd, spd, spd};
		if (warp_[1].mat_.scale_.x_ < 0)
		{
			warp_[1].mat_.scale_ = { 0,0,0 };
		}
	}
}

void Warp::Reset()
{
	for (size_t i = 0; i < size_; i++)
	{
		warp_[i].mat_.Initialize();
		warp_[i].mat_.scale_ = { 5,5,3 };
		warp_[i].mat_.trans_ = { 1000,15,0 };
	}
}