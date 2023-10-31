#include "TitleScene.h"
#include "imgui.h"
#include "ChengeScene.h"

void TitleScene::Update()
{
	//hitStop更新
	hitStop_->Update();

	//まだ時間無いなら戻る
	if (hitStop_->GetTimeFlag())
	{
		return;
	}

	if (!ChengeScene::GetInstance()->GetPlayFlag() && !titleObject->IsMovie_)
	{
		//player更新
		player_->Update(matView_.mat_, matProjection_, shader_);

		//縦移動
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

		//横移動
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

		float prusTargetY = 10;

		matView_.eye_.y_ = player_->GetPos().y_ + prusTargetY;
		matView_.target_.y_ = player_->GetPos().y_ + prusTargetY;
	}

	//タイトルオブジェ更新
	titleObject->Update(matView_.mat_, matProjection_);

	//
	if (titleObject->BoxCollision(player_->GetAttackModel()) && !titleObject->IsMovie_)
	{
		titleObject->Movie();

		//この下の処理まとめろ
		float setStopTime = 17.0f;

		//止まる
		hitStop_->SetTime(setStopTime);

		//hit音
		sound_->SoundPlayWave(hitSound_);
	}

	//
	if (titleObject->EndMovie_)
	{
		titleObject->EndMovie_ = false;

		ChengeScene::GetInstance()->SetPlayFlag("PLAY");
	}

	if (titleObject->IsMovie_)
	{
		matView_.eye_.x_ = 0;
		matView_.target_.x_ = 0;

		matView_.eye_.y_ = 15;
		matView_.target_.y_ = 15;

		player_->SetCamera(matView_.mat_,matProjection_);
	}

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
	blockTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/background.png");
	plyerTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/Player/Player.png");
	whiteTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Sprite/white1x1.png");

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

	//タイトルモデルの初期化
	titleObject->Initialize(shader_, pipeline_.get());

	//jsonファイルから読み込んだものの初期化
	LoadObjectData::GetInstance()->SetModel(shader_, pipeline_.get());
	LoadObjectData::GetInstance()->Initialize();

	//音読み込み
	hitSound_ = sound_->SoundLoadWave("Resources/sound/se_hit_008.wav");
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
	screen_.Draw(blockTex_);

	//Actor描画
	player_->Draw(plyerTex_, plyerTex_);

	//タイトル
	titleObject->Draw(whiteTex_);

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