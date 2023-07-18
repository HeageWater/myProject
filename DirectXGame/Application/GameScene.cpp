#include "GameScene.h"

void GameScene::Update()
{
	//FlameWork::Update();
	win->MsgUpdate();
	if (win->EndLoop())
	{
		SetEndRwqust(true);
	}

	//Update
	input->Update();

	//���W�X�V
	matView.MatUpdate();

	//Imgui
	imgui->Begin();

	ImGui::InputFloat("matView.eye.x", &matView.eye.x, 0.0f, 10.0f, "%f");
	ImGui::InputFloat("matView.eye.y", &matView.eye.y, 0.0f, 10.0f, "%f");
	ImGui::InputFloat("matView.eye.z", &matView.eye.z, 0.0f, 10.0f, "%f");

	//ImGui�����܂�
	imgui->End();

	if (scene == false)
	{
		//player�X�V
		player->Update(matView.mat, matProjection);

		//enemy�X�V
		enemy->Update(matView.mat, matProjection);
		bool sheikF = enemy->BoxCollision(player->GetAttackModel());

		//enemy�X�V
		enemy2->Update(matView.mat, matProjection);
		sheikF = enemy2->BoxCollision(player->GetAttackModel());

		//enemy�X�V
		enemy3->Update(matView.mat, matProjection);
		sheikF = enemy3->BoxCollision(player->GetAttackModel());

		//enemy�X�V
		enemy4->Update(matView.mat, matProjection);
		sheikF = enemy4->BoxCollision(player->GetAttackModel());

		//�X�e�[�W�X�V
		stage->Update(matView.mat, matProjection, input.get());
		goal->Update(matView.mat, matProjection);

		//debugcamera.Update(*input);

		//�X�N���[���X�V
		screen.MatUpdate(matView.mat, matProjection, 0);

		Vector2D moveCamera = { 0,0 };

		moveCamera = player->GetController();

		//target��player��
		matView.eye.x += moveCamera.x;
		matView.target.x = player->GetPos().x;

		matView.eye.x = min(matView.eye.x, 1050);
		matView.eye.x = max(matView.eye.x, 0);

		//stage->stage_.mat.trans.x = max(stage->stage_.mat.trans.x, minMapX);
		bool checkGoal = goal->BoxCollision(player->GetModel());

		if (checkGoal)
		{
			scene = true;
		}
	}
	//�����܂�

	//Escape�Ŕ�����
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
	//FlameWork::Initilize();

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

	//�`�揉����
	multipathPipeline = std::make_unique<GPipeline>();
	multipathPipeline->Initialize(dx->GetDev(), bilShader);

	//screen
	screen.Initialize(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };

	//gpipeline
	uiPipeline = std::make_unique<GPipeline>();
	uiPipeline->Initialize(dx->GetDev(), bilShader);

	//sprite
	spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);

	//tex
	pressText.Initialize(dx.get(), uiPipeline.get(), bilShader);
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	//�`��p�s��
	matView.Init(Vector3D(0.0f, 60.0f, -150.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//sound
	sound_ = MyXAudio::Get();
	bgm = sound_->SoundLoadWave("Resources/sound/BGM.wav");
	fanfare = sound_->SoundLoadWave("Resources/sound/fanfare.wav");

	//player
	player->Initialize(dx.get(), shader, pipeline.get());

	//��enemy�u��
	enemy->Initialize(dx.get(), shader, pipeline.get());
	enemy->SetTrans(Vector3D{ 100,20,0 });

	enemy2->Initialize(dx.get(), shader, pipeline.get());
	enemy2->SetTrans(Vector3D{ 300,40,0 });

	enemy3->Initialize(dx.get(), shader, pipeline.get());
	enemy3->SetTrans(Vector3D{ 600,30,0 });

	enemy4->Initialize(dx.get(), shader, pipeline.get());
	enemy4->SetTrans(Vector3D{ 800,50,0 });

	//stage
	//�X�e�[�W������
	stage->Initialize(dx.get(), shader, pipeline.get());
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	stageWhite->Initialize(dx.get(), shader, pipeline.get());
	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection, input.get());

	//�S�[��������
	goal->Initialize(dx.get(), shader, pipeline.get());

	scene = false;

	//�摜�ǂݍ���
	white = dx->LoadTextureGraph(L"Resources/white1x1.png");
	texP = dx->LoadTextureGraph(L"Resources/cube.jpg");
	brPng = dx->LoadTextureGraph(L"Resources/br.png");
	enemyPng = dx->LoadTextureGraph(L"Resources/ene/enemy.png");
	clearTex = dx->LoadTextureGraph(L"Resources/gameclear.png");

	//imgui������
	imgui->Initialize(dx.get());
}

void GameScene::Draw()
{
	//Draw
	dx->PrevDrawScreen();

	//// �`��R�}���h

	dx->PostDrawScreen();

	//UIDraw
	dx->PrevDraw();

	//�X�N���[���`��
	screen.Draw(texP);

	//Actor�`��
	player->Draw(texP, white);
	enemy->Draw(enemyPng);
	enemy2->Draw(enemyPng);
	enemy3->Draw(enemyPng);
	enemy4->Draw(enemyPng);
	stage->Draw(brPng);
	stageWhite->Draw(white);
	goal->Draw(white);

	if (scene == true)
	{
		pressText.Draw(clearTex);
	}

	imgui->Draw(dx.get());

	dx->PostDraw();
}

void GameScene::Finalize()
{
	imgui->Finalize();

	//FlameWork::Finalize();
}

void GameScene::Run()
{
	//������
	Initilize();

	//�Q�[�����[�v
	while (true)
	{
		//�X�V
		Update();

		//�`��
		Draw();

		//�����G���h�t���O��True�Ȃ甲����
		if (IsEndRequst() == true)
		{
			break;
		}
	}

	//�I������
	Finalize();
}
