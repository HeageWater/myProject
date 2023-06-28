#include "Enemy.h"

Enemy::Enemy()
{
	enemy.mat.Initialize();
	enemy.mat.scale = { 3,3,3 };
}

Enemy::~Enemy()
{

}

void Enemy::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	enemy.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);

	enemy.mat.Initialize();
	enemy.mat.scale = { 10,10,10 };
}

void Enemy::Draw(int tex)
{
	enemy.Draw(tex);
}

void Enemy::Update(Matrix matView, Matrix matProjection)
{
	enemy.MatUpdate(matView, matProjection);
}

void Enemy::Reset()
{
	enemy.mat.trans.x = 0;
	enemy.mat.trans.y = 0;
}
