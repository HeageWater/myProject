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
	//playerAttack_.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
	playerAttack_.Initialize(dx_, shader, "Resources\\Model\\Attack\\attack2.obj", pipeline_);

	playerAttack_.mat_.Initialize();
	//playerAttack_.mat_.scale_ = { 50,5,1 };
	playerAttack_.mat_.scale_ = { 10,5,1 };

	controller_ = Controller::GetInstance();
	attackF_ = false;

	sound_ = MyXAudio::GetInstance();

	time_ = 15;
	isDead_ = false;
	deleteFlag = false;
}

void PlayerAttack::Draw()
{
	playerAttack_.Draw(tex_);
}

void PlayerAttack::Update(Matrix matView, Matrix matProjection)
{
	time_--;

	if (time_ <= 1)
	{
		isDead_ = true;
	}

	float maxScale = playerAttack_.mat_.scale_.x_ * 1.5f;
	float spd = 3.0f;

	if (!deleteFlag)
	{
		if (playerAttack_.mat_.scale_.x_ < maxScale)
		{
			playerAttack_.mat_.scale_.x_ += spd;
		}
		else
		{
			deleteFlag = true;
		}

		if (playerAttack_.mat_.scale_.y_ < maxScale / 2)
		{
			playerAttack_.mat_.scale_.y_ += spd;
		}
	}

	if (deleteFlag)
	{
		if (playerAttack_.mat_.scale_.x_ > 0)
		{
			playerAttack_.mat_.scale_.x_ -= spd;
		}

		if (playerAttack_.mat_.scale_.y_ > 0)
		{
			playerAttack_.mat_.scale_.y_ -= spd;
		}
	}

	playerAttack_.MatUpdate(matView, matProjection);
}

void PlayerAttack::SetUpdate()
{
	float angle = 3.1415f / 2;

	if (vec_.x_ > 0)
	{
		angle *= 2;
	}

	vec_.normalize();
	playerAttack_.mat_.rotAngle_.z_ = (angle * vec_.y_);
}
