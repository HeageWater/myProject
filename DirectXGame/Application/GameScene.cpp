#include "GameScene.h"

int Time = 0;

void GameScene::Update()
{
	FlameWork::Update();

	//Update
	input->Update();

	if (input->GetTrigger(DIK_O))
	{
		imguiDrawFlag = !imguiDrawFlag;
	}

	//imgui
	imgui->Begin();

	if (ImGui::TreeNode("group 1"))
	{
		float x = player->GetPos().x;
		float y = player->GetPos().y;
		float nowLife = player->GetLife();
		bool a = true;

		ImGui::Text("player pos");
		ImGui::SliderFloat("player pos.x", &UIPress->position.x, -1000.0f, 1000.0f, "%f");
		ImGui::SliderFloat("player pos.y", &UIPress->position.y, -1000.0f, 1000.0f, "%f");
		ImGui::Text("DebugCamera");
		ImGui::Checkbox("DebugCamera", &a);
		//ImGui::InputFloat("x", &x, 0.0f, 1000.0f, "%f");
		ImGui::SliderFloat("camera.eye.x", &matView.eye.x, -1000.0f, 1000.0f, "%f");
		//ImGui::SliderFloat("y", &y, 0.0f, 1000.0f, "%f");
		ImGui::SliderFloat("lesPng->position.y", &lesPng->position.y, -1000.0f, 1000.0f, "%f");
		ImGui::SliderFloat("Life", &nowLife, 0.0f, 1000.0f, "%f");

		ImGui::Text("O Key : ImGuiDrawFlag");
		ImGui::Checkbox("ImGuiDraw", &imguiDrawFlag);

		ImGui::Text("P Key : Reset");

		ImGui::Text("NextScene : SpaceKey or A Button");
		ImGui::Text("LStick : Move");
		ImGui::Text("RStick : Attack");
		ImGui::Text("LT : Jump");

		if (ImGui::Button("Title"))
		{
			scene = Title;
		}

		if (ImGui::Button("Play"))
		{
			scene = Play;
		}

		if (ImGui::Button("GameClear"))
		{
			scene = GameClear;
		}

		if (ImGui::Button("GameOver"))
		{
			scene = GameOver;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("stageObjScale"))
	{
		for (auto& object : objects_)
		{
			ImGui::SliderFloat("objrotAngleX", &object->stage_.mat.rotAngle.x, 0.0f, 5.0f, "%f");

			ImGui::SliderFloat("objrotAngleY", &object->stage_.mat.rotAngle.y, 0.0f, 5.0f, "%f");

			ImGui::SliderFloat("objrotAngleZ", &object->stage_.mat.rotAngle.z, 0.0f, 5.0f, "%f");
		}

		ImGui::TreePop();
	}

	//ImGuiここまで
	imgui->End();

	//sceneに改造
	//ここからSceneの処理

	//switch内で使う関数をここで定義
	float chengeTime = 50;

	if (input->GetTrigger(DIK_P))
	{
		scene = Title;
		Reset();
	}

	//ステージホットリロード
	StageReload();

	//switch用変数

	float t = 85;
	float stopTime = 100;
	switch (scene)
	{
	case Title:

		if (chengeScene->GetTime() > chengeTime)
		{
			scene = Play;
		}

		if (input->GetTrigger(DIK_SPACE) || player->GetA())
		{
			chengeScene->SetPlayFlag();
		}
		break;

	case Select:
		break;

	case Play:

		if (Time < t)
		{
			Time++;
		}

		if (hitStop->GetTime() < 1 && Time == t)
		{
			//player更新
			player->Update(matView.mat, matProjection, dx.get(), shader, pipeline.get());

			//player->player_.mat.trans.x += (input->GetKey(DIK_D) - input->GetKey(DIK_A));
			//player->player_.mat.trans.y += (input->GetKey(DIK_W) - input->GetKey(DIK_S));

			if (input->GetTrigger(DIK_B))
			{
				//player->LesLife();
				player->SetLife(4);
			}

			//enemy更新
			enemy->Update(matView.mat, matProjection);
			enemy->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy->Time = stopTime;
			}
			bool sheikF = enemy->BoxCollision(player->GetAttackModel());

			//この下の処理まとめろ
			float setStopTime = 7.0f;

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				CreatePatricle(enemy->GetPos());
			}

			//enemy更新
			enemy2->Update(matView.mat, matProjection);
			enemy2->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy2->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy2->Time = stopTime;
			}
			sheikF = enemy2->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				CreatePatricle(enemy2->GetPos());
			}

			//enemy更新
			enemy3->Update(matView.mat, matProjection);
			enemy3->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy3->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy3->Time = stopTime;
			}
			sheikF = enemy3->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				CreatePatricle(enemy3->GetPos());
			}

			//enemy更新
			enemy4->Update(matView.mat, matProjection);
			enemy4->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy4->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy4->Time = stopTime;
			}
			sheikF = enemy4->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				CreatePatricle(enemy4->GetPos());
			}

			//warpとplayerのcollision
			if (warp->BoxCollision(player->GetModel()))
			{
				/*player->knockBackVec = -2;
				player->knockBackFlag = true;*/

				//player->SetPos(warp->GetOutPos());
				player->warpActionFlag = true;
				player->SetWarpPos(warp->GetInPos(), warp->GetOutPos());
			}

			uint32_t Two = 2;

			if (player->GetWarpMode() == Two)
			{
				if (player->GetPos().x < warp->GetOutPos().x)
				{
					matView.eye.x++;
				}
				else
				{
					player->SetWarpMode(3);
				}
			}

			warp->CheckMode(player->GetWarpMode());

			//ここまで

			//ステージ更新
			stage->Update(matView.mat, matProjection);
			goal->Update(matView.mat, matProjection);
			warp->Update(matView.mat, matProjection);

			//bool hit = player->CollisionAttackToEnemy(enemy->enemy_);

			//パーティクル
			for (size_t i = 0; i < boxParticles_.size(); i++)
			{
				boxParticles_[i]->Update(matView.mat, matProjection);

				//削除
				if (boxParticles_[i]->IsDead() == true)
				{
					boxParticles_.erase(boxParticles_.begin() + i);
				}
			}

			//スクリーン更新
			screen.MatUpdate(matView.mat, matProjection, 0);

			//Vector2D moveCamera = { 0,0 };

			//moveCamera = player->GetController();

			//
			player->MoveY();
			//stageUpdate
			for (auto& object : objects_)
			{
				object->SetFlag(true);

				object->Update(matView.mat, matProjection);


				if (player->StageCollsionY(object->stage_, matView.mat, matProjection))
				{
					//object->SetFlag(false);
				}
			}

			//
			player->MoveX();
			for (auto& object : objects_)
			{
				if (player->StageCollsionX(object->stage_, matView.mat, matProjection))
				{
					//object->SetFlag(false);
				}
			}


			//targetをplayerに
			matView.eye.x = player->GetPos().x;// +(input->GetKey(DIK_C) * 90);
			matView.target.x = player->GetPos().x;// +(input->GetKey(DIK_C) * 90);

			//debugcamera.eye.x += input->GetKey(DIK_RIGHT) - input->GetKey(DIK_LEFT);
			//debugcamera.target.x += input->GetKey(DIK_RIGHT) - input->GetKey(DIK_LEFT);

			//float range = 50;

			matView.eye.y = player->GetPos().y; //moveCamera.x;
			matView.target.y = player->GetPos().y;// -dev[0];part

			//debugcamera.eye.y += input->GetKey(DIK_UP) - input->GetKey(DIK_DOWN);
		//	debugcamera.target.y += input->GetKey(DIK_UP) - input->GetKey(DIK_DOWN);


			matView.mat = playcamera.mat;

			if (input->GetKey(DIK_1))
			{
				matView.mat = debugcamera.mat;
			}

			//座標更新
			playcamera.Update(*input);
			debugcamera.Update(*input);

			matView.MatUpdate();


			//stage->stage_.mat.trans.x = max(stage->stage_.mat.trans.x, minMapX);
			bool checkGoal = goal->BoxCollision(player->GetModel());

			if (checkGoal)
			{
				chengeScene->SetPlayFlag();
				goalFlag = true;
			}

			if (chengeScene->GetTime() > chengeTime && goalFlag)
			{
				scene = GameClear;

				goalFlag = false;
			}

			if (player->GetLife() < 1)
			{
				chengeScene->SetPlayFlag();
				overFlag = true;
			}

			if (chengeScene->GetTime() > chengeTime && overFlag)
			{
				scene = GameOver;

				overFlag = false;
			}
		}

		break;

	case Pause:
		break;

	case Movie:
		break;

	case GameClear:
		if (input->GetTrigger(DIK_SPACE) || player->GetA())
		{
			chengeScene->SetPlayFlag();

			scene = Title;

			Reset();
		}
		break;

	case GameOver:

		if (input->GetTrigger(DIK_SPACE) || player->GetA())
		{
			chengeScene->SetPlayFlag();

			scene = Title;

			Reset();
		}
		break;

	default:
		break;
	}

	//ここまでSceneの処理

	//Scene共通の処理
	pressText.MatUpdate(Matrix(), spriteProjection);

	hitStop->Update();

	sprite_->Update();
	titlePng->Update();

	havePng->Update();
	havePng2->Update();
	havePng3->Update();
	lesPng->Update();
	lesPng2->Update();
	lesPng3->Update();

	lifePng->Update();

	chengeScene->Update(matView.mat, spriteProjection);

	//Escapeで抜ける
	if (input->GetTrigger(DIK_ESCAPE))
	{
		SetEndRwqust(true);
	}
}

