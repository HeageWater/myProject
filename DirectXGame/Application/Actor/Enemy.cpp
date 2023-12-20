#include "Enemy.h"
#include "Enum.h"

Enemy::Enemy()
{
	enemy_.mat_.Initialize();
	enemy_.mat_.scale_ = { 3,3,3 };
}

Enemy::~Enemy()
{

}

void Enemy::Initialize(Shader shader, GPipeline* pipeline_)
{
	enemy_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\ene\\ene.obj", pipeline_);

	enemy_.mat_.Initialize();
	enemy_.mat_.scale_ = { 1,1,1 };
	isDead_ = false;
	deadVec_ = false;
	Vec_ = { 0,0,0 };
}

void Enemy::Draw(size_t tex)
{
	if (isDead_ == false)
	{
		enemy_.Draw(tex);
	}
}

void Enemy::Update(Matrix matView, Matrix matProjection)
{
	float spd = 0.005f;

	if (deadVec_)
	{
		spd = 0.7f;
	}

	enemy_.mat_.rotAngle_.y_ += spd;

	if (Time_ > 0)
	{
		Time_--;

		move_ = { 0,0,0 };
	}

	enemy_.mat_.trans_ -= move_;

	DeadVec();

	enemy_.MatUpdate(matView, matProjection);
}

bool Enemy::BoxCollision(Model model)
{
	if (deadVec_ == false)
	{
		float a = (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_) * (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_);
		float b = (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_) * (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_);

		float c = model.mat_.scale_.x_ + enemy_.mat_.scale_.x_ * 100;

		//あたり判定
		if (a + b < c * (float)TWO)
		{
			float spd = TWO;
			Vec_ = model.mat_.trans_ - enemy_.mat_.trans_;
			Vec_ *= spd;
			deadVec_ = true;

			return true;
		}

		a = (-model.mat_.trans_.x_ - enemy_.mat_.trans_.x_) * (-model.mat_.trans_.x_ - enemy_.mat_.trans_.x_);
		b = (-model.mat_.trans_.y_ - enemy_.mat_.trans_.y_) * (-model.mat_.trans_.y_ - enemy_.mat_.trans_.y_);

		c = model.mat_.scale_.x_ + enemy_.mat_.scale_.x_;

		//あたり判定
		if (a + b < c)
		{
			float spd = TWO;
			Vec_ = model.mat_.trans_ - enemy_.mat_.trans_;
			Vec_ *= spd;
			deadVec_ = true;

			return true;
		}
	}

	return false;
}

void Enemy::SertchPlayer(Model model)
{
	const float sertchScale = 100;

	float a = (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_) * (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_);
	float b = (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_) * (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_);

	float c = model.mat_.scale_.x_ + sertchScale;

	//あたり判定
	if (a + b < c)
	{
		sertchFlag_ = true;
		move_ = enemy_.mat_.trans_ - model.mat_.trans_;
		move_.normalize();

		const float SIZE = 8;

		move_.x_ /= SIZE;
		move_.y_ /= SIZE;
		move_.z_ /= SIZE;
	}
	else
	{
		move_ = { 0,0,0 };
	}
}

void Enemy::DeadVec()
{
	if (deadVec_)
	{
		enemy_.mat_.trans_ -= Vec_;

		bool Y = -15 > enemy_.mat_.trans_.y_ || 1000 < enemy_.mat_.trans_.y_;
		bool X = 0 > enemy_.mat_.trans_.x_ || 1000 < enemy_.mat_.trans_.x_;

		if (Y || X)
		{
			deadVec_ = false;
			isDead_ = true;
		}
	}
}
