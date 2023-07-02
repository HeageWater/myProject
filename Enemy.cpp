#include "Enemy.h"

Enemy::Enemy()
{
	enemy_.mat.Initialize();
	enemy_.mat.scale = { 3,3,3 };
}

Enemy::~Enemy()
{

}

void Enemy::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	enemy_.Initialize(dx_, shader, "Resources\\ene\\ene.obj", pipeline_);

	enemy_.mat.Initialize();
	enemy_.mat.scale = { 5,5,3 };
	isDead = false;
	deadVec = false;
	Vec = { 0,0,0 };
}

void Enemy::Draw(int tex)
{
	if (isDead == false)
	{
		enemy_.Draw(tex);
	}
}

void Enemy::Update(Matrix matView, Matrix matProjection)
{
	float spd = 0.01f;

	if (deadVec)
	{
		spd = 1.2f;
	}

	enemy_.mat.rotAngle.y += spd;

	DeadVec();

	enemy_.MatUpdate(matView, matProjection);
}

bool Enemy::BoxCollision(Model model)
{
	if (deadVec == false)
	{
		int a = (model.mat.trans.x - enemy_.mat.trans.x) * (model.mat.trans.x - enemy_.mat.trans.x);
		int b = (model.mat.trans.y - enemy_.mat.trans.y) * (model.mat.trans.y - enemy_.mat.trans.y);

		int c = model.mat.scale.x * enemy_.mat.scale.x;

		//‚ ‚½‚è”»’è
		if (a + b < c)
		{
			float spd = 1.5f;
			Vec = model.mat.trans - enemy_.mat.trans;
			Vec *= spd;
			deadVec = true;

			return true;
		}
	}

	return false;
}

void Enemy::DeadVec()
{
	if (deadVec)
	{
		enemy_.mat.trans -= Vec;

		bool Y = -15 > enemy_.mat.trans.y || 1000 < enemy_.mat.trans.y;
		bool X = 0 > enemy_.mat.trans.x || 1000 < enemy_.mat.trans.x;

		if (Y || X)
		{
			deadVec = false;
			isDead = true;
		}
	}
}
