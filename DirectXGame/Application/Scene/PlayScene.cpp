#include "PlayScene.h"
#include "imgui.h"
#include "ChengeScene.h"

void PlayScene::Update()
{
	//最初のムービー用タイマー
	if (time_ < 100)
		time_++;

	//
	player_->MoveY();
	//stageUpdate
	for (auto& object : LoadObjectData::GetInstance()->GetStage())
	{
		object->SetFlag(true);

		object->Update(matView_.mat_, matProjection_);

		if (player_->StageCollsionY(object->stage_))
		{

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

		}
	}

	//targetをplayerに
	matView_.eye_.x_ = player_->GetPos().x_;
	matView_.target_.x_ = player_->GetPos().x_;

	//playerのyからどれくらい離すか
	float prusTargetY = 10;

	matView_.eye_.y_ = player_->GetPos().y_ + prusTargetY;
	matView_.target_.y_ = player_->GetPos().y_ + prusTargetY;

	/*if (time_ == 75)
	{
		Vector3D pos = player_->GetPos();
		pos -= {0, 5, 0};
		CreatePatricle(pos);
	}*/

	//player更新
	player_->Update(matView_.mat_, matProjection_, shader_);

	//jsonファイルから読み込んだものの更新
	LoadObjectData::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	LoadObjectData::GetInstance()->Update();

	//カメラ更新
	matView_.MatUpdate();

	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, 0);

	//パーティクル
	for (size_t i = 0; i < boxParticles_.size(); i++)
	{
		boxParticles_[i]->Update(matView_.mat_, matProjection_);

		//削除
		if (boxParticles_[i]->IsDead() == true)
		{
			boxParticles_.erase(boxParticles_.begin() + i);
		}
	}

	//シーンチェンジテスト
	if (input_->GetTrigger(DIK_SPACE))
	{
		ChengeScene::GetInstance()->SetPlayFlag("GAMEOVER");
	}

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void PlayScene::Initialize()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//箱画像
	white_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/cube.jpg");

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
	//Vector3D pos = player_->GetPos();
	//pos += {-50, 0, 0};
	//player_->SetPos(pos);

	//jsonファイルから読み込んだものの初期化
	LoadObjectData::GetInstance()->SetModel(shader_, pipeline_.get());
	LoadObjectData::GetInstance()->Initialize();

	//ステージ読み込み
	LoadObjectData::GetInstance()->StageLoad("stage2");

	time_ = 0;
}

void PlayScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	//// 描画コマンド
	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//スクリーン描画
	screen_.Draw(white_);

	//Actor描画
	player_->Draw(white_, white_);

	//ボックスパーティクル
	for (size_t i = 0; i < boxParticles_.size(); i++)
	{
		boxParticles_[i]->Draw(white_);
	}

	//jsonファイルから読み込んだものの描画
	LoadObjectData::GetInstance()->Draw();

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void PlayScene::Finalize()
{
	for (auto& object : boxParticles_)
	{
		delete object;
	}
}


//ランダムの数パーティクルを出す
void PlayScene::CreatePatricle(Vector3D pos)
{
	size_t minRange = 10;
	size_t maxRange = 30;

	size_t play = MyMath::GetRandom(minRange, maxRange);

	for (size_t i = 0; i < play; i++)
	{
		BoxParticle* newP = new BoxParticle();

		newP->Initialize(shader_, pipeline_.get());

		newP->SetPos(pos);

		boxParticles_.push_back(newP);
	}
}
