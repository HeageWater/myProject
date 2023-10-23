#include "TitleScene.h"
#include "imgui.h"
#include "ChengeScene.h"

void TitleScene::Update()
{
	//player更新
	player_->Update(matView_.mat_, matProjection_, shader_);

	//
	player_->MoveY();
	//stageUpdate
	for (auto& object : LoadObjectData::GetInstance()->GetStage())
	{
		object->SetFlag(true);

		object->Update(matView_.mat_, matProjection_);

		//
		if (player_->StageCollsionY(object->stage_))
		{
			//object->SetFlag(false);
		}
	}

	//
	player_->MoveX();
	for (auto& object : LoadObjectData::GetInstance()->GetStage())
	{
		object->SetFlag(true);

		object->Update(matView_.mat_, matProjection_);

		if (player_->StageCollsionX(object->stage_))
		{
			//object->SetFlag(false);
		}
	}


	//targetをplayerに
	matView_.eye_.x_ = player_->GetPos().x_;
	matView_.target_.x_ = player_->GetPos().x_;

	matView_.eye_.y_ = player_->GetPos().y_;
	matView_.target_.y_ = player_->GetPos().y_;

	//jsonファイルから読み込んだものの更新
	LoadObjectData::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	LoadObjectData::GetInstance()->Update();

	//カメラ更新
	matView_.MatUpdate();

	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, 0);

	//シーンチェンジテスト
	if (input_->GetTrigger(DIK_SPACE))
	{
		ChengeScene::GetInstance()->SetPlayFlag("PLAY");
	}

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void TitleScene::Initialize()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//白画像
	block_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/background.png");
	white_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/Player/Player.png");

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

	//jsonファイルから読み込んだものの初期化
	LoadObjectData::GetInstance()->SetModel(shader_, pipeline_.get());
	LoadObjectData::GetInstance()->Initialize();
}

void TitleScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	//// 描画コマンド
	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//スクリーン描画
	screen_.Draw(block_);

	//Actor描画
	player_->Draw(white_, white_);

	//jsonファイルから読み込んだものの描画
	LoadObjectData::GetInstance()->Draw();

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void TitleScene::Finalize()
{
}