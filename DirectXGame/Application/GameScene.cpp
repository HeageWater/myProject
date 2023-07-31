#include "GameScene.h"

void GameScene::Update()
{
	FlameWork::Update();

	win->MsgUpdate();
	if (win->EndLoop())
	{
		SetEndRwqust(true);
	}

	//Update
	input->Update();

	//座標更新
	matView.MatUpdate();

	if (input->GetTrigger(DIK_O))
	{
		imguiDrawFlag = !imguiDrawFlag;
	}

	//imgui
	imgui->Begin();

	float size = (float)boxParticles_.size();

	ImGui::InputFloat("particleCount", &size, 0.0f, 10.0f, "%f");

	ImGui::InputFloat("spriteX", &sprite_->scale.x, 0.0f, 1000.0f, "%f");
	ImGui::InputFloat("spriteY", &sprite_->scale.y, 0.0f, 1000.00f, "%f");

	//for (auto& object : objects_) {
	ImGui::InputFloat("stageX", &objects_[5]->stage_.mat.rotAngle.x, 0.0f, 1000.0f, "%f");
	ImGui::InputFloat("stagey", &objects_[5]->stage_.mat.rotAngle.y, 0.0f, 1000.0f, "%f");

	ImGui::Text("O : ImGuiFlag");
	ImGui::Checkbox("ImGuiDraw", &imguiDrawFlag);

	//ImGuiここまで
	imgui->End();

	//8月中にsceneに改造
	//ここからSceneの処理

	//switch内で使う関数をここで定義
	float chengeTime = 50;

	if (input->GetTrigger(DIK_P))
	{
		scene = Title;
	}

	switch (scene)
	{
	case Title:

		if (chengeScene->GetTime() > chengeTime)
		{
			scene = Play;
		}

		if (input->GetTrigger(DIK_SPACE))
		{
			chengeScene->SetPlayFlag();
		}
		break;

	case Select:
		break;

	case Play:
		if (hitStop->GetTime() < 1)
		{
			//player更新
			player->Update(matView.mat, matProjection, dx.get(), shader, pipeline.get());

			//enemy更新
			enemy->Update(matView.mat, matProjection);
			bool sheikF = enemy->BoxCollision(player->GetAttackModel());

			float setStopTime = 7.0f;

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
			}

			//enemy更新
			enemy2->Update(matView.mat, matProjection);
			sheikF = enemy2->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
			}

			//enemy更新
			enemy3->Update(matView.mat, matProjection);
			sheikF = enemy3->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
			}

			//enemy更新
			enemy4->Update(matView.mat, matProjection);
			sheikF = enemy4->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
			}

			//ステージ更新
			stage->Update(matView.mat, matProjection);
			goal->Update(matView.mat, matProjection);

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

			for (auto& object : objects_)
			{
				object->Update(matView.mat, matProjection);
			}

			//targetをplayerに
			//matView.eye.x += dev[19]; //moveCamera.x;
			matView.eye.x = player->GetPos().x; //moveCamera.x;
			matView.target.x = player->GetPos().x;// -dev[0];

			matView.eye.x = min(matView.eye.x, 1050);
			matView.eye.x = max(matView.eye.x, 0);

			//stage->stage_.mat.trans.x = max(stage->stage_.mat.trans.x, minMapX);
			bool checkGoal = goal->BoxCollision(player->GetModel());

			if (checkGoal)
			{
				scene = GameClear;
			}
		}

		break;

	case Pause:
		break;

	case Movie:
		break;

	case GameClear:
		if (input->GetTrigger(DIK_SPACE))
		{

		}
		break;

	case GameOver:

		if (input->GetTrigger(DIK_SPACE))
		{

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

	chengeScene->Update(matView.mat, spriteProjection);

	//パーティクル試し
	//if (input->GetTrigger(DIK_SPACE))
	//{
	//	size_t play = MyMath::GetRandom(10, 30);

	//	for (size_t i = 0; i < play; i++)
	//	{
	//		BoxParticle* newP = new BoxParticle();

	//		newP->Initialize(dx.get(), shader, pipeline.get());

	//		newP->SetPos(player->GetPos());

	//		boxParticles_.push_back(newP);
	//	}
	//}

	//Escapeで抜ける
	if (input->GetTrigger(DIK_ESCAPE))
	{
		SetEndRwqust(true);
	}
}

