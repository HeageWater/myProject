#include "Enemy.h"

Enemy::Enemy()
{
	enemy_.mat.Initialize();
	enemy_.mat.scale = { 3,3,3 };
}

Enemy::~Enemy()
{

}

void Enemy::Initialize(Shader shader, GPipeline* pipeline_)
{
	enemy_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\ene\\ene.obj", pipeline_);

	enemy_.mat.Initialize();
	enemy_.mat.scale = { 1,1,1 };
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
	float spd = 0.01f;

	if (deadVec_)
	{
		spd = 1.2f;
	}

	enemy_.mat.rotAngle.y += spd;

	if (Time_ > 0)
	{
		Time_--;

		move_ = { 0,0,0 };
	}

	enemy_.mat.trans -= move_;

	DeadVec();

	enemy_.MatUpdate(matView, matProjection);
}

bool Enemy::BoxCollision(Model model)
{
	if (deadVec_ == false)
	{
		float a = (model.mat.trans.x - enemy_.mat.trans.x) * (model.mat.trans.x - enemy_.mat.trans.x);
		float b = (model.mat.trans.y - enemy_.mat.trans.y) * (model.mat.trans.y - enemy_.mat.trans.y);

		float c = model.mat.scale.x * enemy_.mat.scale.x;

		//あたり判定
		if (a + b < c)
		{
			float spd = 1.5f;
			Vec_ = model.mat.trans - enemy_.mat.trans;
			Vec_ *= spd;
			deadVec_ = true;

			return true;
		}
	}

	return false;
}

void Enemy::SertchPlayer(Model model)
{
	float sertchScale = 500;
	//Vector3D sertchScale = {10,10,10};

	float a = (model.mat.trans.x - enemy_.mat.trans.x) * (model.mat.trans.x - enemy_.mat.trans.x);
	float b = (model.mat.trans.y - enemy_.mat.trans.y) * (model.mat.trans.y - enemy_.mat.trans.y);

	float c = model.mat.scale.x * sertchScale;
	//float c = model.mat.scale.x * enemy_.mat.scale.x;

	//あたり判定
	if (a + b < c)
	{
		sertchFlag_ = true;
		move_ = enemy_.mat.trans - model.mat.trans;
		move_.normalize();

		move_.x /= 2;
		move_.y /= 2;
		move_.z /= 2;
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
		enemy_.mat.trans -= Vec_;

		bool Y = -15 > enemy_.mat.trans.y || 1000 < enemy_.mat.trans.y;
		bool X = 0 > enemy_.mat.trans.x || 1000 < enemy_.mat.trans.x;

		if (Y || X)
		{
			deadVec_ = false;
			isDead_ = true;
		}
	}
}
