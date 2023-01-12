#include "enemy.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	enemy.Initialize(dx_, shader, "Resources\\ene\\ene.obj", pipeline_);
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
	//if () {
		enemy.mat.trans.y -= 0.2f;
	//}

	if (hitF) {
		enemy.mat.trans -= trans * spd;

		enemy.mat.rotAngle.y += 1;

		spd += 0.1;
	}

	enemy.MatUpdate(matView, matProjection);
}