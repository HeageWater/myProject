#include "PlayerAttack.h"
#include "Easing.h"

PlayerAttack::PlayerAttack(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	Initialize(dx_, shader, pipeline_);
}

PlayerAttack::~PlayerAttack()
{
}

void PlayerAttack::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	playerAttack_.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);

	playerAttack_.mat.Initialize();
	playerAttack_.mat.scale = { 15,5,1 };

	controller = Controller::GetInstance();
	attackF = false;

	sound_ = MyXAudio::GetInstance();
	//volcano = sound_->SoundLoadWave("Resources/sound/BGM.wav");

	time = 20;
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

	//playerAttack_.mat.trans += Vector3D{ -vec.x * spd,vec.y * spd,0 };
	//playerAttack_.mat.trans.x += controller->GetLeftStickVec().x;
	//playerAttack_.mat.trans.y += controller->GetLeftStickVec().y;

	//playerAttack_.mat.trans.x += (float)Easing::EaseInBack((double)playerAttack_.mat.trans.x, (double)playerAttack_.mat.trans.x + 10, 10);

	playerAttack_.MatUpdate(matView, matProjection);
}

void PlayerAttack::SetUpdate()
{
	//float range = 120.0f;

	//playerAttack_.mat.trans.x += range * vec.x;
	//playerAttack_.mat.trans.y += range * vec.y;
	//playerAttack_.mat.rotAngle.x = controller->GetRightStickVec().x * 2;
	playerAttack_.mat.rotAngle.z = controller->GetRightStickVec().x * controller->GetRightStickVec().y;
}
