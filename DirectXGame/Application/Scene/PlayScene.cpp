#include "PlayScene.h"
#include "imgui.h"
#include "ChengeScene.h"

void PlayScene::Update()
{
	//最初のムービー用タイマー
	if (time_ < 100)
	{
		time_++;
	}

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

	//パーティクルテスト
	if (input_->GetTrigger(DIK_1))
	{
		ParticleManager::GetInstance()->CreateBoxParticle(player_->GetPos());
	}

	//player_->PlayerCollision(player_->GetModel());

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
	goal_->Update(matView_.mat_, matProjection_);

	//jsonファイルから読み込んだものの更新
	LoadObjectData::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	LoadObjectData::GetInstance()->Update();

	//カメラ更新
	matView_.MatUpdate();

	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, 0);

	//パーティクル更新
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->Update();

	//
	bool checkGoal = goal_->BoxCollision(player_->GetModel());

	//GameClear条件が達成されたら
	if (checkGoal)
	{
		ChengeScene::GetInstance()->SetPlayFlag("GAMECLEAR");
		checkGoal = false;
	}

	//GameOver条件が達成されたら
	if (player_->GetLife() <= 0)
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
	Vector3D pos = player_->GetPos();
	pos += {0, 10, 0};
	player_->SetPos(pos);

	//jsonファイルから読み込んだものの初期化
	LoadObjectData::GetInstance()->SetModel(shader_, pipeline_.get());
	LoadObjectData::GetInstance()->Initialize();

	//ステージ読み込み
	LoadObjectData::GetInstance()->StageLoad("stage");

	//開始地点をセット
	player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

	//ゴール初期化
	goal_->Initialize(MyDirectX::GetInstance(), shader_, pipeline_.get());
	goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

	//
	ParticleManager::GetInstance()->Initalize();
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->SetDraw(shader_, pipeline_.get());

	//白画像
	blockTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/background.png");
	plyerTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/Player/Player.png");
	whiteTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Sprite/white1x1.png");

	//
	time_ = 0;

	//画像読み込み
	texP_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/cube.jpg");
	brPng_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");
	clearTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameclear.png");
	overTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameover.png");
	titleTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Title.png");
	enemyPng_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
	blackTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/black.png");
	heartLesTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartLes.png");
	heartHaveTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartHave.png");
	lifeTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/life.png");
	LTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Lstick.png");
	RTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Rstick.png");
	AbuttonTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Abutton.png");
	PressTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/press.png");
	LTTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/LT.png");

	//透過するかどうか
	normalSpriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	{
		//基礎
		sprite_->Inilialize(normalSpriteCommon_, &matProjection_);

		//ライフ英語
		lifePng_->Inilialize(normalSpriteCommon_, &matProjection_);
		lifePng_->position_ = { -590,240,0 };
		lifePng_->scale_ = { 360,144,1 };

		//ライフ1
		lesPng_->Inilialize(normalSpriteCommon_, &matProjection_);
		lesPng_->position_ = { -200,200,0 };
		lesPng_->scale_ = { 256,144,1 };

		//ライフ2
		lesPng2_->Inilialize(normalSpriteCommon_, &matProjection_);
		lesPng2_->position_ = { -200,200,0 };
		lesPng2_->scale_ = { 256,144,1 };

		//ライフ3
		lesPng3_->Inilialize(normalSpriteCommon_, &matProjection_);
		lesPng3_->position_ = { -200,200,0 };
		lesPng3_->scale_ = { 256,144,1 };

		//ライフ1
		havePng_->Inilialize(normalSpriteCommon_, &matProjection_);
		havePng_->position_ = { -680,-420,0 };
		havePng_->scale_ = { 256,144,1 };

		//ライフ2
		havePng2_->Inilialize(normalSpriteCommon_, &matProjection_);
		havePng2_->position_ = { -680,-420,0 };
		havePng2_->scale_ = { 256,144,1 };

		//ライフ3
		havePng3_->Inilialize(normalSpriteCommon_, &matProjection_);
		havePng3_->position_ = { -680,-420,0 };
		havePng3_->scale_ = { 256,144,1 };

		//LStick
		UILStick_->Inilialize(normalSpriteCommon_, &matProjection_);
		UILStick_->position_ = { -540,-280,0 };
		UILStick_->scale_ = { 240,120,1 };

		//RStick
		UIRStick_->Inilialize(normalSpriteCommon_, &matProjection_);
		UIRStick_->position_ = { -460,-320,0 };
		UIRStick_->scale_ = { 240,120,1 };

		//LT
		UILT_->Inilialize(normalSpriteCommon_, &matProjection_);
		UILT_->position_ = { -600,-230,0 };
		UILT_->scale_ = { 200,120,1 };

		//Abutton
		UIAButton_->Inilialize(normalSpriteCommon_, &matProjection_);
		UIAButton_->position_ = { 180,-230,0 };
		UIAButton_->scale_ = { 480,240,1 };

		//Press
		UIPress_->Inilialize(normalSpriteCommon_, &matProjection_);
		UIPress_->position_ = { -280,-230,0 };
		UIPress_->scale_ = { 600,240,1 };
	};
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
	screen_.Draw(blockTex_);

	//Actor描画
	player_->Draw(plyerTex_, plyerTex_);

	//
	goal_->Draw(white_);

	//ボックスパーティクル
	ParticleManager::GetInstance()->Draw();

	//jsonファイルから読み込んだものの描画
	LoadObjectData::GetInstance()->Draw();

	//スプライトのプレドロー
	sprite_->PreDraw();

	//操作(UI描画一つにまとめる)
	UILStick_->Draw(LTex_);
	UILStick_->Update();

	UIRStick_->Draw(RTex_);
	UIRStick_->Update();

	UILT_->Draw(LTTex_);
	UILT_->Update();

	//HP表示
	lifePng_->Draw(lifeTex_);
	lifePng_->Update();

	//描画用の最大HP(HitPoint用のクラスを作る)
	float maxHP = 3;

	//HP表示(3つまで)
		//(マジックナンバー直す)
	for (size_t i = 0; i < maxHP; i++)
	{
		if (i < (player_->GetLife()))
		{
			if (i == 0)
			{
				havePng_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
				havePng_->Update();
				havePng_->Draw(heartHaveTex_);
			}
			else if (i == 1)
			{
				havePng2_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
				havePng2_->Update();
				havePng2_->Draw(heartHaveTex_);
			}
			else
			{
				havePng3_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
				havePng3_->Update();
				havePng3_->Draw(heartHaveTex_);
			}
		}
		else
		{
			if (i == 0)
			{
				lesPng_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
				lesPng_->Update();
				lesPng_->Draw(heartLesTex_);
			}
			else if (i == 1)
			{
				lesPng2_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
				lesPng2_->Update();
				lesPng2_->Draw(heartLesTex_);
			}
			else
			{
				lesPng3_->position_ = { -430.0f + (i * 128),230.0f,0.0f };
				lesPng3_->Update();
				lesPng3_->Draw(heartLesTex_);
			}
		}
	}

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void PlayScene::Finalize()
{

	ParticleManager::GetInstance()->Finalize();
}