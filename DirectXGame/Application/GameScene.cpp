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

	//Imgui
	imgui->Begin();

	float size = (float)particles_.size();

	//ImGui::InputFloat("constMapTransform.eye.x", &sprite->position.x, 0.0f, 10.0f, "%f");
	//ImGui::InputFloat("constMapTransform.eye.y", &sprite->position.y, 0.0f, 10.0f, "%f");
	//ImGui::InputFloat("constMapTransform.eye.z", &sprite->position.z, 0.0f, 10.0f, "%f");
	ImGui::InputFloat("particleCount", &size, 0.0f, 10.0f, "%f");

	ImGui::InputFloat("spriteX", &sprite_->position.x, 0.0f, 1000.0f, "%f");
	ImGui::InputFloat("spriteY", &sprite_->position.y, 0.0f, 1000.00f, "%f");

	sprite_->position.x += input->GetKey(DIK_L) - input->GetKey(DIK_K);
	sprite_->position.y += input->GetKey(DIK_M) - input->GetKey(DIK_N);


	for (size_t i = 0; i < particles_.size(); i++)
	{
		ImGui::InputFloat("dev", &particles_[i]->time, 0.0f, 10.0f, "%f");
	}

	//ImGuiここまで
	imgui->End();

	if (scene == false && hitStop->GetTime() < 1)
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
		stage->Update(matView.mat, matProjection, input.get());
		goal->Update(matView.mat, matProjection);

		bool hit = player->CollisionAttackToEnemy(enemy->enemy_);

		//debugcamera.Update(*input);

		//パーティクル

		for (size_t i = 0; i < particles_.size(); i++)
		{
			particles_[i]->Update(matView.mat, matProjection);

			//削除
			if (particles_[i]->IsDead() == true)
			{
				particles_.erase(particles_.begin() + i);
			}
		}

		//スクリーン更新
		screen.MatUpdate(matView.mat, matProjection, 0);

		//Vector2D moveCamera = { 0,0 };

		//moveCamera = player->GetController();

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
			scene = true;
		}
	}

	//ここまで
	pressText.MatUpdate(Matrix(), spriteProjection);

	hitStop->Update();

	sprite_->Update();

	if (input->GetTrigger(DIK_SPACE))
	{
		//emitter_->Create();

		size_t play = MyMath::GetRandom(10, 30);

		for (size_t i = 0; i < play; i++)
		{
			Particle* newP = new Particle();

			//newP = copyParticle_;

			newP->SetModel(copyParticle_->GetModel());

			newP->Initialize(dx.get(), pipeline.get());

			newP->Initialize(player->GetPos());

			particles_.push_back(newP);
		}
	}

	//Escapeで抜ける
	if (input->GetTrigger(DIK_ESCAPE))
	{
		SetEndRwqust(true);
	}

	if (input->GetTrigger(DIK_K))
	{
		enemy->enemy_.mat.trans.x -= 1;
	}

	if (input->GetTrigger(DIK_P))
	{
		sound_->SoundPlayWave(bgm);
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
	enemy->Initialize(dx.get(), shader, pipeline.get());
	enemy->SetTrans(Vector3D{ 100,20,0 });

	enemy2->Initialize(dx.get(), shader, pipeline.get());
	enemy2->SetTrans(Vector3D{ 300,40,0 });

	enemy3->Initialize(dx.get(), shader, pipeline.get());
	enemy3->SetTrans(Vector3D{ 600,30,0 });

	enemy4->Initialize(dx.get(), shader, pipeline.get());
	enemy4->SetTrans(Vector3D{ 800,50,0 });

	//stage
	//ステージ初期化
	stage->Initialize(dx.get(), shader, pipeline.get());
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	stageWhite->Initialize(dx.get(), shader, pipeline.get());
	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection, input.get());

	//ゴール初期化
	goal->Initialize(dx.get(), shader, pipeline.get());

	//シーンフラグ
	scene = false;

	//画像読み込み
	white = dx->LoadTextureGraph(L"Resources/white1x1.png");
	texP = dx->LoadTextureGraph(L"Resources/cube.jpg");
	brPng = dx->LoadTextureGraph(L"Resources/br.png");
	enemyPng = dx->LoadTextureGraph(L"Resources/ene/enemy.png");
	clearTex = dx->LoadTextureGraph(L"Resources/gameclear.png");

	//imgui初期化
	imgui->Initialize(dx.get());

	spriteCommon->Inilialize(dx.get());

	//sprite->TransferSpriteVertex(Vector2D(300, 300));
	sprite_->Inilialize(spriteCommon, &matProjection);

	copyParticle_->Initialize(dx.get(), shader, pipeline.get());
}

void GameScene::Draw()
{
	//Draw
	dx->PrevDrawScreen();

	//// 描画コマンド

	dx->PostDrawScreen();

	//UIDraw
	dx->PrevDraw();

	//スクリーン描画
	screen.Draw(texP);

	//Actor描画
	//player->Draw(texP, white);
	enemy->Draw(enemyPng);
	enemy2->Draw(enemyPng);
	enemy3->Draw(enemyPng);
	enemy4->Draw(enemyPng);
	//stage->Draw(brPng);
	//stageWhite->Draw(white);
	goal->Draw(white);

	sprite_->PreDraw();

	sprite_->Draw(clearTex);

	//パーティクル
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i]->Draw(white);
	}

	if (scene == true)
	{
		//dx->ClearDepthBuff();
		if (input->GetKey(DIK_0))
		{
			pressText.Draw(clearTex);
		}
	}

	imgui->Draw(dx.get());

	dx->PostDraw();
}

void GameScene::Finalize()
{
	imgui->Finalize();

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
