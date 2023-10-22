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
	time_ = (float)MyMath::GetRandom(30, 50);

	//spd
	spd_ = (float)MyMath::GetRandom(0, 15) - 15;

	spd_ /= 10;
}

BoxParticle::~BoxParticle()
{
}

void BoxParticle::Initialize(Shader shader, GPipeline* pipeline_)
{
	box_.Initialize(MyDirectX::GetInstance(), pipeline_, shader);

	box_.mat.Initialize();

	float setScale = 3.0f;
	box_.mat.scale = { setScale,setScale,setScale };
}

void BoxParticle::Draw(size_t tex)
{
	box_.Draw(tex);
}

void BoxParticle::Update(Matrix matView, Matrix matProjection)
{
	box_.mat.trans += velocity * spd_;
	box_.mat.rotAngle += velocity * spd_;

	float minSpd = 0.05f;

	box_.mat.scale -= {minSpd, minSpd, minSpd};

	time_--;

	bool timeZero = time_ < 0;
	bool scaleZeroX = box_.mat.scale.x < 0;
	bool scaleZeroY = box_.mat.scale.y < 0;
	bool scaleZeroZ = box_.mat.scale.z < 0;

	if (timeZero || scaleZeroX || scaleZeroY || scaleZeroZ)
	{
		isDead_ = true;
	}

	box_.MatUpdate(matView, matProjection);
}
