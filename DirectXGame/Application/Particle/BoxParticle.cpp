#include "BoxParticle.h"

BoxParticle::BoxParticle()
{
	float vel[3] = { 0,0,0 };

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

	spd /= 10;
}

BoxParticle::~BoxParticle()
{
}

void BoxParticle::Initialize(Shader shader, GPipeline* pipeline_)
{
	box.Initialize(MyDirectX::GetInstance(), pipeline_, shader);

	box.mat.Initialize();

	float setScale = 3.0f;
	box.mat.scale = { setScale,setScale,setScale };
}

void BoxParticle::Draw(size_t tex)
{
	box.Draw(tex);
}

void BoxParticle::Update(Matrix matView, Matrix matProjection)
{
	box.mat.trans += velocity * spd;
	box.mat.rotAngle += velocity * spd;

	float minSpd = 0.05f;

	box.mat.scale -= {minSpd, minSpd, minSpd};

	time--;

	bool timeZero = time < 0;
	bool scaleZeroX = box.mat.scale.x < 0;
	bool scaleZeroY = box.mat.scale.y < 0;
	bool scaleZeroZ = box.mat.scale.z < 0;

	if (timeZero || scaleZeroX || scaleZeroY || scaleZeroZ)
	{
		isDead = true;
	}

	box.MatUpdate(matView, matProjection);
}
