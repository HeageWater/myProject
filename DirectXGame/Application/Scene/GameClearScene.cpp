#include "GameClearScene.h"
#include "ChengeScene.h"

void GameClearScene::Initialize()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//画像
	clearTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameclear.png");
	plyerTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/Player/Player.png");
	blockTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/background.png");

	//shader
	shader_.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	bilShader_.Initizlize(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");

	//pipeline
	pipeline_ = std::make_unique<GPipeline>();
	pipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), shader_);

	//描画初期化
	multipathPipeline_ = std::make_unique<GPipeline>();
	multipathPipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), bilShader_);

	//背景のスクリーン(これが必要なので依存しないようにしたい)
	screen_.Initialize(multipathPipeline_.get(), bilShader_);
	screen_.obj_.trans_.z_ = 100.1f;
	screen_.obj_.scale_ = { Window::window_width_ * 2,Window::window_height_ / 2,0.2f };

	//player
	player_->Initialize(shader_, pipeline_.get());

	//透過するかどうか
	normalSpriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	//基礎
	sprite_->Inilialize(normalSpriteCommon_, &matProjection_);

	//
	clearPng_->Inilialize(normalSpriteCommon_, &matProjection_);
	clearPng_->position_ = { -680,-420,0 };
	clearPng_->scale_ = { 3600,1440,1 };
}

void GameClearScene::Update()
{
	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, 0);

	//
	clearPng_->Update();

	if (input_->GetTrigger(DIK_SPACE) || controller_->ButtonTriggerPush(A))
	{
		ChengeScene::GetInstance()->SetPlayFlag("TITLE");
	}

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void GameClearScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	//// 描画コマンド
	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//スクリーン描画
	screen_.Draw(blockTex_);

	//Actor描画
	player_->Draw(plyerTex_, plyerTex_);

	//スプライトのプレドロー
	sprite_->PreDraw();

	//
	clearPng_->Draw(clearTex_);

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void GameClearScene::Finalize()
{
}