void GameScene::Initilize()
{
	FlameWork::Initilize();

	//input
	input = std::make_unique<Input>(win.get());

	controller = Controller::GetInstance();

	//camera
	debugcamera.Init(Vector3D(0.0f, 30.0f, 10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	playcamera.Init(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//screen
	screen.Initialize(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };

	//sprite
	spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);

	//tex
	pressText.Initialize(dx.get(), uiPipeline.get(), spriteShader);
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	//描画用行列
	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//sound
	sound_ = MyXAudio::Get();
	bgm = sound_->SoundLoadWave("Resources/sound/BGM.wav");
	fanfare = sound_->SoundLoadWave("Resources/sound/fanfare.wav");

	//player
	player->Initialize(dx.get(), shader, pipeline.get());

	//warp
	warp->Initialize(dx.get(), shader, pipeline.get());

	float size = 3.0f;

	//仮enemy置き
	//今は決め打ち
	//Bkenderで設定できるように+9	
	enemy->Initialize(dx.get(), shader, pipeline.get());
	enemy->SetTrans(Vector3D{ 180,20,0 });
	enemy->SetScale(Vector3D{ size,size,size });

	enemy2->Initialize(dx.get(), shader, pipeline.get());
	enemy2->SetTrans(Vector3D{ 150,40,0 });
	enemy2->SetScale(Vector3D{ size,size,size });

	enemy3->Initialize(dx.get(), shader, pipeline.get());
	enemy3->SetTrans(Vector3D{ 200,30,0 });
	enemy3->SetScale(Vector3D{ size,size,size });

	enemy4->Initialize(dx.get(), shader, pipeline.get());
	enemy4->SetTrans(Vector3D{ 300,50,0 });
	enemy4->SetScale(Vector3D{ size,size,size });

	//stage
	//ステージ初期化
	stage->Initialize(dx.get(), shader, pipeline.get());
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	stageWhite->Initialize(dx.get(), shader, pipeline.get());
	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection);

	//ゴール初期化
	goal->Initialize(dx.get(), shader, pipeline.get());
	//シーンフラグ
	scene = Title;

	//画像読み込み
	white = dx->LoadTextureGraph(L"Resources/sprite/white1x1.png");
	texP = dx->LoadTextureGraph(L"Resources/sprite/cube.jpg");
	brPng = dx->LoadTextureGraph(L"Resources/sprite/br.png");
	clearTex = dx->LoadTextureGraph(L"Resources/sprite/gameclear.png");
	overTex = dx->LoadTextureGraph(L"Resources/sprite/gameover.png");
	titleTex = dx->LoadTextureGraph(L"Resources/sprite/Title.png");
	playerTex = dx->LoadTextureGraph(L"Resources/Model/Player/Player.png");
	enemyPng = dx->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
	blackTex = dx->LoadTextureGraph(L"Resources/sprite/black.png");
	heartLesTex = dx->LoadTextureGraph(L"Resources/sprite/heartLes.png");
	heartHaveTex = dx->LoadTextureGraph(L"Resources/sprite/heartHave.png");
	lifeTex = dx->LoadTextureGraph(L"Resources/sprite/life.png");
	backTex = dx->LoadTextureGraph(L"Resources/sprite/background.png");
	LTex = dx->LoadTextureGraph(L"Resources/sprite/Lstick.png");
	RTex = dx->LoadTextureGraph(L"Resources/sprite/Rstick.png");
	AbuttonTex = dx->LoadTextureGraph(L"Resources/sprite/Abutton.png");
	PressTex = dx->LoadTextureGraph(L"Resources/sprite/press.png");
	LTTex = dx->LoadTextureGraph(L"Resources/sprite/LT.png");
	//sprite_->LoadFile(L"Resources/Model/ene/enemy.png");

	//imgui初期化
	imgui->Initialize(dx.get());

	//透過するかどうか
	semiArphaSpriteCommon->Inilialize(dx.get(), true);
	normalSpriteCommon->Inilialize(dx.get(), false);

	//基礎
	sprite_->Inilialize(semiArphaSpriteCommon, &matProjection);

	//タイトル
	titlePng->Inilialize(normalSpriteCommon, &matProjection);
	titlePng->position = { -680,-420,0 };
	titlePng->scale = { 3600,1440,1 };

	//ライフ英語
	lifePng->Inilialize(normalSpriteCommon, &matProjection);
	lifePng->position = { -590,240,0 };
	lifePng->scale = { 360,144,1 };

	//ライフ1
	lesPng->Inilialize(normalSpriteCommon, &matProjection);
	lesPng->position = { -200,200,0 };
	lesPng->scale = { 256,144,1 };

	//ライフ2
	lesPng2->Inilialize(normalSpriteCommon, &matProjection);
	lesPng2->position = { -200,200,0 };
	lesPng2->scale = { 256,144,1 };

	//ライフ3
	lesPng3->Inilialize(normalSpriteCommon, &matProjection);
	lesPng3->position = { -200,200,0 };
	lesPng3->scale = { 256,144,1 };

	//ライフ1
	havePng->Inilialize(normalSpriteCommon, &matProjection);
	havePng->position = { -680,-420,0 };
	havePng->scale = { 256,144,1 };

	//ライフ2
	havePng2->Inilialize(normalSpriteCommon, &matProjection);
	havePng2->position = { -680,-420,0 };
	havePng2->scale = { 256,144,1 };

	//ライフ3
	havePng3->Inilialize(normalSpriteCommon, &matProjection);
	havePng3->position = { -680,-420,0 };
	havePng3->scale = { 256,144,1 };

	//LStick
	UILStick->Inilialize(normalSpriteCommon, &matProjection);
	UILStick->position = { -540,-280,0 };
	UILStick->scale = { 240,120,1 };

	//RStick
	UIRStick->Inilialize(normalSpriteCommon, &matProjection);
	UIRStick->position = { -460,-320,0 };
	UIRStick->scale = { 240,120,1 };

	//LT
	UILT->Inilialize(normalSpriteCommon, &matProjection);
	UILT->position = { -600,-230,0 };
	UILT->scale = { 200,120,1 };

	//Abutton
	UIAButton->Inilialize(normalSpriteCommon, &matProjection);
	UIAButton->position = { 180,-230,0 };
	UIAButton->scale = { 480,240,1 };

	//Press
	UIPress->Inilialize(normalSpriteCommon, &matProjection);
	UIPress->position = { -280,-230,0 };
	UIPress->scale = { 600,240,1 };

	goalFlag = false;
	//stageファイル
	levelData_ = JsonFileOpen::FileOpen("stage4");

	//ホットリロードでStageSelectごとに読み込むようにする
	//レベルデータからオブジェクトに生成、配置
	for (auto& objectdata : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Stage* model_ = nullptr;
		decltype(stages_)::iterator it = stages_.find(objectdata.fileName);

		//終わりか
		if (it != stages_.end())
		{
			model_ = it->second;
		}

		//モデルを指定して3Dオブジェクトを生成
		Stage* newModel_ = new Stage();
		newModel_->Initialize(dx.get(), shader, pipeline.get());

		//調整
		float scale = 10.0f;

		//trans
		newModel_->stage_.mat.trans = objectdata.translation * scale;

		//rotation
		newModel_->stage_.mat.rotAngle = objectdata.rotation;

		//scale;
		newModel_->stage_.mat.scale = objectdata.scaling * scale;

		//Update
		newModel_->Update(matView.mat, matProjection);

		//格納
		objects_.push_back(newModel_);
	}

	chengeScene->Initialize(dx.get(), pipeline.get(), matProjection);

	//音を鳴らす
	sound_->SoundPlayLoopWave(bgm);
}

void GameScene::Draw()
{
	//Draw
	dx->PrevDrawScreen();

	//// 描画コマンド

	dx->PostDrawScreen();

	//UIDraw
	dx->PrevDraw();

	//ここから共通先描画

	//スクリーン描画
	screen.Draw(backTex);
	size_t texCount = 19;
	//ここまで共通先描画

	//ここから3D描画
	switch (scene)
	{
	case Title:
		break;

	case Select:
		break;

	case Play:

		//Actor描画
		player->Draw(playerTex, white);

		if (enemy->Time == 0)
		{
			enemy->Draw(enemyPng);
		}
		else
		{
			enemy->Draw(white);
		}

		if (enemy2->Time == 0)
		{
			enemy2->Draw(enemyPng);
		}
		else
		{
			enemy2->Draw(white);
		}

		if (enemy3->Time == 0)
		{
			enemy3->Draw(enemyPng);
		}
		else
		{
			enemy3->Draw(white);
		}

		if (enemy4->Time == 0)
		{
			enemy4->Draw(enemyPng);
		}
		else
		{
			enemy4->Draw(white);
		}

		warp->Draw(texP);

		//stage->Draw(brPng);
		//stageWhite->Draw(white);
		goal->Draw(white);

		//ステージ
		for (auto& object : objects_) {
			//object->Draw(texP);
			object->Draw(texCount);
		}

		//ボックスパーティクル
		for (size_t i = 0; i < boxParticles_.size(); i++)
		{
			boxParticles_[i]->Draw(blackTex);
		}

		break;

	case Pause:
		break;

	case Movie:
		break;

	case GameClear:

		/*if (input->GetKey(DIK_0))
		{
			pressText.Draw(clearTex);
		}*/
		break;

	case GameOver:
		break;

	default:
		break;
	}
	//ここまで3D描画

	//ここから2D描画

	//スプライトのプレドロー
	sprite_->PreDraw();

	//描画用の最大HP(HitPoint用のクラスを作る)
	float maxHP = 3;

	switch (scene)
	{
	case Title:
		//タイトル
		titlePng->Draw(titleTex);
		//sprite_->Draw();

		UIAButton->Draw(AbuttonTex);
		UIAButton->Update();

		UIPress->Draw(PressTex);
		UIPress->Update();
		break;

	case Select:
		break;

	case Play:

		//操作(UI描画一つにまとめる)
		UILStick->Draw(LTex);
		UILStick->Update();

		UIRStick->Draw(RTex);
		UIRStick->Update();

		UILT->Draw(LTTex);
		UILT->Update();

		//HP表示
		lifePng->Draw(lifeTex);

		//HP表示(3つまで)
		//(マジックナンバー直す)
		for (size_t i = 0; i < maxHP; i++)
		{
			if (i < (player->GetLife()))
			{
				if (i == 0)
				{
					havePng->position = { -430.0f + (i * 128),230.0f,0.0f };
					havePng->Update();
					havePng->Draw(heartHaveTex);
				}
				else if (i == 1)
				{
					havePng2->position = { -430.0f + (i * 128),230.0f,0.0f };
					havePng2->Update();
					havePng2->Draw(heartHaveTex);
				}
				else
				{
					havePng3->position = { -430.0f + (i * 128),230.0f,0.0f };
					havePng3->Update();
					havePng3->Draw(heartHaveTex);
				}
			}
			else
			{
				if (i == 0)
				{
					lesPng->position = { -430.0f + (i * 128),230.0f,0.0f };
					lesPng->Update();
					lesPng->Draw(heartLesTex);
				}
				else if (i == 1)
				{
					lesPng2->position = { -430.0f + (i * 128),230.0f,0.0f };
					lesPng2->Update();
					lesPng2->Draw(heartLesTex);
				}
				else
				{
					lesPng3->position = { -430.0f + (i * 128),230.0f,0.0f };
					lesPng3->Update();
					lesPng3->Draw(heartLesTex);
				}
			}
		}

		break;

	case Pause:
		break;

	case Movie:
		break;

	case GameClear:
		//sprite_->Draw(clearTex);
		titlePng->Draw(clearTex);
		break;

	case GameOver:
		titlePng->Draw(overTex);
		break;

	default:
		break;
	}

	//シーンチェンジ用描画
	chengeScene->Draw();

	//ここまで2D描画

	//Imgui 表示させるかどうか
	if (imguiDrawFlag)
	{
		//imgui->Draw(dx.get());
	}

	dx->PostDraw();
}

//delete
void GameScene::Finalize()
{
	imgui->Finalize();

	for (auto& object : objects_)
	{
		delete object;
	}

	FlameWork::Finalize();
}

//一連の流れ
void GameScene::Run()
{
	//初期化
	Initilize();

	//ゲームループ
	while (true)
	{
		//更新
		Update();

		//描画
		Draw();

		//もしエンドフラグがTrueなら抜ける
		if (IsEndRequst() == true)
		{
			break;
		}
	}

	//終了処理
	Finalize();
}

//リセット
void GameScene::Reset()
{
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };
	goalFlag = false;

	//tex
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	//描画用行列
	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//player
	player->Reset();

	//仮enemy置き
	//今は決め打ち
	//Bkenderで設定できるように
	enemy->SetTrans(Vector3D{ 180,20,0 });
	enemy->SetScale(Vector3D{ 1,1,1 });
	enemy->isDead = false;

	enemy2->SetTrans(Vector3D{ 150,40,0 });
	enemy2->SetScale(Vector3D{ 1,1,1 });
	enemy2->isDead = false;

	enemy3->SetTrans(Vector3D{ 200,30,0 });
	enemy3->SetScale(Vector3D{ 1,1,1 });
	enemy3->isDead = false;

	enemy4->SetTrans(Vector3D{ 300,50,0 });
	enemy4->SetScale(Vector3D{ 1,1,1 });
	enemy4->isDead = false;
	//stage
	//ステージ初期化
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection);

	//ゴール初期化
	goal->Reset();

	//シーンフラグ
	scene = Title;
}

