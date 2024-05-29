#include "UiManager.h"
#include "ChengeScene.h"

void UiManager::Initialize()
{
	////画像読み込み
	//texP_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/cube.jpg");
	//brPng_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");
	//clearTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameclear.png");
	//overTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameover.png");
	//titleTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Title.png");
	//enemyPng_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
	//heartLesTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartLes.png");
	//heartHaveTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartHave.png");
	//lifeTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/life.png");
	//LTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Lstick.png");
	//RTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Rstick.png");
	//AbuttonTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Abutton.png");
	//PressTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/press.png");
	//LTTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/LT.png");

	////透過するかどうか
	//normalSpriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	//color_ = { 1.0f,1.0f,1.0f,1.0f };

	//{
	//	//基礎
	//	sprite_->Inilialize(normalSpriteCommon_, &matProjection_);

	//	//ライフ英語
	//	lifePng_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	lifePng_->position_ = { -590,240,0 };
	//	lifePng_->scale_ = { 360,144,1 };
	//	lifePng_->SetColor(color_);

	//	//ライフ1
	//	lesPng_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	lesPng_->position_ = { -200,200,0 };
	//	lesPng_->scale_ = { 256,144,1 };
	//	lesPng_->SetColor(color_);

	//	//ライフ2
	//	lesPng2_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	lesPng2_->position_ = { -200,200,0 };
	//	lesPng2_->scale_ = { 256,144,1 };
	//	lesPng2_->SetColor(color_);

	//	//ライフ3
	//	lesPng3_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	lesPng3_->position_ = { -200,200,0 };
	//	lesPng3_->scale_ = { 256,144,1 };
	//	lesPng3_->SetColor(color_);

	//	//ライフ1
	//	havePng_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	havePng_->position_ = { -680,-420,0 };
	//	havePng_->scale_ = { 256,144,1 };
	//	havePng_->SetColor(color_);

	//	//ライフ2
	//	havePng2_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	havePng2_->position_ = { -680,-420,0 };
	//	havePng2_->scale_ = { 256,144,1 };
	//	havePng2_->SetColor(color_);

	//	//ライフ3
	//	havePng3_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	havePng3_->position_ = { -680,-420,0 };
	//	havePng3_->scale_ = { 256,144,1 };
	//	havePng3_->SetColor(color_);

	//	//LStick
	//	UILStick_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	UILStick_->position_ = { -540,-280,0 };
	//	UILStick_->scale_ = { 240,120,1 };
	//	UILStick_->SetColor(color_);

	//	//RStick
	//	UIRStick_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	UIRStick_->position_ = { -460,-320,0 };
	//	UIRStick_->scale_ = { 240,120,1 };
	//	UIRStick_->SetColor(color_);

	//	//LT
	//	UILT_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	UILT_->position_ = { -600,-230,0 };
	//	UILT_->scale_ = { 200,120,1 };
	//	UILT_->SetColor(color_);

	//	//Abutton
	//	UIAButton_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	UIAButton_->position_ = { 180,-230,0 };
	//	UIAButton_->scale_ = { 480,240,1 };
	//	UIAButton_->SetColor(color_);

	//	//Press
	//	UIPress_->Inilialize(normalSpriteCommon_, &matProjection_);
	//	UIPress_->position_ = { -280,-230,0 };
	//	UIPress_->scale_ = { 600,240,1 };
	//	UIPress_->SetColor(color_);
	//};
}

void UiManager::Update()
{
}

void UiManager::Draw()
{
	////今のシーンを取得
	//std::string nowScene = ChengeScene::GetInstance()->GetNowScene();

	////動いているか
	//bool isPlay = ChengeScene::GetInstance()->GetPlayFlag();

	////UIの透明度の増減する速度
	//float spd = 0.03f;

	////
	//if (isPlay)
	//{
	//	if (color_.x_ > 0.0f)
	//	{
	//		color_.x_ -= spd;
	//		color_.y_ -= spd;
	//		color_.z_ -= spd;
	//		color_.w_ -= spd;
	//	}
	//}
	//else
	//{
	//	if (color_.x_ < 1.0f)
	//	{
	//		color_.x_ += spd;
	//		color_.y_ += spd;
	//		color_.z_ += spd;
	//		color_.w_ += spd;
	//	}
	//}

	////今のシーンがTITLEかPLAYなら描画する
	//if (nowScene == "PLAY" || nowScene == "TITLE")
	//{
	//	//Ui描画
	//	UiDraw();
	//}

	////今のシーンがPLAYなら描画する
	//if (nowScene == "PLAY" && !isPlay)
	//{
	//	//ライフ描画
	//	LifeDraw();
	//}
}

void UiManager::Finalize()
{
}

void UiManager::UiDraw()
{
	////操作(UI描画一つにまとめる)
	//UILStick_->Update();
	//UILStick_->SetColor(color_);
	//UILStick_->Draw(LTex_);

	//UIRStick_->Update();
	//UIRStick_->SetColor(color_);
	//UIRStick_->Draw(RTex_);

	//UILT_->Update();
	//UILT_->SetColor(color_);
	//UILT_->Draw(LTTex_);
}

void UiManager::LifeDraw()
{
	////HP表示
	//lifePng_->Update();
	//lifePng_->SetColor(color_);
	//lifePng_->Draw(lifeTex_);

	////描画用の最大HP(HitPoint用のクラスを作る)
	//float maxHP = 3;

	////HP表示(3つまで)
	////(マジックナンバー直す)
	//for (size_t i = 0; i < maxHP; i++)
	//{
	//	if (i < life_)
	//	{
	//		if (i == 0)
	//		{
	//			havePng_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
	//			havePng_->Update();
	//			havePng_->SetColor(color_);
	//			havePng_->Draw(heartHaveTex_);
	//		}
	//		else if (i == 1)
	//		{
	//			havePng2_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
	//			havePng2_->Update();
	//			havePng2_->SetColor(color_);
	//			havePng2_->Draw(heartHaveTex_);
	//		}
	//		else
	//		{
	//			havePng3_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
	//			havePng3_->Update();
	//			havePng3_->SetColor(color_);
	//			havePng3_->Draw(heartHaveTex_);
	//		}
	//	}
	//	else
	//	{
	//		if (i == 0)
	//		{
	//			lesPng_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
	//			lesPng_->Update();
	//			lesPng_->SetColor(color_);
	//			lesPng_->Draw(heartLesTex_);
	//		}
	//		else if (i == 1)
	//		{
	//			lesPng2_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
	//			lesPng2_->Update();
	//			lesPng2_->SetColor(color_);
	//			lesPng2_->Draw(heartLesTex_);
	//		}
	//		else
	//		{
	//			lesPng3_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
	//			lesPng3_->Update();
	//			lesPng3_->SetColor(color_);
	//			lesPng3_->Draw(heartLesTex_);
	//		}
	//	}
	//}
}
