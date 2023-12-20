#include "GameClearScene.h"
#include "ChengeScene.h"
#include "ParticleManager.h"

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
	moiePlayer_->Initialize(shader_, pipeline_.get());

	//透過するかどうか
	normalSpriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	//基礎
	sprite_->Inilialize(normalSpriteCommon_, &matProjection_);

	//
	clearPng_->Inilialize(normalSpriteCommon_, &matProjection_);
	clearPng_->position_ = { -680,0,0 };
	clearPng_->scale_ = { 3600,1440,1 };
	clearPng_->SetColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));

	//jsonファイルから読み込んだものの初期化
	LoadObjectData::GetInstance()->SetModel(shader_, pipeline_.get());
	LoadObjectData::GetInstance()->Initialize();

	//ステージ読み込み
	LoadObjectData::GetInstance()->StageLoad("ClearStage");

	ClearGravity = false;
	notP = false;

	//
	ParticleManager::GetInstance()->Initalize();
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->SetDraw(shader_, pipeline_.get());

}

void GameClearScene::Update()
{
	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, 0);

	//
	clearPng_->Update();

	//
	moiePlayer_->Update(matView_.mat_, matProjection_, shader_);

	//
	controller_->Update();

	//
	LoadObjectData::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	LoadObjectData::GetInstance()->Update();

	if (input_->GetTrigger(DIK_SPACE) || controller_->ButtonTriggerPush(A))
	{
		ChengeScene::GetInstance()->SetPlayFlag("TITLE");
	}

	if (moiePlayer_->PlayerSet() && !notP)
	{
		ClearGravity = true;
	}

	if (ClearGravity && !notP)
	{
		ParticleManager::GetInstance()->CreateBoxParticle(moiePlayer_->GetPos());
		notP = true;
	}

	if (notP)
	{
		if (clearPng_->position_.y_ > -420)
		{
			clearPng_->position_.y_ -= 10;
		}
	}

	//パーティクル更新
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->Update();

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
	moiePlayer_->Draw(plyerTex_);

	//
	LoadObjectData::GetInstance()->Draw();

	//ボックスパーティクル
	ParticleManager::GetInstance()->Draw();

	//
	clearPng_->Draw(clearTex_);

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void GameClearScene::Finalize()
{
	//ボックスパーティクル
	ParticleManager::GetInstance()->Finalize();
}
