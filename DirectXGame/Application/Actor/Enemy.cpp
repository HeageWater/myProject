#include "Enemy.h"
#include "Enum.h"
#include "CollisionManager.h"

Enemy::Enemy()
{
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };
}

Enemy::~Enemy()
{

}

void Enemy::Initialize(Shader shader, GPipeline* pipeline_)
{
	//modelを制作
	model_ = std::make_unique<Model>();

	tag_ = "enemy";
	CollisionManager::GetInstance()->AddCollision(this);

	model_->Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\enemy\\enemy.obj", pipeline_);

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 1,1,1 };
	isDead_ = false;
	deadVec_ = false;
	vec_ = { 0,0,0 };
}

void Enemy::Draw(size_t tex)
{
	if (isDead_ == false)
	{
		model_->Draw(tex);
	}
}

void Enemy::Update(Matrix matView, Matrix matProjection)
{
	float spd = 0.005f;

	if (deadVec_)
	{
		spd = 0.01f;
	}

	model_->mat_.rotAngle_.y_ += spd;

	if (Time_ > 0)
	{
		Time_--;

		move_ = { 0,0,0 };
	}

	model_->mat_.trans_ -= move_;

	DeadVec();

	model_->MatUpdate(matView, matProjection);
}

bool Enemy::BoxCollision(Model model)
{
	if (deadVec_ == false)
	{
		float a = (model.mat_.trans_.x_ - model_->mat_.trans_.x_) * (model.mat_.trans_.x_ - model_->mat_.trans_.x_);
		float b = (model.mat_.trans_.y_ - model_->mat_.trans_.y_) * (model.mat_.trans_.y_ - model_->mat_.trans_.y_);

		float c = model.mat_.scale_.x_ + model_->mat_.scale_.x_ * 100;

		//あたり判定
		if (a + b < c * (float)TWO)
		{
			float spd = TWO;
			vec_ = model.mat_.trans_ - model_->mat_.trans_;
			vec_ *= spd;
			deadVec_ = true;

			return true;
		}

		a = (-model.mat_.trans_.x_ - model_->mat_.trans_.x_) * (-model.mat_.trans_.x_ - model_->mat_.trans_.x_);
		b = (-model.mat_.trans_.y_ - model_->mat_.trans_.y_) * (-model.mat_.trans_.y_ - model_->mat_.trans_.y_);

		c = model.mat_.scale_.x_ + model_->mat_.scale_.x_;

		//あたり判定
		if (a + b < c)
		{
			float spd = TWO;
			vec_ = model.mat_.trans_ - model_->mat_.trans_;
			vec_ *= spd;
			deadVec_ = true;

			return true;
		}
	}

	return false;
}

void Enemy::SertchPlayer(Model* model)
{
	const float sertchScale = 5000;

	float a = (model->mat_.trans_.x_ - model_->mat_.trans_.x_) * (model->mat_.trans_.x_ - model_->mat_.trans_.x_);
	float b = (model->mat_.trans_.y_ - model_->mat_.trans_.y_) * (model->mat_.trans_.y_ - model_->mat_.trans_.y_);

	float c = model->mat_.scale_.x_ + sertchScale;

	//あたり判定
	if (a + b < c)
	{
		sertchFlag_ = true;
		move_ = model_->mat_.trans_ - model->mat_.trans_;
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
		model_->mat_.trans_ -= vec_;

		bool Y = -15 > model_->mat_.trans_.y_ || 1000 < model_->mat_.trans_.y_;
		bool X = 0 > model_->mat_.trans_.x_ || 1000 < model_->mat_.trans_.x_;

		if (Y || X)
		{
			deadVec_ = false;
			isDead_ = true;
		}
	}
}