//ホットリロード
void GameScene::StageReload()
{
	//デバッグ用
	bool plessZero = input->GetTrigger(DIK_0);
	bool plessNine = input->GetTrigger(DIK_9);
	bool plessEight = input->GetTrigger(DIK_8);
	bool plessSeven = input->GetTrigger(DIK_7);
	bool plessSix = input->GetTrigger(DIK_6);
	bool plessFive = input->GetTrigger(DIK_5);

	bool ChengeFlag = plessZero || plessNine || plessEight || plessSeven || plessSix || plessFive;

	if (ChengeFlag)
	{
		size_t count = objects_.size();

		//今あるステージを削除
		for (size_t i = 0; i < count; i++)
		{
			objects_.erase(objects_.begin());
		}

		//0ならuntitled
		if (plessZero)
		{
			levelData_ = JsonFileOpen::FileOpen("untitled");
		}

		//9ならTest01
		if (plessNine)
		{
			levelData_ = JsonFileOpen::FileOpen("Test01");
		}

		//8ならstage2
		if (plessEight)
		{
			levelData_ = JsonFileOpen::FileOpen("stage2");
		}

		//7ならstage2test
		if (plessSeven)
		{
			levelData_ = JsonFileOpen::FileOpen("stage2Test");
		}

		//6ならstage3
		if (plessSix)
		{
			levelData_ = JsonFileOpen::FileOpen("stage3");
		}

		//5ならstage4
		if (plessFive)
		{
			levelData_ = JsonFileOpen::FileOpen("stage4");
		}

		//ホットリロードでStageSelectごとに読み込むようにする
		//レベルデータからオブジェクトに生成、配置
		for (auto& objectdata : levelData_->objects)
		{
			//ファイル名から登録済みモデルを検索
			Stage* model_ = nullptr;
			decltype(stages_)::iterator it = stages_.find(objectdata.fileName);

			//終わりか
			if (it != stages_.end())
			{
				model_ = it->second;
			}

			//モデルを指定して3Dオブジェクトを生成
			Stage* newModel_ = new Stage();
			newModel_->Initialize(dx.get(), shader, pipeline.get());

			//調整
			float scale = 10.0f;

			//trans
			newModel_->stage_.mat.trans = objectdata.translation * scale;

			//rotation
			newModel_->stage_.mat.rotAngle = objectdata.rotation;

			//scale;
			newModel_->stage_.mat.scale = objectdata.scaling * scale;

			//Update
			newModel_->Update(matView.mat, matProjection);

			//格納
			objects_.push_back(newModel_);
		}
	}
}

//ランダムの数パーティクルを出す
void GameScene::CreatePatricle(Vector3D pos)
{
	size_t minRange = 10;
	size_t maxRange = 30;

	size_t play = MyMath::GetRandom(minRange, maxRange);

	for (size_t i = 0; i < play; i++)
	{
		BoxParticle* newP = new BoxParticle();

		newP->Initialize(dx.get(), shader, pipeline.get());

		newP->SetPos(pos);

		boxParticles_.push_back(newP);
	}
}
