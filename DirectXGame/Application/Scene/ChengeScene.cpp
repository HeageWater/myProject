#include "ChengeScene.h"
#include "Easing.h"
#include"SceneManager.h"

void ChengeScene::Initialize(Matrix matProjection)
{
	spriteCommon->Inilialize(MyDirectX::GetInstance(), false);

	sprite_->Inilialize(spriteCommon, &matProjection);

	tex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/blockNormal.png");

	//scale用
	float size = 2.5f;

	//pos用
	float width = 1.5f;

	sprite_->scale.x = Window::window_width * size;
	sprite_->scale.y = Window::window_height * size;

	sprite_->position.x = -Window::window_width * width;
	sprite_->position.y = -Window::window_height;
}

void ChengeScene::Draw(size_t tex_)
{
	//ここから画像描画(y軸は-に)
	sprite_->PreDraw();

	sprite_->Draw(tex_);
}

void ChengeScene::Draw()
{
	//ここから画像描画(y軸は-に)
	sprite_->PreDraw();

	sprite_->Draw(tex);
}

void ChengeScene::Update()
{
	float maxTime = 60;

	if (isPlayFlag)
	{
		time++;

		if (time > 30.0f)
		{
			sprite_->position.x = (float)Easing::EaseInOut(-2200.0f, -640.0f, time, maxTime);
		}
		else if (time == 31.0f)
		{
			SceneManager::GetInstance()->ChangeScene("PLAY");
		}
		else
		{

		}

		if (time > maxTime)
		{
			Reset();
		}
	}

	sprite_->Update();
}

void ChengeScene::Reset()
{
	isPlayFlag = false;
	time = 0;
}

void ChengeScene::SetPlayFlag()
{
	if (!isPlayFlag)
	{
		float size = 3.0f;
		float width = 1.5f;

		isPlayFlag = true;

		sprite_->scale.x = Window::window_width * size;
		sprite_->scale.y = Window::window_height * size;

		sprite_->position.x = -Window::window_width * width;
		sprite_->position.y = -Window::window_height;

		time = 0;
	}
}

ChengeScene* ChengeScene::GetInstance()
{
	static ChengeScene chengeScene_;
	return &chengeScene_;
}
