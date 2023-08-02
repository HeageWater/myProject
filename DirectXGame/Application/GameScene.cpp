#include "GameScene.h"

void GameScene::Update()
{
	FlameWork::Update();

	//Update
	input->Update();

	//���W�X�V
	matView.MatUpdate();

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

		ImGui::Text("player pos");
		ImGui::InputFloat("x", &x, 0.0f, 1000.0f, "%f");
		ImGui::SliderFloat("y", &y, 0.0f, 1000.0f, "%f");

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

		ImGui::TreePop();
	}

	//ImGui�����܂�
	imgui->End();

	//scene�ɉ���
	//��������Scene�̏���

	//switch���Ŏg���֐��������Œ�`
	float chengeTime = 50;

	if (input->GetTrigger(DIK_P))
	{
		scene = Title;
		Reset();
	}

	//�X�e�[�W�z�b�g�����[�h
	StageReload();

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
		if (hitStop->GetTime() < 1)
		{
			//player�X�V
			player->Update(matView.mat, matProjection, dx.get(), shader, pipeline.get());

			//enemy�X�V
			enemy->Update(matView.mat, matProjection);
			bool sheikF = enemy->BoxCollision(player->GetAttackModel());

			//���̉��̏����܂Ƃ߂�
			float setStopTime = 7.0f;

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				size_t play = MyMath::GetRandom(10, 30);

				for (size_t i = 0; i < play; i++)
				{
					BoxParticle* newP = new BoxParticle();

					newP->Initialize(dx.get(), shader, pipeline.get());

					newP->SetPos(enemy->GetPos());

					boxParticles_.push_back(newP);
				}
			}

			//enemy�X�V
			enemy2->Update(matView.mat, matProjection);
			sheikF = enemy2->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				size_t play = MyMath::GetRandom(10, 30);

				for (size_t i = 0; i < play; i++)
				{
					BoxParticle* newP = new BoxParticle();

					newP->Initialize(dx.get(), shader, pipeline.get());

					newP->SetPos(enemy2->GetPos());

					boxParticles_.push_back(newP);
				}
			}

			//enemy�X�V
			enemy3->Update(matView.mat, matProjection);
			sheikF = enemy3->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				size_t play = MyMath::GetRandom(10, 30);

				for (size_t i = 0; i < play; i++)
				{
					BoxParticle* newP = new BoxParticle();

					newP->Initialize(dx.get(), shader, pipeline.get());

					newP->SetPos(enemy3->GetPos());

					boxParticles_.push_back(newP);
				}
			}

			//enemy�X�V
			enemy4->Update(matView.mat, matProjection);
			sheikF = enemy4->BoxCollision(player->GetAttackModel());

			if (sheikF)
			{
				hitStop->SetTime(setStopTime);

				size_t play = MyMath::GetRandom(10, 30);

				for (size_t i = 0; i < play; i++)
				{
					BoxParticle* newP = new BoxParticle();

					newP->Initialize(dx.get(), shader, pipeline.get());

					newP->SetPos(enemy4->GetPos());

					boxParticles_.push_back(newP);
				}
			}

			//�����܂�

			//�X�e�[�W�X�V
			stage->Update(matView.mat, matProjection);
			goal->Update(matView.mat, matProjection);

			//bool hit = player->CollisionAttackToEnemy(enemy->enemy_);

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

			for (auto& object : objects_)
			{
				object->SetFlag(true);

				object->Update(matView.mat, matProjection);

				if (player->StageCollsion(object->stage_, matView.mat, matProjection))
				{
					//object->SetFlag(false);
				}
			}

			//target��player��
			//matView.eye.x += dev[19]; //moveCamera.x;
			matView.eye.x = player->GetPos().x; //moveCamera.x;
			matView.target.x = player->GetPos().x;// -dev[0];

			//float range = 50;

			matView.eye.y = player->GetPos().y; //moveCamera.x;
			matView.target.y = player->GetPos().y;// -dev[0];part

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
			}

		}

		break;

	case Pause:
		break;

	case Movie:
		break;

	case GameClear:
		/*if (chengeScene->GetTime() > chengeTime)
		{
			scene = Title;

			Reset();
		}

		if (input->GetTrigger(DIK_SPACE) || player->GetA())
		{
			chengeScene->SetPlayFlag();
		}*/
		break;

	case GameOver:

		if (input->GetTrigger(DIK_SPACE))
		{

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

	//�`��p�s��
	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//sound
	sound_ = MyXAudio::Get();
	bgm = sound_->SoundLoadWave("Resources/sound/BGM.wav");
	fanfare = sound_->SoundLoadWave("Resources/sound/fanfare.wav");

	//player
	player->Initialize(dx.get(), shader, pipeline.get());

	float size = 3.0f;

	//��enemy�u��
	//���͌��ߑł�
	//Bkender�Őݒ�ł���悤��+9	
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
	//�X�e�[�W������
	stage->Initialize(dx.get(), shader, pipeline.get());
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	stageWhite->Initialize(dx.get(), shader, pipeline.get());
	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection);

	//�S�[��������
	goal->Initialize(dx.get(), shader, pipeline.get());
	//�V�[���t���O
	scene = Play;

	//�摜�ǂݍ���
	white = dx->LoadTextureGraph(L"Resources/sprite/white1x1.png");
	texP = dx->LoadTextureGraph(L"Resources/sprite/cube.jpg");
	brPng = dx->LoadTextureGraph(L"Resources/sprite/br.png");
	clearTex = dx->LoadTextureGraph(L"Resources/sprite/gameclear.png");
	titleTex = dx->LoadTextureGraph(L"Resources/sprite/Title.png");
	playerTex = dx->LoadTextureGraph(L"Resources/Model/Player/Player.png");
	enemyPng = dx->LoadTextureGraph(L"Resources/Model/ene/enemy.png");

	//imgui������
	imgui->Initialize(dx.get());

	semiArphaSpriteCommon->Inilialize(dx.get(), true);
	normalSpriteCommon->Inilialize(dx.get(), false);

	sprite_->Inilialize(semiArphaSpriteCommon, &matProjection);
	titlePng->Inilialize(normalSpriteCommon, &matProjection);
	titlePng->position = { -680,-420,0 };
	titlePng->scale = { 3600,1440,1 };

	goalFlag = false;
	//stage�t�@�C��
	levelData_ = JsonFileOpen::FileOpen("untitled2");

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
		newModel_->Initialize(dx.get(), shader, pipeline.get());

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

	chengeScene->Initialize(dx.get(), pipeline.get(), matProjection);

	//����炷
	sound_->SoundPlayLoopWave(bgm);
}

void GameScene::Draw()
{
	//Draw
	dx->PrevDrawScreen();

	//// �`��R�}���h

	dx->PostDrawScreen();

	//UIDraw
	dx->PrevDraw();

	//�������狤�ʐ�`��

	//�X�N���[���`��
	screen.Draw(texP);

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

		//�{�b�N�X�p�[�e�B�N��
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
	//�����܂�3D�`��

	//��������2D�`��

	sprite_->PreDraw();

	switch (scene)
	{
	case Title:
		titlePng->Draw(titleTex);
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
		//sprite_->Draw(clearTex);
		titlePng->Draw(clearTex);
		break;

	case GameOver:
		break;

	default:
		break;
	}

	//�V�[���`�F���W�p�`��
	chengeScene->Draw();

	//�����܂�2D�`��

	//Imgui �\�������邩�ǂ���
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

void GameScene::StageReload()
{
	//�f�o�b�O�p
	bool plessZero = input->GetTrigger(DIK_0);
	bool plessNine = input->GetTrigger(DIK_9);

	if (plessZero || plessNine)
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
			newModel_->Initialize(dx.get(), shader, pipeline.get());

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
}
