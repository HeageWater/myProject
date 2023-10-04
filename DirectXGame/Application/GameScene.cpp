#include "GameScene.h"

int Time = 0;

void GameScene::Update()
{
	FlameWork::Update();

	//Update
	input->Update();

	//ImGui��t�J�n
	ImguiManager::GetInstance()->Begin();
	ImGui::Text("player pos");
	//ImGui��t�I��
	ImguiManager::GetInstance()->End();

	//scene�ɉ���
	//��������Scene�̏���

	//switch���Ŏg���֐��������Œ�`
	float chengeTime = 50;

	//�X�e�[�W�z�b�g�����[�h
	StageReload();

	//switch�p�ϐ�

	float t = 85;
	float stopTime = 100;
	switch (scene)
	{
	case Title:

		if (chengeScene->GetTime() > chengeTime)
		{
			scene = Play;
		}

		if (player->GetA())
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
			//player�X�V
			player->Update(matView.mat, matProjection, MyDirectX::GetInstance(), shader, pipeline.get());

			//enemy�X�V
			enemy->Update(matView.mat, matProjection);
			enemy->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy->Time = stopTime;
				sound_->SoundPlayWave(playerHit);
			}
			bool sheikF = enemy->BoxCollision(player->GetAttackModel());

			//���̉��̏����܂Ƃ߂�
			float setStopTime = 7.0f;

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
				sound_->SoundPlayWave(enemyHit);
				CreatePatricle(enemy->GetPos());
			}

			//enemy�X�V
			enemy2->Update(matView.mat, matProjection);
			enemy2->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy2->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy2->Time = stopTime;
				sound_->SoundPlayWave(playerHit);
			}
			sheikF = enemy2->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
				sound_->SoundPlayWave(enemyHit);
				CreatePatricle(enemy2->GetPos());
			}

			//enemy�X�V
			enemy3->Update(matView.mat, matProjection);
			enemy3->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy3->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy3->Time = stopTime;
				sound_->SoundPlayWave(playerHit);
			}
			sheikF = enemy3->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
				sound_->SoundPlayWave(enemyHit);
				CreatePatricle(enemy3->GetPos());
			}

			//enemy�X�V
			enemy4->Update(matView.mat, matProjection);
			enemy4->SertchPlayer(player->GetModel());
			if (player->PlayerCollision(enemy4->enemy_))
			{
				CreatePatricle(player->GetPos());
				enemy4->Time = stopTime;
				sound_->SoundPlayWave(playerHit);
			}
			sheikF = enemy4->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);
				sound_->SoundPlayWave(enemyHit);
				CreatePatricle(enemy4->GetPos());
			}

			//warp��player��collision
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

			//�����܂�

			//�X�e�[�W�X�V
			stage->Update(matView.mat, matProjection);
			goal->Update(matView.mat, matProjection);
			warp->Update(matView.mat, matProjection);

			//�p�[�e�B�N��
			for (size_t i = 0; i < boxParticles_.size(); i++)
			{
				boxParticles_[i]->Update(matView.mat, matProjection);

				//�폜
				if (boxParticles_[i]->IsDead() == true)
				{
					boxParticles_.erase(boxParticles_.begin() + i);
				}
			}

			//�X�N���[���X�V
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


			//target��player��
			matView.eye.x = player->GetPos().x;
			matView.target.x = player->GetPos().x;

			matView.eye.y = player->GetPos().y;
			matView.target.y = player->GetPos().y;

			matView.mat = playcamera.mat;

			//���W�X�V
			playcamera.Update(*input);
			debugcamera.Update(*input);

			//�J�����X�V
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

				Reset();

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
		if (player->GetA())
		{
			chengeScene->SetPlayFlag();

			scene = Title;

			Reset();
		}
		break;

	case GameOver:

		if (player->GetA())
		{
			chengeScene->SetPlayFlag();

			scene = Title;

			Reset();
		}
		break;

	default:
		break;
	}

	//�����܂�Scene�̏���

	//Scene���ʂ̏���
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

	//Escape�Ŕ�����
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
	//screen.Initialize(dx.get(), multipathPipeline.get(), bilShader);
	screen.Initialize(MyDirectX::GetInstance(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };

	//sprite
	spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);

	//tex
	pressText.Initialize(MyDirectX::GetInstance(), uiPipeline.get(), spriteShader);
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	//�`��p�s��
	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//sound
	sound_ = MyXAudio::Get();
	bgm = sound_->SoundLoadWave("Resources/sound/BGM.wav");
	fanfare = sound_->SoundLoadWave("Resources/sound/fanfare.wav");
	playerHit = sound_->SoundLoadWave("Resources/sound/se_hit_005.wav");
	enemyHit = sound_->SoundLoadWave("Resources/sound/se_hit_008.wav");

	//player
	player->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());

	//warp
	warp->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());

	float size = 3.0f;

	//��enemy�u��
	enemy->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());
	enemy->SetTrans(Vector3D{ 180,20,0 });
	enemy->SetScale(Vector3D{ size,size,size });

	enemy2->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());
	enemy2->SetTrans(Vector3D{ 150,40,0 });
	enemy2->SetScale(Vector3D{ size,size,size });

	enemy3->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());
	enemy3->SetTrans(Vector3D{ 200,30,0 });
	enemy3->SetScale(Vector3D{ size,size,size });

	enemy4->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());
	enemy4->SetTrans(Vector3D{ 300,50,0 });
	enemy4->SetScale(Vector3D{ size,size,size });

	//stage
	//�X�e�[�W������
	stage->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	stageWhite->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());
	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection);

	//�S�[��������
	goal->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());
	//�V�[���t���O
	scene = Title;

	//�摜�ǂݍ���
	white = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/white1x1.png");
	texP = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/cube.jpg");
	brPng = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");
	clearTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameclear.png");
	overTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/gameover.png");
	titleTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Title.png");
	playerTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/Player/Player.png");
	enemyPng = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
	blackTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/black.png");
	heartLesTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartLes.png");
	heartHaveTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/heartHave.png");
	lifeTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/life.png");
	backTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/background.png");
	LTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Lstick.png");
	RTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Rstick.png");
	AbuttonTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/Abutton.png");
	PressTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/press.png");
	LTTex = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/LT.png");

	//���߂��邩�ǂ���
	semiArphaSpriteCommon->Inilialize(MyDirectX::GetInstance(), true);
	normalSpriteCommon->Inilialize(MyDirectX::GetInstance(), false);

	{
		//��b
		sprite_->Inilialize(semiArphaSpriteCommon, &matProjection);

		//�^�C�g��
		titlePng->Inilialize(normalSpriteCommon, &matProjection);
		titlePng->position = { -680,-420,0 };
		titlePng->scale = { 3600,1440,1 };

		//���C�t�p��
		lifePng->Inilialize(normalSpriteCommon, &matProjection);
		lifePng->position = { -590,240,0 };
		lifePng->scale = { 360,144,1 };

		//���C�t1
		lesPng->Inilialize(normalSpriteCommon, &matProjection);
		lesPng->position = { -200,200,0 };
		lesPng->scale = { 256,144,1 };

		//���C�t2
		lesPng2->Inilialize(normalSpriteCommon, &matProjection);
		lesPng2->position = { -200,200,0 };
		lesPng2->scale = { 256,144,1 };

		//���C�t3
		lesPng3->Inilialize(normalSpriteCommon, &matProjection);
		lesPng3->position = { -200,200,0 };
		lesPng3->scale = { 256,144,1 };

		//���C�t1
		havePng->Inilialize(normalSpriteCommon, &matProjection);
		havePng->position = { -680,-420,0 };
		havePng->scale = { 256,144,1 };

		//���C�t2
		havePng2->Inilialize(normalSpriteCommon, &matProjection);
		havePng2->position = { -680,-420,0 };
		havePng2->scale = { 256,144,1 };

		//���C�t3
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

	};

	goalFlag = false;

	//�X�e�[�W�ǂݍ���
	//StageLoad("stage4");
	StageLoad("TitleStage");

	chengeScene->Initialize(MyDirectX::GetInstance(), pipeline.get(), matProjection);

	//����炷
	//sound_->SoundPlayLoopWave(bgm);
}

void GameScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	//// �`��R�}���h

	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//�������狤�ʐ�`��

	//�X�N���[���`��
	screen.Draw(backTex);
	size_t texCount = 19;
	//�����܂ŋ��ʐ�`��

	//��������3D�`��
	switch (scene)
	{
	case Title:
		break;

	case Select:
		break;

	case Play:

		//Actor�`��
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

		//�X�e�[�W
		for (auto& object : objects_) {
			//object->Draw(texP);
			object->Draw(texCount);
		}

		//�{�b�N�X�p�[�e�B�N��
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
		break;

	case GameOver:
		break;

	default:
		break;
	}
	//�����܂�3D�`��

	//��������2D�`��

	//�X�v���C�g�̃v���h���[
	sprite_->PreDraw();

	//�`��p�̍ő�HP(HitPoint�p�̃N���X�����)
	float maxHP = 3;

	switch (scene)
	{
	case Title:
		//�^�C�g��
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

		//����(UI�`���ɂ܂Ƃ߂�)
		UILStick->Draw(LTex);
		UILStick->Update();

		UIRStick->Draw(RTex);
		UIRStick->Update();

		UILT->Draw(LTTex);
		UILT->Update();

		//HP�\��
		lifePng->Draw(lifeTex);

		//HP�\��(3�܂�)
		//(�}�W�b�N�i���o�[����)
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

	//�V�[���`�F���W�p�`��
	chengeScene->Draw();

	//�����܂�2D�`��
	ImguiManager::GetInstance()->Draw();

	MyDirectX::GetInstance()->PostDraw();
}

