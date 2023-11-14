//#include "GameScene.h"
//
//int Time = 0;
//
//void GameScene::Update()
//{
//	FlameWork::Update();
//
//	//Update
//	//input->Update();
//
//	//ImGui受付開始
//	ImguiManager::GetInstance()->Begin();
//	ImGui::SliderFloat("Title posX", &titleObject->titleObj_.mat_.trans_.x_, -400, 400);
//	ImGui::SliderFloat("Title posY", &titleObject->titleObj_.mat_.trans_.y_, -400, 400);
//	ImGui::SliderFloat("Title posZ", &titleObject->titleObj_.mat_.trans_.z_, -400, 400);
//
//	ImGui::SliderFloat("Title rotX", &titleObject->titleObj_.mat_.rotAngle_.x_, -400, 400);
//	ImGui::SliderFloat("Title rotY", &titleObject->titleObj_.mat_.rotAngle_.y_, -400, 400);
//	ImGui::SliderFloat("Title rotZ", &titleObject->titleObj_.mat_.rotAngle_.z_, -400, 400);
//
//	ImGui::SliderFloat("playcamera posX", &playcamera.eye_.x_, -400, 400);
//	ImGui::SliderFloat("playcamera posY", &playcamera.eye_.y_, -400, 400);
//	ImGui::SliderFloat("playcamera posZ", &playcamera.eye_.z_, -400, 400);
//
//	//ImGui受付終了
//	ImguiManager::GetInstance()->End();
//
//	//sceneに改造
//	//ここからSceneの処理
//
//	//switch内で使う関数をここで定義
//	float chengeTime = 50;
//
//	//ステージホットリロード
//	StageReload();
//
//	//switch用変数
//
//	float t = 85;
//	float stopTime = 100;
//	switch (scene)
//	{
//	case Title:
//
//		if (chengeScene->GetTime() > chengeTime)
//		{
//			scene = Play;
//		}
//
//		//if (player->GetA())
//		if (titleObject->BoxCollision(player->GetAttackModel()) && !titleObject->IsMovie_)
//		{
//			titleObject->Movie();
//
//			//この下の処理まとめろ
//			float setStopTime = 7.0f;
//
//			hitStop->SetTime(setStopTime);
//			sound_->SoundPlayWave(enemyHit);
//		}
//
//		if (titleObject->EndMovie_)
//		{
//			//if (hitStop->GetTime() < 1)
//			{
//				if (chengeScene->GetTime() < 1)
//				{
//					titleObject->EndMovie_ = false;
//
//					//chengeScene->SetPlayFlag();
//				}
//			}
//		}
//
//		//player更新
//		player->Update(matView.mat_, matProjection, shader_);
//		titleObject->Update(matView.mat_, matProjection);
//
//		//
//		player->MoveY();
//		//stageUpdate
//		for (auto& object : objects_)
//		{
//			object->SetFlag(true);
//
//			object->Update(matView.mat_, matProjection);
//
////
//			//if (player->StageCollsionY(object->stage_, matView.mat, matProjection))
//			{
//				//object->SetFlag(false);
//			}
//		}
//
//		//
//		player->MoveX();
//		for (auto& object : objects_)
//		{
//			object->SetFlag(true);
//
//			object->Update(matView.mat_, matProjection);
//
//		//	if (player->StageCollsionX(object->stage_, matView.mat, matProjection))
//			{
//				//object->SetFlag(false);
//			}
//		}
//
//		//スクリーン更新
//		screen.MatUpdate(matView.mat_, matProjection, 0);
//
//		//matView.mat = playcamera.mat;
//
//		////座標更新
//		//playcamera.Update(*input);
//		//debugcamera.Update(*input);
//
//		//カメラ更新
//		matView.MatUpdate();
//
//		break;
//
//	case Select:
//		break;
//
//	case Play:
//
//		if (Time < t)
//		{
//			Time++;
//		}
//
//		//if (hitStop->GetTime() < 1 && Time == t)
//		{
//			//player更新
//			player->Update(matView.mat_, matProjection, shader_);
//
//			//enemy更新
//			enemy->Update(matView.mat_, matProjection);
//			enemy->SertchPlayer(player->GetModel());
//			if (player->PlayerCollision(enemy->enemy_))
//			{
//				CreatePatricle(player->GetPos());
//				enemy->Time_ = stopTime;
//				sound_->SoundPlayWave(playerHit);
//			}
//			bool sheikF = enemy->BoxCollision(player->GetAttackModel());
//
//			//この下の処理まとめろ
//			float setStopTime = 7.0f;
//
//			if (sheikF)
//			{
//				hitStop->SetTime(setStopTime);
//				sound_->SoundPlayWave(enemyHit);
//				CreatePatricle(enemy->GetPos());
//			}
//
//			//enemy更新
//			enemy2->Update(matView.mat_, matProjection);
//			enemy2->SertchPlayer(player->GetModel());
//			if (player->PlayerCollision(enemy2->enemy_))
//			{
//				CreatePatricle(player->GetPos());
//				enemy2->Time_ = stopTime;
//				sound_->SoundPlayWave(playerHit);
//			}
//			sheikF = enemy2->BoxCollision(player->GetAttackModel());
//
//			if (sheikF)
//			{
//				hitStop->SetTime(setStopTime);
//				sound_->SoundPlayWave(enemyHit);
//				CreatePatricle(enemy2->GetPos());
//			}
//
//			//enemy更新
//			enemy3->Update(matView.mat_, matProjection);
//			enemy3->SertchPlayer(player->GetModel());
//			if (player->PlayerCollision(enemy3->enemy_))
//			{
//				CreatePatricle(player->GetPos());
//				enemy3->Time_ = stopTime;
//				sound_->SoundPlayWave(playerHit);
//			}
//			sheikF = enemy3->BoxCollision(player->GetAttackModel());
//
//			if (sheikF)
//			{
//				hitStop->SetTime(setStopTime);
//				sound_->SoundPlayWave(enemyHit);
//				CreatePatricle(enemy3->GetPos());
//			}
//
//			//enemy更新
//			enemy4->Update(matView.mat_, matProjection);
//			enemy4->SertchPlayer(player->GetModel());
//			if (player->PlayerCollision(enemy4->enemy_))
//			{
//				CreatePatricle(player->GetPos());
//				enemy4->Time_ = stopTime;
//				sound_->SoundPlayWave(playerHit);
//			}
//			sheikF = enemy4->BoxCollision(player->GetAttackModel());
//
//			if (sheikF)
//			{
//				hitStop->SetTime(setStopTime);
//				sound_->SoundPlayWave(enemyHit);
//				CreatePatricle(enemy4->GetPos());
//			}
//
//			//warpとplayerのcollision
//			if (warp->BoxCollision(player->GetModel()))
//			{
//				/*player->knockBackVec = -2;
//				player->knockBackFlag = true;*/
//
//				//player->SetPos(warp->GetOutPos());
//				//player->warpActionFlag_ = true;
//				player->SetWarpPos(warp->GetInPos(), warp->GetOutPos());
//			}
//
//			uint32_t Two = 2;
//
//			if (player->GetWarpMode() == Two)
//			{
//				if (player->GetPos().x_ < warp->GetOutPos().x_)
//				{
//					matView.eye_.x_++;
//				}
//				else
//				{
//					player->SetWarpMode(3);
//				}
//			}
//
//			warp->CheckMode(player->GetWarpMode());
//
//			//ここまで
//
//			//ステージ更新
//			stage->Update(matView.mat_, matProjection);
//			goal->Update(matView.mat_, matProjection);
//			warp->Update(matView.mat_, matProjection);
//
//			//スクリーン更新
//			screen.MatUpdate(matView.mat_, matProjection, 0);
//
//			//Vector2D moveCamera = { 0,0 };
//
//			//moveCamera = player->GetController();
//
//			//
//			player->MoveY();
//			//stageUpdate
//			for (auto& object : objects_)
//			{
//				object->SetFlag(true);
//
//				object->Update(matView.mat_, matProjection);
//
//
//				if (player->StageCollsionY(object->stage_))
//				{
//					//object->SetFlag(false);
//				}
//			}
//
//			//
//			player->MoveX();
//			for (auto& object : objects_)
//			{
//				if (player->StageCollsionX(object->stage_))
//				{
//					//object->SetFlag(false);
//				}
//			}
//
//
//			//targetをplayerに
//			matView.eye_.x_ = player->GetPos().x_;
//			matView.target_.x_ = player->GetPos().x_;
//
//			matView.eye_.y_ = player->GetPos().y_;
//			matView.target_.y_ = player->GetPos().y_;
//
//			matView.mat_ = playcamera.mat_;
//
//			//座標更新
//			//playcamera.Update(*input);
//			//debugcamera.Update(*input);
//
//			//カメラ更新
//			matView.MatUpdate();
//
//			//stage->stage_.mat.trans.x = max(stage->stage_.mat.trans.x, minMapX);
//			bool checkGoal = goal->BoxCollision(player->GetModel());
//
//			if (checkGoal)
//			{
//				//chengeScene->SetPlayFlag();
//				goalFlag = true;
//			}
//
//			if (chengeScene->GetTime() > chengeTime && goalFlag)
//			{
//				scene = GameClear;
//
//				goalFlag = false;
//			}
//
//			if (player->GetLife() < 1)
//			{
//				//chengeScene->SetPlayFlag();
//				overFlag = true;
//
//				Reset();
//
//			}
//
//			if (chengeScene->GetTime() > chengeTime && overFlag)
//			{
//				scene = GameOver;
//
//				overFlag = false;
//			}
//		}
//		if (Time == 1)
//		{
//			Reset();
//			StageLoad("stage4");
//		}
//		break;
//
//	case Pause:
//		break;
//
//	case Movie:
//		break;
//
//	case GameClear:
//		//if (player->GetA())
//		//{
//		//	//chengeScene->SetPlayFlag();
//
//		//	scene = Title;
//
//		//	Reset();
//		//}
//		break;
//
//	case GameOver:
//
//		//if (player->GetA())
//		//{
//		//	//chengeScene->SetPlayFlag();
//
//		//	scene = Title;
//
//		//	Reset();
//		//}
//		break;
//
//	default:
//		break;
//	}
//
//	//ここまでSceneの処理
//
//	//Scene共通の処理
//	pressText.MatUpdate(Matrix(), spriteProjection);
//
//	hitStop->Update();
//
//	sprite_->Update();
//	titlePng->Update();
//
//	havePng->Update();
//	havePng2->Update();
//	havePng3->Update();
//	lesPng->Update();
//	lesPng2->Update();
//	lesPng3->Update();
//
//	lifePng->Update();
//
//	chengeScene->Update();
//
//	//パーティクル
//	for (size_t i = 0; i < boxParticles_.size(); i++)
//	{
//		boxParticles_[i]->Update(matView.mat_, matProjection);
//
//		//削除
//		if (boxParticles_[i]->IsDead() == true)
//		{
//			boxParticles_.erase(boxParticles_.begin() + i);
//		}
//	}
//
//	//Escapeで抜ける
//	if (input->GetTrigger(DIK_ESCAPE))
//	{
//		SetEndRwqust(true);
//	}
//}
//
//void GameScene::Initialize()
//{
//	FlameWork::Initialize();
//
//	//input
//	input = Input::GetInstance();
//
//	controller = Controller::GetInstance();
//
//	//camera
//	debugcamera.Init(Vector3D(0.0f, 30.0f, 10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
//	playcamera.Init(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
//
//	//screen
//	//screen.Initialize(dx.get(), multipathPipeline.get(), bilShader);
//	screen.Initialize(multipathPipeline_.get(), bilShader_);
//	screen.obj_.trans_.z_ = 100.1f;
//	screen.obj_.scale_ = { Window::window_width_ * 2,Window::window_height_ / 2,0.2f };
//
//	//sprite
//	spriteProjection = MyMath::OrthoLH(Window::window_width_, Window::window_height_, 0.0f, 1.0f);
//
//	//tex
//	pressText.Initialize(uiPipeline_.get(), spriteShader_);
//	pressText.obj_.trans_.y_ = -200;
//	pressText.obj_.scale_ = { Window::window_width_,Window::window_height_ ,0.2f };
//	pressText.MatUpdate(Matrix(), spriteProjection, 0);
//
//	//描画用行列
//	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
//
//	//sound
//	sound_ = MyXAudio::GetInstance();
//	bgm = sound_->SoundLoadWave("Resources/sound/BGM.wav");
//	fanfare = sound_->SoundLoadWave("Resources/sound/fanfare.wav");
//	playerHit = sound_->SoundLoadWave("Resources/sound/se_hit_005.wav");
//	enemyHit = sound_->SoundLoadWave("Resources/sound/se_hit_008.wav");
//
//	//player
//	player->Initialize(shader_, pipeline_.get());
//
//	//
//	titleObject->Initialize(shader_, pipeline_.get());
//
//	//warp
//	warp->Initialize(shader_, pipeline_.get());
//
//	float size = 3.0f;
//
//	//仮enemy置き
//	enemy->Initialize(shader_, pipeline_.get());
//	enemy->SetTrans(Vector3D{ 180,20,0 });
//	enemy->SetScale(Vector3D{ size,size,size });
//
//	enemy2->Initialize(shader_, pipeline_.get());
//	enemy2->SetTrans(Vector3D{ 150,40,0 });
//	enemy2->SetScale(Vector3D{ size,size,size });
//
//	enemy3->Initialize(shader_, pipeline_.get());
//	enemy3->SetTrans(Vector3D{ 200,30,0 });
//	enemy3->SetScale(Vector3D{ size,size,size });
//
//	enemy4->Initialize(shader_, pipeline_.get());
//	enemy4->SetTrans(Vector3D{ 300,50,0 });
//	enemy4->SetScale(Vector3D{ size,size,size });
//
//	//stage
//	//ステージ初期化
//	stage->Initialize(shader_, pipeline_.get());
//	float minMapX = stage->stage_.mat_.scale_.x_ - 200;
//	stage->stage_.mat_.trans_.x_ = minMapX;
//
//	stageWhite->Initialize(shader_, pipeline_.get());
//	stageWhite->stage_.mat_.trans_.y_ += 1;
//	stageWhite->stage_.mat_.scale_.z_ = 10;
//	stageWhite->Update(matView.mat_, matProjection);
//
//	//ゴール初期化
//	goal->Initialize(MyDirectX::GetInstance(), shader_, pipeline_.get());
//	//シーンフラグ
//	scene = Title;
//
//	//画像読み込み
//	white = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/white1x1.png");
//	texP = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/cube.jpg");
//	brPng = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");
//	clearTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameclear.png");
//	overTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameover.png");
//	titleTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Title.png");
//	playerTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/Player/Player.png");
//	enemyPng = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
//	blackTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/black.png");
//	heartLesTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartLes.png");
//	heartHaveTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartHave.png");
//	lifeTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/life.png");
//	backTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/background.png");
//	LTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Lstick.png");
//	RTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Rstick.png");
//	AbuttonTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Abutton.png");
//	PressTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/press.png");
//	LTTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/LT.png");
//
//	//透過するかどうか
//	semiArphaSpriteCommon->Inilialize(MyDirectX::GetInstance(), true);
//	normalSpriteCommon->Inilialize(MyDirectX::GetInstance(), false);
//
//	{
//		//基礎
//		sprite_->Inilialize(semiArphaSpriteCommon, &matProjection);
//
//		//タイトル
//		titlePng->Inilialize(normalSpriteCommon, &matProjection);
//		titlePng->position_ = { -680,-420,0 };
//		titlePng->scale_ = { 3600,1440,1 };
//
//		//ライフ英語
//		lifePng->Inilialize(normalSpriteCommon, &matProjection);
//		lifePng->position_ = { -590,240,0 };
//		lifePng->scale_ = { 360,144,1 };
//
//		//ライフ1
//		lesPng->Inilialize(normalSpriteCommon, &matProjection);
//		lesPng->position_ = { -200,200,0 };
//		lesPng->scale_ = { 256,144,1 };
//
//		//ライフ2
//		lesPng2->Inilialize(normalSpriteCommon, &matProjection);
//		lesPng2->position_ = { -200,200,0 };
//		lesPng2->scale_ = { 256,144,1 };
//
//		//ライフ3
//		lesPng3->Inilialize(normalSpriteCommon, &matProjection);
//		lesPng3->position_ = { -200,200,0 };
//		lesPng3->scale_ = { 256,144,1 };
//
//		//ライフ1
//		havePng->Inilialize(normalSpriteCommon, &matProjection);
//		havePng->position_ = { -680,-420,0 };
//		havePng->scale_ = { 256,144,1 };
//
//		//ライフ2
//		havePng2->Inilialize(normalSpriteCommon, &matProjection);
//		havePng2->position_ = { -680,-420,0 };
//		havePng2->scale_ = { 256,144,1 };
//
//		//ライフ3
//		havePng3->Inilialize(normalSpriteCommon, &matProjection);
//		havePng3->position_ = { -680,-420,0 };
//		havePng3->scale_ = { 256,144,1 };
//
//		//LStick
//		UILStick->Inilialize(normalSpriteCommon, &matProjection);
//		UILStick->position_ = { -540,-280,0 };
//		UILStick->scale_ = { 240,120,1 };
//
//		//RStick
//		UIRStick->Inilialize(normalSpriteCommon, &matProjection);
//		UIRStick->position_ = { -460,-320,0 };
//		UIRStick->scale_ = { 240,120,1 };
//
//		//LT
//		UILT->Inilialize(normalSpriteCommon, &matProjection);
//		UILT->position_ = { -600,-230,0 };
//		UILT->scale_ = { 200,120,1 };
//
//		//Abutton
//		UIAButton->Inilialize(normalSpriteCommon, &matProjection);
//		UIAButton->position_ = { 180,-230,0 };
//		UIAButton->scale_ = { 480,240,1 };
//
//		//Press
//		UIPress->Inilialize(normalSpriteCommon, &matProjection);
//		UIPress->position_ = { -280,-230,0 };
//		UIPress->scale_ = { 600,240,1 };
//
//	};
//
//	goalFlag = false;
//
//	//ステージ読み込み
//	//StageLoad("stage4");
//	StageLoad("TitleStage");
//
//	chengeScene->Initialize(matProjection);
//
//	//音を鳴らす
//	sound_->SoundPlayLoopWave(bgm);
//}
//
//void GameScene::Draw()
//{
//	//Draw
//	MyDirectX::GetInstance()->PrevDrawScreen();
//
//	//// 描画コマンド
//
//	MyDirectX::GetInstance()->PostDrawScreen();
//
//	//UIDraw
//	MyDirectX::GetInstance()->PrevDraw();
//
//	//ここから共通先描画
//
//	//スクリーン描画
//	screen.Draw(backTex);
//	size_t texCount = 19;
//	//ここまで共通先描画
//
//	//ここから3D描画
//	switch (scene)
//	{
//	case Title:
//		//Actor描画
//		player->Draw(playerTex, white);
//
//		titleObject->Draw(white);
//
//		//ステージ
//		for (auto& object : objects_) {
//			//object->Draw(texP);
//			object->Draw(texCount);
//		}
//		break;
//
//	case Select:
//		break;
//
//	case Play:
//
//		//Actor描画
//		player->Draw(playerTex, white);
//
//		/*if (enemy->Time_ == 0)
//		{
//			enemy->Draw(enemyPng);
//		}
//		else
//		{
//			enemy->Draw(white);
//		}
//
//		if (enemy2->Time_ == 0)
//		{
//			enemy2->Draw(enemyPng);
//		}
//		else
//		{
//			enemy2->Draw(white);
//		}
//
//		if (enemy3->Time_ == 0)
//		{
//			enemy3->Draw(enemyPng);
//		}
//		else
//		{
//			enemy3->Draw(white);
//		}
//
//		if (enemy4->Time_ == 0)
//		{
//			enemy4->Draw(enemyPng);
//		}
//		else
//		{
//			enemy4->Draw(white);
//		}*/
//
//		warp->Draw(texP);
//
//		//stage->Draw(brPng);
//		//stageWhite->Draw(white);
//		goal->Draw(white);
//
//		//ステージ
//		for (auto& object : objects_) {
//			//object->Draw(texP);
//			object->Draw(texCount);
//		}
//
//		//ボックスパーティクル
//		for (size_t i = 0; i < boxParticles_.size(); i++)
//		{
//			boxParticles_[i]->Draw(white);
//		}
//
//		break;
//
//	case Pause:
//		break;
//
//	case Movie:
//		break;
//
//	case GameClear:
//		break;
//
//	case GameOver:
//		break;
//
//	default:
//		break;
//	}
//	//ここまで3D描画
//
//	//ここから2D描画
//
//	//スプライトのプレドロー
//	sprite_->PreDraw();
//
//	//描画用の最大HP(HitPoint用のクラスを作る)
//	float maxHP = 3;
//
//	switch (scene)
//	{
//	case Title:
//		//タイトル
//		//titlePng->Draw(titleTex);
//		//sprite_->Draw();
//
//		//UIAButton->Draw(AbuttonTex);
//		//UIAButton->Update();
//
//		//UIPress->Draw(PressTex);
//		//UIPress->Update();
//
//		//操作(UI描画一つにまとめる)
//		UILStick->Draw(LTex);
//		UILStick->Update();
//
//		UIRStick->Draw(RTex);
//		UIRStick->Update();
//		break;
//
//	case Select:
//		break;
//
//	case Play:
//
//		//操作(UI描画一つにまとめる)
//		UILStick->Draw(LTex);
//		UILStick->Update();
//
//		UIRStick->Draw(RTex);
//		UIRStick->Update();
//
//		UILT->Draw(LTTex);
//		UILT->Update();
//
//		//HP表示
//		lifePng->Draw(lifeTex);
//
//		//HP表示(3つまで)
//		//(マジックナンバー直す)
//		for (size_t i = 0; i < maxHP; i++)
//		{
//			if (i < (player->GetLife()))
//			{
//				if (i == 0)
//				{
//					havePng->position_ = { -430.0f + (i * 128),230.0f,0.0f };
//					havePng->Update();
//					havePng->Draw(heartHaveTex);
//				}
//				else if (i == 1)
//				{
//					havePng2->position_ = { -430.0f + (i * 128),230.0f,0.0f };
//					havePng2->Update();
//					havePng2->Draw(heartHaveTex);
//				}
//				else
//				{
//					havePng3->position_ = { -430.0f + (i * 128),230.0f,0.0f };
//					havePng3->Update();
//					havePng3->Draw(heartHaveTex);
//				}
//			}
//			else
//			{
//				if (i == 0)
//				{
//					lesPng->position_ = { -430.0f + (i * 128),230.0f,0.0f };
//					lesPng->Update();
//					lesPng->Draw(heartLesTex);
//				}
//				else if (i == 1)
//				{
//					lesPng2->position_ = { -430.0f + (i * 128),230.0f,0.0f };
//					lesPng2->Update();
//					lesPng2->Draw(heartLesTex);
//				}
//				else
//				{
//					lesPng3->position_ = { -430.0f + (i * 128),230.0f,0.0f };
//					lesPng3->Update();
//					lesPng3->Draw(heartLesTex);
//				}
//			}
//		}
//
//		break;
//
//	case Pause:
//		break;
//
//	case Movie:
//		break;
//
//	case GameClear:
//		//sprite_->Draw(clearTex);
//		titlePng->Draw(clearTex);
//		break;
//
//	case GameOver:
//		titlePng->Draw(overTex);
//		break;
//
//	default:
//		break;
//	}
//
//	//シーンチェンジ用描画
//	chengeScene->Draw();
//
//	//ここまで2D描画
//	//ImguiManager::GetInstance()->Draw();
//
//	MyDirectX::GetInstance()->PostDraw();
//}
//
////delete
//void GameScene::Finalize()
//{
//	for (auto& object : objects_)
//	{
//		delete object;
//	}
//
//	FlameWork::Finalize();
//}
//
////一連の流れ
//void GameScene::Run()
//{
//	//初期化
//	Initialize();
//
//	//ゲームループ
//	while (true)
//	{
//		//更新
//		Update();
//
//		//描画
//		Draw();
//
//		//もしエンドフラグがTrueなら抜ける
//		if (IsEndRequst())
//		{
//			break;
//		}
//	}
//
//	//終了処理
//	Finalize();
//}
//
////リセット
//void GameScene::Reset()
//{
//	screen.obj_.trans_.z_ = 100.1f;
//	screen.obj_.scale_ = { Window::window_width_ * 2,Window::window_height_ / 2,0.2f };
//	goalFlag = false;
//
//	//tex
//	pressText.obj_.trans_.y_ = -200;
//	pressText.obj_.scale_ = { Window::window_width_,Window::window_height_ ,0.2f };
//	pressText.MatUpdate(Matrix(), spriteProjection, 0);
//
//	//描画用行列
//	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
//
//	//player
//	player->Reset();
//
//	//仮enemy置き
//	//今は決め打ち
//	//Bkenderで設定できるように
//	enemy->SetTrans(Vector3D{ 180,20,0 });
//	enemy->SetScale(Vector3D{ 1,1,1 });
//	enemy->isDead_ = false;
//
//	enemy2->SetTrans(Vector3D{ 150,40,0 });
//	enemy2->SetScale(Vector3D{ 1,1,1 });
//	enemy2->isDead_ = false;
//
//	enemy3->SetTrans(Vector3D{ 200,30,0 });
//	enemy3->SetScale(Vector3D{ 1,1,1 });
//	enemy3->isDead_ = false;
//
//	enemy4->SetTrans(Vector3D{ 300,50,0 });
//	enemy4->SetScale(Vector3D{ 1,1,1 });
//	enemy4->isDead_ = false;
//	//stage
//	//ステージ初期化
//	float minMapX = stage->stage_.mat_.scale_.x_ - 200;
//	stage->stage_.mat_.trans_.x_ = minMapX;
//
//	stageWhite->stage_.mat_.trans_.y_ += 1;
//	stageWhite->stage_.mat_.scale_.z_ = 10;
//	stageWhite->Update(matView.mat_, matProjection);
//
//	//ゴール初期化
//	goal->Reset();
//
//	//シーンフラグ
//	scene = Title;
//}
//
////ホットリロード
//void GameScene::StageReload()
//{
//	//デバッグ用
//	bool plessZero = input->GetTrigger(DIK_0);
//	bool plessNine = input->GetTrigger(DIK_9);
//	bool plessEight = input->GetTrigger(DIK_8);
//	bool plessSeven = input->GetTrigger(DIK_7);
//	bool plessSix = input->GetTrigger(DIK_6);
//	bool plessFive = input->GetTrigger(DIK_5);
//
//	bool ChengeFlag = plessZero || plessNine || plessEight || plessSeven || plessSix || plessFive;
//
//	if (ChengeFlag)
//	{
//		size_t count = objects_.size();
//
//		//今あるステージを削除
//		for (size_t i = 0; i < count; i++)
//		{
//			objects_.erase(objects_.begin());
//		}
//
//		//0ならuntitled
//		if (plessZero)
//		{
//			levelData_ = JsonFileOpen::FileOpen("untitled");
//		}
//
//		//9ならTest01
//		if (plessNine)
//		{
//			levelData_ = JsonFileOpen::FileOpen("Test01");
//		}
//
//		//8ならstage2
//		if (plessEight)
//		{
//			levelData_ = JsonFileOpen::FileOpen("stage2");
//		}
//
//		//7ならstage2test
//		if (plessSeven)
//		{
//			levelData_ = JsonFileOpen::FileOpen("stage2Test");
//		}
//
//		//6ならstage3
//		if (plessSix)
//		{
//			levelData_ = JsonFileOpen::FileOpen("stage3");
//		}
//
//		//5ならstage4
//		if (plessFive)
//		{
//			levelData_ = JsonFileOpen::FileOpen("stage4");
//		}
//
//		//ホットリロードでStageSelectごとに読み込むようにする
//		//レベルデータからオブジェクトに生成、配置
//		for (auto& objectdata : levelData_->objects_)
//		{
//			//ファイル名から登録済みモデルを検索
//			Stage* model_ = nullptr;
//			decltype(stages_)::iterator it = stages_.find(objectdata.fileName_);
//
//			//終わりか
//			if (it != stages_.end())
//			{
//				model_ = it->second;
//			}
//
//			//モデルを指定して3Dオブジェクトを生成
//			Stage* newModel_ = new Stage();
//			newModel_->Initialize(shader_, pipeline_.get());
//
//			//調整
//			float scale = 10.0f;
//
//			//trans
//			newModel_->stage_.mat_.trans_ = objectdata.translation_ * scale;
//
//			//rotationboxParticles_
//			newModel_->stage_.mat_.rotAngle_ = objectdata.rotation_;
//
//			//scale;
//			newModel_->stage_.mat_.scale_ = objectdata.scaling_ * scale;
//
//			//Update
//			newModel_->Update(matView.mat_, matProjection);
//
//			//格納
//			objects_.push_back(newModel_);
//		}
//	}
//}
//
////ランダムの数パーティクルを出す
//void GameScene::CreatePatricle(Vector3D pos)
//{
//	size_t minRange = 10;
//	size_t maxRange = 30;
//
//	size_t play = MyMath::GetRandom(minRange, maxRange);
//
//	for (size_t i = 0; i < play; i++)
//	{
//		BoxParticle* newP = new BoxParticle();
//
//		newP->Initialize(shader_, pipeline_.get());
//
//		newP->SetPos(pos);
//
//		boxParticles_.push_back(newP);
//	}
//}
//
////ファイル名を入れるとそれを読み込む
//void GameScene::StageLoad(const std::string& filePath)
//{
//	//stageファイル
//	levelData_ = JsonFileOpen::FileOpen(filePath);
//
//	//
//	size_t count = objects_.size();
//
//	//今あるステージを削除
//	for (size_t i = 0; i < count; i++)
//	{
//		objects_.erase(objects_.begin());
//	}
//
//	//ホットリロードでStageSelectごとに読み込むようにする
//	//レベルデータからオブジェクトに生成、配置
//	for (auto& objectdata : levelData_->objects_)
//	{
//		//ファイル名から登録済みモデルを検索
//		Stage* model_ = nullptr;
//		decltype(stages_)::iterator it = stages_.find(objectdata.fileName_);
//
//		//終わりか
//		if (it != stages_.end())
//		{
//			model_ = it->second;
//		}
//
//		//モデルを指定して3Dオブジェクトを生成
//		Stage* newModel_ = new Stage();
//		newModel_->Initialize(shader_, pipeline_.get());
//
//		//調整
//		float scale = 10.0f;
//
//		//trans
//		newModel_->stage_.mat_.trans_ = objectdata.translation_ * scale;
//
//		//rotation
//		newModel_->stage_.mat_.rotAngle_ = objectdata.rotation_;
//
//		//scale;
//		newModel_->stage_.mat_.scale_ = objectdata.scaling_ * scale;
//
//		//Update
//		newModel_->Update(matView.mat_, matProjection);
//
//		//格納
//		objects_.push_back(newModel_);
//	}
//}
