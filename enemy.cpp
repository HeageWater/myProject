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

	if (!hitF && lerpF)
	{
		nowCount += 0.01f;
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float>(elapsedCount);

		timeRate = min(elapsedTime / maxTime, 1.0f);

		a = a.lerp(p0, p1, timeRate);
		b = b.lerp(p2, p3, timeRate);

		position = position.lerp(a, b, timeRate);

		enemy.mat.trans = position;

		if (enemy.mat.trans.x >= p3.x ||
			enemy.mat.trans.y >= p3.y)
		{
			deathF = true;
		}
	}

	if (!lerpF) 
	{
		enemy.mat.trans.y -= 0.2f;
	}
	
	//êÅÇ¡îÚÇŒÇ≥ÇÍÇÈ
	if (hitF)
	{
		enemy.mat.trans -= trans * spd;

		enemy.mat.rotAngle.y += 1;

		spd += 0.1;
	}

	enemy.MatUpdate(matView, matProjection);
}