//delete
void GameScene::Finalize()
{
	for (auto& object : objects_)
	{
		delete object;
	}

	FlameWork::Finalize();
}

//��A�̗���
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
		if (IsEndRequst())
		{
			break;
		}
	}

	//�I������
	Finalize();
}

//���Z�b�g
void GameScene::Reset()
{
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };
	goalFlag = false;

	//tex
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	//�`��p�s��
	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//player
	player->Reset();

	//��enemy�u��
	//���͌��ߑł�
	//Bkender�Őݒ�ł���悤��
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
	//�X�e�[�W������
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection);

	//�S�[��������
	goal->Reset();

	//�V�[���t���O
	scene = Title;
}

//�z�b�g�����[�h
void GameScene::StageReload()
{
	//�f�o�b�O�p
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

		//������X�e�[�W���폜
		for (size_t i = 0; i < count; i++)
		{
			objects_.erase(objects_.begin());
		}

		//0�Ȃ�untitled
		if (plessZero)
		{
			levelData_ = JsonFileOpen::FileOpen("untitled");
		}

		//9�Ȃ�Test01
		if (plessNine)
		{
			levelData_ = JsonFileOpen::FileOpen("Test01");
		}

		//8�Ȃ�stage2
		if (plessEight)
		{
			levelData_ = JsonFileOpen::FileOpen("stage2");
		}

		//7�Ȃ�stage2test
		if (plessSeven)
		{
			levelData_ = JsonFileOpen::FileOpen("stage2Test");
		}

		//6�Ȃ�stage3
		if (plessSix)
		{
			levelData_ = JsonFileOpen::FileOpen("stage3");
		}

		//5�Ȃ�stage4
		if (plessFive)
		{
			levelData_ = JsonFileOpen::FileOpen("stage4");
		}

		//�z�b�g�����[�h��StageSelect���Ƃɓǂݍ��ނ悤�ɂ���
		//���x���f�[�^����I�u�W�F�N�g�ɐ����A�z�u
		for (auto& objectdata : levelData_->objects)
		{
			//�t�@�C��������o�^�ς݃��f��������
			Stage* model_ = nullptr;
			decltype(stages_)::iterator it = stages_.find(objectdata.fileName);

			//�I��肩
			if (it != stages_.end())
			{
				model_ = it->second;
			}

			//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
			Stage* newModel_ = new Stage();
			newModel_->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());

			//����
			float scale = 10.0f;

			//trans
			newModel_->stage_.mat.trans = objectdata.translation * scale;

			//rotationboxParticles_
			newModel_->stage_.mat.rotAngle = objectdata.rotation;

			//scale;
			newModel_->stage_.mat.scale = objectdata.scaling * scale;

			//Update
			newModel_->Update(matView.mat, matProjection);

			//�i�[
			objects_.push_back(newModel_);
		}
	}
}

//�����_���̐��p�[�e�B�N�����o��
void GameScene::CreatePatricle(Vector3D pos)
{
	size_t minRange = 10;
	size_t maxRange = 30;

	size_t play = MyMath::GetRandom(minRange, maxRange);

	for (size_t i = 0; i < play; i++)
	{
		BoxParticle* newP = new BoxParticle();

		newP->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());

		newP->SetPos(pos);

		boxParticles_.push_back(newP);
	}
}

//�t�@�C����������Ƃ����ǂݍ���
void GameScene::StageLoad(const std::string& filePath)
{
	//stage�t�@�C��
	levelData_ = JsonFileOpen::FileOpen(filePath);

	//�z�b�g�����[�h��StageSelect���Ƃɓǂݍ��ނ悤�ɂ���
	//���x���f�[�^����I�u�W�F�N�g�ɐ����A�z�u
	for (auto& objectdata : levelData_->objects)
	{
		//�t�@�C��������o�^�ς݃��f��������
		Stage* model_ = nullptr;
		decltype(stages_)::iterator it = stages_.find(objectdata.fileName);

		//�I��肩
		if (it != stages_.end())
		{
			model_ = it->second;
		}

		//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		Stage* newModel_ = new Stage();
		newModel_->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());

		//����
		float scale = 10.0f;

		//trans
		newModel_->stage_.mat.trans = objectdata.translation * scale;

		//rotation
		newModel_->stage_.mat.rotAngle = objectdata.rotation;

		//scale;
		newModel_->stage_.mat.scale = objectdata.scaling * scale;

		//Update
		newModel_->Update(matView.mat, matProjection);

		//�i�[
		objects_.push_back(newModel_);
	}
}
