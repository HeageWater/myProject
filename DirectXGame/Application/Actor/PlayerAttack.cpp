#include "PlayerAttack.h"

PlayerAttack::PlayerAttack(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	Initialize(dx_, shader, pipeline_);
}

PlayerAttack::~PlayerAttack()
{
}

void PlayerAttack::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	playerAttack_.Initialize(dx_, shader, "Resources\\kyu\\kyu.obj", pipeline_);

	playerAttack_.mat.Initialize();
	playerAttack_.mat.scale = { 5,5,5 };

	controller = Controller::GetInstance();
	attackF = false;

	sound_ = MyXAudio::Get();
	volcano = sound_->SoundLoadWave("Resources/sound/BGM.wav");

	time = 100;
	isDead = false;
}

void PlayerAttack::Draw()
{
	playerAttack_.Draw(tex);
}

void PlayerAttack::Update(Matrix matView, Matrix matProjection)
{
	time--;

	if (time <= 1)
	{
		isDead = true;
	}

	float spd = 1.5f;

	playerAttack_.mat.trans += Vector3D{ -vec.x * spd,-vec.y * spd,0 };

	/*if (vec.x < vec.y)
	{
		playerAttack_.mat.scale += Vector3D{ 0,-vec.y * spd,0 };
	}
	else
	{
		playerAttack_.mat.scale += Vector3D{ -vec.x * spd,0,0 };
	}*/

	playerAttack_.MatUpdate(matView, matProjection);
}

void PlayerAttack::SetUpdate()
{
	float range = 20.0f;

	playerAttack_.mat.trans.x += range * vec.x;
	playerAttack_.mat.trans.y += range * vec.y;
}
