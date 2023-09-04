#include "ChengeScene.h"
#include "Easing.h"

ChengeScene::ChengeScene()
{
	tex = 0;
	time = 0;
}

ChengeScene::~ChengeScene()
{
}

void ChengeScene::Initialize(MyDirectX* dx_, GPipeline* pipeline_, Matrix matProjection)
{
	spriteCommon->Inilialize(dx_, false);

	sprite_->Inilialize(spriteCommon, &matProjection);

	tex = dx_->LoadTextureGraph(L"Resources/sprite/blockNormal.png");

	//scale—p
	float size = 2.5f;

	//pos—p
	float width = 1.5f;

	sprite_->scale.x = Window::window_width * size;
	sprite_->scale.y = Window::window_height * size;

	sprite_->position.x = -Window::window_width * width;
	sprite_->position.y = -Window::window_height;
}

void ChengeScene::Draw(size_t tex)
{
	//‚±‚±‚©‚ç‰æ‘œ•`‰æ(yŽ²‚Í-‚É)
	sprite_->PreDraw();

	sprite_->Draw(tex);
}

void ChengeScene::Draw()
{
	//‚±‚±‚©‚ç‰æ‘œ•`‰æ(yŽ²‚Í-‚É)
	sprite_->PreDraw();

	sprite_->Draw(tex);
}

void ChengeScene::Update(Matrix matView, Matrix matProjection)
{
	if (isPlayFlag)
	{
		float width = 1.5f;

		time++;

		float maxTime = 15;
		sprite_->position.x = (float)Easing::EaseInOut(-2200.0f, -640.0f, time / 100, maxTime);

	}

	bool widthOut = sprite_->position.x > Window::window_width;
	//bool widthOut = sprite_->position.x < 0 || sprite_->position.x > Window::window_width;
	//bool heightOut = sprite_->position.y < 0 || sprite_->position.y > Window::window_height;

	if (widthOut)
	{
		isPlayFlag = false;
		time = 0;
	}

	sprite_->Update();
}

void ChengeScene::Reset()
{
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
