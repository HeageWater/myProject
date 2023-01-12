#include "enemy.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	enemy.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
}

void Enemy::Draw(int tex, Matrix matView, Matrix matProjection)
{
	Vector3D hozon = enemy.mat.rotAngle;

	enemy.mat.rotAngle = rot;

	enemy.MatUpdate(matView, matProjection);

	enemy.Draw(tex);

	enemy.mat.rotAngle = hozon;

	enemy.MatUpdate(matView, matProjection);
}

void Enemy::Update(Matrix matView, Matrix matProjection)
{
	if (hitF) {
		enemy.mat.trans += trans * spd;
		spd += 0.01;
	}

	enemy.MatUpdate(matView, matProjection);
}