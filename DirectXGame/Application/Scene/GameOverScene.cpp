#include "GameOverScene.h"
#include "ChengeScene.h"
#include "ImGui.h"

void GameOverScene::Initialize()
{
#ifdef _DEBUG

	//ImGui受付開始
	ImguiManager::GetInstance()->Begin();
	float test1 = 0.5f;

	ImGui::Text("test");
	ImGui::SliderFloat("Test", &test1, 0.01f, 0.99f);

	//titleSceneheへ
	if (ImGui::Button("TITLE"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("TITLE");
	}

	//playSceneheへ
	if (ImGui::Button("Play"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("PLAY");
	}

	//clearSceneheへ
	if (ImGui::Button("GAMECLEAR"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("GAMECLEAR");
	}

	//goalSceneheへ
	if (ImGui::Button("GAMEOVER"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("GAMEOVER");
	}

	//ImGui受付終了
	ImguiManager::GetInstance()->End();

#endif _DEBUG

	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//
	overTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameover.png");
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
	overPng_->Inilialize(normalSpriteCommon_, &matProjection_);
	overPng_->position_ = { -680,-420,0 };
	overPng_->scale_ = { 3600,1440,1 };
	overPng_->SetColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
}

void GameOverScene::Update()
{
	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, 0);

	//
	overPng_->Update();

	if (input_->GetTrigger(DIK_SPACE) || controller_->ButtonTriggerPush(A))
	{
		ChengeScene::GetInstance()->SetPlayFlag("TITLE");
	}

	//
	controller_->Update();

	//
	LoadObjectData::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	LoadObjectData::GetInstance()->Update();

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void GameOverScene::Draw()
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
	player_->Draw(plyerTex_);

	//
	overPng_->Draw(overTex_);

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

#ifdef _DEBUG

	//ImGui描画
	ImguiManager::GetInstance()->Draw();

#endif _DEBUG

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void GameOverScene::Finalize()
{
}