void GameScene::Initilize()
{
	FlameWork::Initilize();

	//windowApi
	win = std::make_unique<Window>();

	//dxCommon
	dx = std::make_unique<MyDirectX>(win.get());

	//camera
	debugcamera.Init(Vector3D(0.0f, 30.0f, 10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	playcamera.Init(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//buff
	cBuff = std::make_unique<ConstBuff>(dx->GetDev(), (float)win->window_width, (float)win->window_height);

	//input
	input = std::make_unique<Input>(win.get());

	//shader
	shader.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	bilShader.Initizlize(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");
	spriteShader.Initizlize(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");

	//pipeline
	pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(dx->GetDev(), shader);

	//描画初期化
	multipathPipeline = std::make_unique<GPipeline>();
	multipathPipeline->Initialize(dx->GetDev(), bilShader);

	//screen
	screen.Initialize(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };

	//gpipeline
	uiPipeline = std::make_unique<GPipeline>();
	uiPipeline->Initialize(dx->GetDev(), bilShader);
	//uiPipeline->SetBlend(dx->GetDev(), GPipeline::ALPHA_BLEND);

	//sprite
	spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);

	//tex
	pressText.Initialize(dx.get(), uiPipeline.get(), spriteShader);
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	//描画用行列
	matView.Init(Vector3D(0.0f, 60.0f, -150.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//sound
	sound_ = MyXAudio::Get();
	bgm = sound_->SoundLoadWave("Resources/sound/BGM.wav");
	fanfare = sound_->SoundLoadWave("Resources/sound/fanfare.wav");

	//player
	player->Initialize(dx.get(), shader, pipeline.get());

	//仮enemy置き
	//今は決め打ち
	//Bkenderで設定できるように
	enemy->Initialize(dx.get(), shader, pipeline.get());
	enemy->SetTrans(Vector3D{ 180,20,0 });
	enemy->SetScale(Vector3D{ 1,1,1 });

	enemy2->Initialize(dx.get(), shader, pipeline.get());
	enemy2->SetTrans(Vector3D{ 150,40,0 });
	enemy2->SetScale(Vector3D{ 1,1,1 });

	enemy3->Initialize(dx.get(), shader, pipeline.get());
	enemy3->SetTrans(Vector3D{ 200,30,0 });
	enemy3->SetScale(Vector3D{ 1,1,1 });

	enemy4->Initialize(dx.get(), shader, pipeline.get());
	enemy4->SetTrans(Vector3D{ 300,50,0 });
	enemy4->SetScale(Vector3D{ 1,1,1 });

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
	scene = Play;

	//画像読み込み
	white = dx->LoadTextureGraph(L"Resources/white1x1.png");
	texP = dx->LoadTextureGraph(L"Resources/cube.jpg");
	brPng = dx->LoadTextureGraph(L"Resources/br.png");
	enemyPng = dx->LoadTextureGraph(L"Resources/ene/enemy.png");
	clearTex = dx->LoadTextureGraph(L"Resources/gameclear.png");
	playerTex = dx->LoadTextureGraph(L"Resources/Player/Player.png");

	//imgui初期化
	imgui->Initialize(dx.get());

	semiArphaSpriteCommon->Inilialize(dx.get(), true);
	normalSpriteCommon->Inilialize(dx.get(), false);

	sprite_->Inilialize(semiArphaSpriteCommon, &matProjection);

	//stageファイル
	levelData_ = JsonFileOpen::FileOpen("untitled2");

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
	screen.Draw(texP);

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
		enemy->Draw(enemyPng);
		enemy2->Draw(enemyPng);
		enemy3->Draw(enemyPng);
		enemy4->Draw(enemyPng);
		//stage->Draw(brPng);
		//stageWhite->Draw(white);
		goal->Draw(white);
		for (auto& object : objects_) {
			object->Draw(white);
		}

		//ボックスパーティクル
		for (size_t i = 0; i < boxParticles_.size(); i++)
		{
			boxParticles_[i]->Draw(white);
		}

		break;

	case Pause:
		break;

	case Movie:
		break;

	case GameClear:

		if (input->GetKey(DIK_0))
		{
			pressText.Draw(clearTex);
		}
		break;

	case GameOver:
		break;

	default:
		break;
	}
	//ここまで3D描画

	//ここから2D描画

	sprite_->PreDraw();

	switch (scene)
	{
	case Title:
		break;

	case Select:
		break;

	case Play:
		break;

	case Pause:
		break;

	case Movie:
		break;

	case GameClear:
		sprite_->Draw(clearTex);
		break;

	case GameOver:
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
		imgui->Draw(dx.get());
	}

	dx->PostDraw();
}

void GameScene::Finalize()
{
	imgui->Finalize();

	for (auto& object : objects_)
	{
		delete object;
	}

	FlameWork::Finalize();
}

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
