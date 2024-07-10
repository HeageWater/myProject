#include "PlayScene.h"
#include "imgui.h"
#include "ChengeScene.h"
#include "Enum.h"
#include "UiManager.h"

void PlayScene::Update()
{
#ifdef _DEBUG

	//ImGui受付開始
	ImguiManager::GetInstance()->Begin();
	float test1 = 0.5f;

	Vector2D con = controller_->GetInstance()->GetRightStickVec();

	ImGui::Text("test");
	ImGui::SliderFloat("Test", &test1, 0.01f, 0.99f);
	ImGui::SliderFloat("RstickX", &con.x_, 0.01f, 0.99f);
	ImGui::SliderFloat("RstickY", &con.y_, 0.01f, 0.99f);

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

	if (ImGui::Button("stage2"))
	{
		LoadObjectData::GetInstance()->StageLoad("stage2");

		//開始地点をセット
		player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

		//ゴール初期化
		goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

		stageCount_ = 1;
	}

	if (ImGui::Button("stage3"))
	{
		LoadObjectData::GetInstance()->StageLoad("stage3");

		//開始地点をセット
		player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

		//ゴール初期化
		goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

		stageCount_ = 2;
	}

	if (ImGui::Button("stage4"))
	{
		LoadObjectData::GetInstance()->StageLoad("stage4");

		//開始地点をセット
		player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

		//ゴール初期化
		goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

		stageCount_ = 3;
	}

	if (ImGui::Button("stageLIBLADE"))
	{
		LoadObjectData::GetInstance()->StageLoad("stageLIBLADE");

		//開始地点をセット
		player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

		//ゴール初期化
		goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

		stageCount_ = 4;
	}

	//+x
	if (ImGui::Button("+x"))
	{
		Vector3D nowPos = player_->GetPos();
		nowPos += {10, 0, 0};
		player_->SetPos(nowPos);
	}

	//ImGui受付終了
	ImguiManager::GetInstance()->End();

#endif _DEBUG

	//hitStop更新
	hitStop_->Update();

	//burakkuauto 
	blackOut_->Update();

	//暗転
	BlackOut();

	//まだ時間無いなら戻る
	if (hitStop_->GetTimeFlag() || blackOutFlag_)
	{
		return;
	}

	//ゴールに触れているか
	bool checkGoal = goal_->BoxCollision(player_->GetMat());

	//最初のムービー用タイマー
	if (time_ < HUNDRED)
	{
		time_++;
	}

	bool movieFlag_ = movieClearFlag_ || movieOverFlag_;

	if (!movieFlag_)
	{
		//
		player_->MoveY();
		//stageUpdate
		for (auto& object : LoadObjectData::GetInstance()->GetStage())
		{
			object->SetFlag(true);

			object->Update(matView_.mat_, matProjection_);

			if (player_->StageCollsionY(object->GetMat()))
			{

			}
		}

		//
		player_->MoveX();
		for (auto& object : LoadObjectData::GetInstance()->GetStage())
		{
			object->SetFlag(true);

			object->Update(matView_.mat_, matProjection_);

			if (player_->StageCollsionX(object->GetMat()))
			{

			}
		}

		//止まっている時間
		size_t setStopTime = 0;

		//プレイヤーと敵の判定
		for (auto& object : LoadObjectData::GetInstance()->GetEnemy())
		{
			object->SertchPlayer(player_->GetMat());

			if (object->GetDeadVec())
			{
				//ParticleManager::GetInstance()->CreateBoxParticle(object->GetPos());
				continue;
			}

			if (player_->PlayerCollision(object->GetMat()))
			{
				//敵が止まっている時間
				const size_t StopTime = 150;

				ParticleManager::GetInstance()->CreateBoxParticle(player_->GetPos());
				object->SetTime(StopTime);
				sound_->SoundPlayWave(hitSound_);
			}

			//playerの攻撃との判定
			for (auto& attack : player_->GetAttack())
			{
				if (object->BoxCollision(attack->GetMat()))
				{
					setStopTime += FIVE;
					sound_->SoundPlayWave(hitSound_);
					ParticleManager::GetInstance()->CreateBoxParticle(object->GetPos());

					Shake::GetInstance()->SetTime(30);
				}
			}
		}

		//保存したヒットストップの時間をセット
		hitStop_->SetTime((float)setStopTime);

		//パーティクルテスト
		if (input_->GetTrigger(DIK_1))
		{
			ParticleManager::GetInstance()->CreateBoxParticle(player_->GetPos());
		}
		if (input_->GetTrigger(DIK_2))
		{
			ParticleManager::GetInstance()->CreateCircleParticle(player_->GetPos());
		}



		//プレイヤーの先が見えるようにするボタン
		float camera = 25;
		float cameraX = (controller_->GetInstance()->ButtonKeepPush(B) * camera) - (controller_->GetInstance()->ButtonKeepPush(X) * camera);
		float cameraY = (controller_->GetInstance()->ButtonKeepPush(Y) * camera) - (controller_->GetInstance()->ButtonKeepPush(A) * camera);

		Vector2D leftVec= controller_->GetLeftStickVec();

		//カメラが先に行く
		if (leftVec.x_ > 0.7)
			moveCameraX++;
		else if (leftVec.x_ < -0.7)
			moveCameraX--;
		else
		{
			if (moveCameraX > 0)
				moveCameraX--;
			else if (moveCameraX < 0)
				moveCameraX++;
			else
				moveCameraX = 0;
		}

		//moveCameraY += 0;
		const float maxCamera = 15;

		if (moveCameraX > maxCamera)
			moveCameraX = maxCamera;
		if (moveCameraX < -maxCamera)
			moveCameraX = -maxCamera;

		//targetをplayerに
		matView_.eye_.x_ = player_->GetPos().x_ + Shake::GetInstance()->GetShake().x_ + cameraX + moveCameraX;
		matView_.target_.x_ = player_->GetPos().x_ + Shake::GetInstance()->GetShake().x_ + cameraX + moveCameraX;

		//playerのyからどれくらい離すか
		const float prusTargetY = 10;

		//カメラに揺れを追加
		matView_.eye_.y_ = player_->GetPos().y_ + prusTargetY + Shake::GetInstance()->GetShake().y_ + cameraY + moveCameraY;
		matView_.target_.y_ = player_->GetPos().y_ + prusTargetY + Shake::GetInstance()->GetShake().y_ + cameraY + moveCameraY;

		//player更新
		player_->Update(matView_.mat_, matProjection_, shader_);
		goal_->Update(matView_.mat_, matProjection_);

		//jsonファイルから読み込んだものの更新
		LoadObjectData::GetInstance()->SetCamera(matView_.mat_, matProjection_);
		LoadObjectData::GetInstance()->Update();

		//カメラ更新
		matView_.MatUpdate();

		//スクリーン更新
		screen_.MatUpdate(matView_.mat_, matProjection_, ZERO);

		//パーティクル更新
		ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
		ParticleManager::GetInstance()->Update();

		//揺れ更新
		Shake::GetInstance()->Update();

		//GameClear条件が達成されたら
		if (checkGoal && !blackOutFlag_)
		{
			//ステージの最大数より多いか
			if (MaxStageCount_ > stageCount_)
			{
				//ステージカウントプラス
				stageCount_++;

				//ブラックアウト
				blackOutFlag_ = true;
			}
			else
			{
				movieClearFlag_ = true;
			}
		}

		//GameOver条件が達成されたら
		if (player_->GetLife() <= (float)ZERO)
		{
			movieOverFlag_ = true;
			player_->SetDeadAnimation();
		}
	}
	else
	{
		//GameClear条件が達成されたら
		if (movieClearFlag_)
		{
			//ChengeScene::GetInstance()->SetPlayFlag("GAMECLEAR");
			ChengeScene::GetInstance()->SetPlayFlag("BOSS");
		}

		//GameOver条件が達成されたら
		if (player_->GetLife() <= 0)
		{
			//死亡アニメーション
			if (player_->DeadAnimation())
			{
				ChengeScene::GetInstance()->SetPlayFlag("GAMEOVER");
			}
		}
	}

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();

	//Uiマネージャー更新
	UiManager::GetInstance()->Update();
	UiManager::GetInstance()->SetLife(player_->GetLife());
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

	//カメラ初期化
	gameCamera_ = std::make_unique<GameCamera>();

	//player
	player_ = std::make_unique<Player>();
	player_->Initialize(shader_, pipeline_.get());
	Vector3D pos = player_->GetPos();
	pos += {0, 10, 0};
	player_->SetPos(pos);

	//jsonファイルから読み込んだものの初期化
	LoadObjectData::GetInstance()->SetModel(shader_, pipeline_.get());
	LoadObjectData::GetInstance()->Initialize();

	//ステージ読み込み
	LoadObjectData::GetInstance()->StageLoad("stage1");

	//開始地点をセット
	player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

	//
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(shader_, pipeline_.get());

	//ゴール初期化
	goal_->Initialize(MyDirectX::GetInstance(), shader_, pipeline_.get());
	goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

	//パーティクルマネージャー
	ParticleManager::GetInstance()->Initalize();
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->SetDraw(shader_, pipeline_.get());

	//白画像
	blockTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/back.png");
	plyerTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/Player/Player.png");
	whiteTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/white1x1.png");
	blackTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/black.png");

	//カウント
	time_ = 0;

	////透過するかどうか
	normalSpriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	//画像色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	//基礎
	sprite_->Inilialize(normalSpriteCommon_, &matProjection_);

	//
	movieOverFlag_ = false;
	movieClearFlag_ = false;

	//音読み込み
	hitSound_ = sound_->SoundLoadWave("Resources/sound/se_hit_008.wav");

	//scale用
	float size = 15.5f;

	//ブラックアウト
	blackOut_->Inilialize(normalSpriteCommon_, &matProjection_);
	blackOut_->position_ = { -680,-Window::window_height_,0 };
	blackOut_->scale_ = { Window::window_width_ * size,Window::window_height_ * size,1 };
	blackOut_->SetColor(color_);

	//画像色
	color_ = { 0.0f,0.0f,0.0f,0.0f };

	Shake::GetInstance()->Initalize();
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

	//
	skydome_->Draw();


	//Actor描画
	player_->Draw(plyerTex_);

	//ゴール描画
	goal_->Draw(white_);

	//ボックスパーティクル
	ParticleManager::GetInstance()->Draw();

	//jsonファイルから読み込んだものの描画
	LoadObjectData::GetInstance()->Draw();

	//Uiマネージャー描画
	UiManager::GetInstance()->Draw();

	//ブラックアウト
	blackOut_->SetColor(color_);
	blackOut_->Draw(blackTex_);

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

#ifdef _DEBUG

	//ImGui描画
	ImguiManager::GetInstance()->Draw();

#endif _DEBUG

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void PlayScene::Finalize()
{
	//パーティクル削除
	ParticleManager::GetInstance()->Finalize();
}

void PlayScene::BlackOut()
{
	//ブラックアウト
	if (blackOutFlag_)
	{
		//ブラックアウト
		if (color_.x_ < 1.0f)
		{
			color_.x_ += 0.01f;
			color_.y_ += 0.01f;
			color_.z_ += 0.01f;
			color_.w_ += 0.01f;

			if (color_.x_ >= 1.0f)
			{
				//ステージ読み込み
				if (stageCount_ == ONE)
				{
					LoadObjectData::GetInstance()->StageLoad("stage2");
				}
				else if (stageCount_ == TWO)
				{
					LoadObjectData::GetInstance()->StageLoad("stage3");
				}
				else if (stageCount_ == THREE)
				{
					LoadObjectData::GetInstance()->StageLoad("stage4");
				}
				else if (stageCount_ == FOUR)
				{
					LoadObjectData::GetInstance()->StageLoad("stageLIBLADE");
				}

				//開始地点をセット
				player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

				//ゴール初期化
				goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

				blackOutFlag_ = false;
			}
		}
	}
	else
	{
		//ブラックアウト
		if (color_.x_ > 0.0f)
		{
			color_.x_ -= 0.01f;
			color_.y_ -= 0.01f;
			color_.z_ -= 0.01f;
			color_.w_ -= 0.01f;
		}
	}
}