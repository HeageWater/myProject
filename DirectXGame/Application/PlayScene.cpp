#include "PlayScene.h"

void PlayScene::Update()
{
	//if (scene == false && hitStop->GetTime() < 1)
	//{
	//	//player�X�V
	//	player->Update(matView.mat, matProjection, dx.get(), shader, pipeline.get());

	//	//enemy�X�V
	//	enemy->Update(matView.mat, matProjection);
	//	bool sheikF = enemy->BoxCollision(player->GetAttackModel());

	//	float setStopTime = 7.0f;

	//	if (sheikF)
	//	{
	//		hitStop->SetTime(setStopTime);
	//	}

	//	//enemy�X�V
	//	enemy2->Update(matView.mat, matProjection);
	//	sheikF = enemy2->BoxCollision(player->GetAttackModel());

	//	if (sheikF)
	//	{
	//		hitStop->SetTime(setStopTime);
	//	}

	//	//enemy�X�V
	//	enemy3->Update(matView.mat, matProjection);
	//	sheikF = enemy3->BoxCollision(player->GetAttackModel());

	//	if (sheikF)
	//	{
	//		hitStop->SetTime(setStopTime);
	//	}

	//	//enemy�X�V
	//	enemy4->Update(matView.mat, matProjection);
	//	sheikF = enemy4->BoxCollision(player->GetAttackModel());

	//	if (sheikF)
	//	{
	//		hitStop->SetTime(setStopTime);
	//	}

	//	//�X�e�[�W�X�V
	//	stage->Update(matView.mat, matProjection);
	//	goal->Update(matView.mat, matProjection);

	//	bool hit = player->CollisionAttackToEnemy(enemy->enemy_);




	//	//debugcamera.Update(*input);

	//	//�p�[�e�B�N��
	//	for (size_t i = 0; i < boxParticles_.size(); i++)
	//	{
	//		boxParticles_[i]->Update(matView.mat, matProjection);

	//		//�폜
	//		if (boxParticles_[i]->IsDead() == true)
	//		{
	//			boxParticles_.erase(boxParticles_.begin() + i);
	//		}
	//	}

	//	//�X�N���[���X�V
	//	screen.MatUpdate(matView.mat, matProjection, 0);

	//	//Vector2D moveCamera = { 0,0 };

	//	//moveCamera = player->GetController();

	//	for (auto& object : objects_)
	//	{
	//		object->stage_.mat.trans.x -= (float)(input->GetKey(DIK_D) - input->GetKey(DIK_A));
	//		object->stage_.mat.trans.y -= (float)(input->GetKey(DIK_S) - input->GetKey(DIK_W));
	//		object->stage_.mat.trans.z -= (float)(input->GetKey(DIK_E) - input->GetKey(DIK_Q));

	//		object->Update(matView.mat, matProjection);
	//	}

	//	//target��player��
	//	//matView.eye.x += dev[19]; //moveCamera.x;
	//	matView.eye.x = player->GetPos().x; //moveCamera.x;
	//	matView.target.x = player->GetPos().x;// -dev[0];

	//	matView.eye.x = min(matView.eye.x, 1050);
	//	matView.eye.x = max(matView.eye.x, 0);

	//	//stage->stage_.mat.trans.x = max(stage->stage_.mat.trans.x, minMapX);
	//	bool checkGoal = goal->BoxCollision(player->GetModel());

	//	if (checkGoal)
	//	{
	//		scene = true;
	//	}
	//}

	////�����܂�
	//pressText.MatUpdate(Matrix(), spriteProjection);

	//hitStop->Update();

	//sprite_->Update();

	//chengeScene->Update(matView.mat, spriteProjection);

	////�p�[�e�B�N������
	//if (input->GetTrigger(DIK_SPACE))
	//{
	//	//emitter_->Create();

	//	size_t play = MyMath::GetRandom(10, 30);

	//	for (size_t i = 0; i < play; i++)
	//	{
	//		BoxParticle* newP = new BoxParticle();

	//		newP->Initialize(dx.get(), shader, pipeline.get());

	//		newP->SetPos(player->GetPos());

	//		boxParticles_.push_back(newP);
	//	}
	//}
}

void PlayScene::Initilize()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Finalize()
{
}
