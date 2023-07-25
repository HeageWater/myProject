#include "Particle.h"

Particle::Particle()
{
	/*float vel[3];

	for (size_t i = 0; i < 3; i++)
	{
		vel[i] = (float)MyMath::GetRandom(1, 100);
	}*/

	//方向,タイム,早さをランダムで
	velocity = { 0.1f,0.1f, 0 };

	time = 50.0f;

	spd = 0.01f;

	//spd /= 100;
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

void Particle::Initialize(Vector3D pos)
{
	particle_.mat.Initialize();
	particle_.mat.scale = { 5,5,5 };

	particle_.mat.trans.x = pos.x;
	particle_.mat.trans.y = pos.y;
	particle_.mat.trans.z = pos.z;

	//方向,タイム,早さをランダムで
	velocity = { 0.1f,0.1f, 0 };

	time = 50.0f;

	spd = 0.01f;

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

	if (time < 0)
	{
		isDead = true;
	}

	particle_.MatUpdate(matView, matProjection);
}
