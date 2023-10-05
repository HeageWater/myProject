#include "Particle.h"

Particle::Particle()
{
	float vel[3];

	for (size_t i = 0; i < 3; i++)
	{
		vel[i] = (float)MyMath::GetRandom(0, 100) - 50;

		vel[i] /= 100;
	}

	//方向,タイム,早さをランダムで
	velocity = { vel[0],vel[1],vel[2] };

	//tine
	time = (float)MyMath::GetRandom(30, 50);

	//spd
	spd = (float)MyMath::GetRandom(0, 15) - 15;

	spd /= 100;
}

Particle::~Particle()
{
}

void Particle::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	particle_.Initialize(dx_, shader, "Resources\\kyu\\kyu.obj", pipeline_);

	particle_.mat.Initialize();
	particle_.mat.scale = { 5,5,5 };

	//方向,タイム,早さをランダムで
	velocity = { 0.1f,0.1f, 0 };

	time = 50.0f;

	spd = 0.01f;
}

void Particle::Initialize(MyDirectX* dx_, GPipeline* pipeline_)
{
	particle_.Initialize(dx_,pipeline_);
}

void Particle::Initialize(Vector3D pos)
{
	particle_.mat.Initialize();
	particle_.mat.scale = { 5,5,5 };

	particle_.mat.trans.x = pos.x;
	particle_.mat.trans.y = pos.y;
	particle_.mat.trans.z = pos.z;

	//方向,タイム,早さをランダムで
	float vel[3];

	for (size_t i = 0; i < 3; i++)
	{
		vel[i] = (float)MyMath::GetRandom(0, 100) - 50;

		vel[i] /= 100;
	}

	velocity = { vel[0],vel[1],vel[2] };

	//tine
	time = (float)MyMath::GetRandom(30, 50);

	//spd
	spd = (float)MyMath::GetRandom(0, 15) - 15;

	spd /= 100;

	isDead = false;
}

void Particle::Draw(size_t tex)
{
	particle_.Draw(tex);
}

void Particle::Update(Matrix matView, Matrix matProjection)
{
	particle_.mat.trans += velocity * spd;

	float minSpd = 0.05f;

	particle_.mat.scale -= {minSpd, minSpd, minSpd};

	time--;

	bool timeZero = time < 0;
	bool scaleZeroX = particle_.mat.scale.x < 0;
	bool scaleZeroY = particle_.mat.scale.y < 0;
	bool scaleZeroZ = particle_.mat.scale.z < 0;

	if (timeZero || scaleZeroX || scaleZeroY || scaleZeroZ)
	{
		isDead = true;
	}

	particle_.MatUpdate(matView, matProjection);
}
