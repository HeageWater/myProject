#include "Particle.h"
#include "DirectX.h"

Particle::Particle()
{
	float vel[3];

	for (size_t i = 0; i < 3; i++)
	{
		vel[i] = (float)MyMath::GetRandom(0, 100) - 50;

		vel[i] /= 100;
	}

	//方向,タイム,早さをランダムで
	velocity_ = { vel[0],vel[1],vel[2] };

	//tine
	time_ = (float)MyMath::GetRandom(30, 50);

	//spd
	spd_ = (float)MyMath::GetRandom(0, 15) - 15;

	spd_ /= 100;
}

Particle::~Particle()
{
}

void Particle::Initialize(Shader shader, GPipeline* pipeline_)
{
	particle_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\kyu\\kyu.obj", pipeline_);

	particle_.mat.Initialize();
	particle_.mat.scale = { 5,5,5 };

	//方向,タイム,早さをランダムで
	velocity_ = { 0.1f,0.1f, 0 };

	time_ = 50.0f;

	spd_ = 0.01f;
}

void Particle::SetInitialize(Vector3D pos)
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

	velocity_ = { vel[0],vel[1],vel[2] };

	//tine
	time_ = (float)MyMath::GetRandom(30, 50);

	//spd
	spd_ = (float)MyMath::GetRandom(0, 15) - 15;

	spd_ /= 100;

	isDead_ = false;
}

void Particle::Draw(size_t tex)
{
	particle_.Draw(tex);
}

void Particle::Update(Matrix matView, Matrix matProjection)
{
	particle_.mat.trans += velocity_ * spd_;

	float minSpd = 0.05f;

	particle_.mat.scale -= {minSpd, minSpd, minSpd};

	time_--;

	bool timeZero = time_ < 0;
	bool scaleZeroX = particle_.mat.scale.x < 0;
	bool scaleZeroY = particle_.mat.scale.y < 0;
	bool scaleZeroZ = particle_.mat.scale.z < 0;

	if (timeZero || scaleZeroX || scaleZeroY || scaleZeroZ)
	{
		isDead_ = true;
	}

	particle_.MatUpdate(matView, matProjection);
}
