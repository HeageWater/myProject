#include "ChengeScene.h"
#include "Easing.h"
#include"SceneManager.h"

void ChengeScene::Initialize(Matrix matProjection)
{
	//画像関係
	spriteCommon_->Inilialize(MyDirectX::GetInstance(), false);
	sprite_->Inilialize(spriteCommon_, &matProjection);
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/blockNormal.png");

	//scale用
	float size = 2.5f;

	//pos用
	float width = 1.5f;

	//サイズ
	sprite_->scale_.x_ = Window::window_width_ * size;
	sprite_->scale_.y_ = Window::window_height_ * size;

	//位置
	sprite_->position_.x_ = -Window::window_width_ * width;
	sprite_->position_.y_ = -Window::window_height_;
}

void ChengeScene::Draw(size_t tex)
{
	//ここから画像描画(y軸は-に)
	sprite_->PreDraw();

	//描画
	sprite_->Draw(tex);
}

void ChengeScene::Draw()
{
	//ここから画像描画(y軸は-に)
	sprite_->PreDraw();

	//描画
	sprite_->Draw(tex_);
}

void ChengeScene::Update()
{
	//移動時の最高フレーム
	float maxTime = 120;

	//プレイフラグがたっているか
	if (isPlayFlag_)
	{
		//加算
		time_++;

		float nowChenge = maxTime / 2;

		bool a = time_ < nowChenge;
		bool b = time_ > nowChenge;

		if (a)
		{
			sprite_->position_.x_ = (float)Easing::EaseInOut(-2540.0f, -640.0f, time_ / 100, nowChenge);
		}
		else if (time_ == nowChenge)
		{
			SceneManager::GetInstance()->ChangeScene(next_);
		}
		else if (b)
		{
			sprite_->position_.x_ = (float)Easing::EaseInOut(-640.0f, 2540.0f, (time_ - nowChenge) / 100, nowChenge);
		}
	}

	//maxTimeを超えたらリセット
	if (time_ > maxTime)
	{
		Reset();
	}

	//更新
	sprite_->Update();
}

//リセット
void ChengeScene::Reset()
{
	float width = 1.5f;

	sprite_->scale_.x_ = 1;
	sprite_->scale_.y_ = 1;

	sprite_->position_.x_ = -Window::window_width_ * width;
	sprite_->position_.y_ = -Window::window_height_;

	isPlayFlag_ = false;
	time_ = 0;
}

//プレイフラグを外部からOnにする
void ChengeScene::SetPlayFlag(std::string next)
{
	if (!isPlayFlag_)
	{
		float size = 3.0f;
		float width = 1.5f;

		isPlayFlag_ = true;

		sprite_->scale_.x_ = Window::window_width_ * size;
		sprite_->scale_.y_ = Window::window_height_ * size;

		sprite_->position_.x_ = -Window::window_width_ * width;
		sprite_->position_.y_ = -Window::window_height_;

		time_ = 0;

		next_ = next;
	}
}

//インスタンス化
ChengeScene* ChengeScene::GetInstance()
{
	static ChengeScene chengeScene_;
	return &chengeScene_;
}
