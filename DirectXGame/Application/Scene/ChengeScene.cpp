#include "ChengeScene.h"
#include "Easing.h"
#include "SceneManager.h"

void ChengeScene::Initialize(Matrix matProjection)
{
	//画像関係
	spriteCommon_->Inilialize(MyDirectX::GetInstance(), true);
	sprite_->Inilialize(spriteCommon_, &matProjection);
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Sprite/black.png");

	//scale用
	const float size = 15.5f;

	//サイズ
	sprite_->scale_ = { Window::window_width_ * size,Window::window_height_ * size,1 };

	//位置
	sprite_->position_ = { -680,-Window::window_height_,0 };

	//カラー
	sprite_->SetColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));

	//カラー
	color_ = { 0.0f,0.0f,0.0f,0.0f };
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
	//プレイフラグがたっているか
	if (isPlayFlag_)
	{
		if (!chengeFlag_)
		{
			//ブラックアウトテスト
			if (color_.x_ < 1.0f)
			{
				color_.x_ += 0.01f;
				color_.y_ += 0.01f;
				color_.z_ += 0.01f;
				color_.w_ += 0.01f;

				if (color_.x_ >= 1.0f)
				{
					SceneManager::GetInstance()->ChangeScene(next_);
					chengeFlag_ = true;
				}
			}
			else
			{
				color_.x_ = 1.0f;
				color_.y_ = 1.0f;
				color_.z_ = 1.0f;
				color_.w_ = 1.0f;
			}
		}
		else
		{
			//ブラックアウトテスト
			if (color_.x_ > 0.0f)
			{
				color_.x_ -= 0.01f;
				color_.y_ -= 0.01f;
				color_.z_ -= 0.01f;
				color_.w_ -= 0.01f;

				if (color_.x_ < 0.0f)
				{
					Reset();
				}
			}
		}
	}

	//カラー
	sprite_->SetColor(color_);

	//更新
	sprite_->Update();
}

//リセット
void ChengeScene::Reset()
{
	float width = 2.0f;

	sprite_->scale_.x_ = 1;
	sprite_->scale_.y_ = 1;

	sprite_->position_.x_ = -Window::window_width_ * width;
	sprite_->position_.y_ = -Window::window_height_;

	isPlayFlag_ = false;
	time_ = 0;
}

//プレイフラグを外部からonにする
void ChengeScene::SetPlayFlag(std::string next)
{
	if (!isPlayFlag_)
	{
		//scale用
		const float size = 15.5f;

		isPlayFlag_ = true;

		//サイズ
		sprite_->scale_ = { Window::window_width_ * size,Window::window_height_ * size,1 };

		//位置
		sprite_->position_ = { -680,-Window::window_height_,0 };

		//カラー
		sprite_->SetColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));

		color_ = { 0.0f,0.0f,0.0f,0.0f };

		next_ = next;

		chengeFlag_ = false;
	}
}

//インスタンス化
ChengeScene* ChengeScene::GetInstance()
{
	static ChengeScene chengeScene_;
	return &chengeScene_;
}
