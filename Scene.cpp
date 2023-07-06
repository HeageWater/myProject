//#include "Scene.h"
//
//Scene::Scene()
//{
//}
//
//Scene::~Scene()
//{
//}
//
//void Scene::Initialize()
//{
//	player->Initialize(dx.get(), shader, pipeline.get());
//
//	//enemy
//	enemy->Initialize(dx.get(), shader, pipeline.get());
//	enemy->SetTrans(Vector3D{ 100,20,0 });
//
//	enemy2->Initialize(dx.get(), shader, pipeline.get());
//	enemy2->SetTrans(Vector3D{ 300,40,0 });
//
//	enemy3->Initialize(dx.get(), shader, pipeline.get());
//	enemy3->SetTrans(Vector3D{ 600,30,0 });
//
//	enemy4->Initialize(dx.get(), shader, pipeline.get());
//	enemy4->SetTrans(Vector3D{ 800,50,0 });
//
//	//stage
//	stage->Initialize(dx.get(), shader, pipeline.get());
//	float minMapX = stage->stage_.mat.scale.x - 200;
//	stage->stage_.mat.trans.x = minMapX;
//
//	stageWhite->Initialize(dx.get(), shader, pipeline.get());
//	stageWhite->stage_.mat.trans.y += 1;
//	stageWhite->stage_.mat.scale.z = 10;
//	stageWhite->Update(matView.mat, matProjection, input.get());
//
//	goal->Initialize(dx.get(), shader, pipeline.get());
//
//}
//
//void Scene::Run()
//{
//	//	�Q�[�����[�v
//	while (true)
//	{
//		//WinMsg
//		win->MsgUpdate();
//		if (win->EndLoop()) { break; }
//
//		//Update
//		input->Update();
//
//		//���W�X�V
//		matView.MatUpdate();
//
//		if (scene == false)
//		{
//			//player�X�V
//			player->Update(matView.mat, matProjection);
//
//			//enemy�X�V
//			enemy->Update(matView.mat, matProjection);
//			bool sheikF = enemy->BoxCollision(player->playerAttack_);
//
//			//enemy�X�V
//			enemy2->Update(matView.mat, matProjection);
//			sheikF = enemy2->BoxCollision(player->playerAttack_);
//
//			//enemy�X�V
//			enemy3->Update(matView.mat, matProjection);
//			sheikF = enemy3->BoxCollision(player->playerAttack_);
//
//			//enemy�X�V
//			enemy4->Update(matView.mat, matProjection);
//			sheikF = enemy4->BoxCollision(player->playerAttack_);
//
//			//�X�e�[�W�X�V
//			stage->Update(matView.mat, matProjection, input.get());
//			goal->Update(matView.mat, matProjection);
//
//			//debugcamera.Update(*input);
//
//			//�X�N���[���X�V
//			screen.MatUpdate(matView.mat, matProjection, 0);
//
//			Vector2D moveCamera = { 0,0 };
//
//			moveCamera = player->GetController();
//
//			//target��player��
//			matView.eye.x += moveCamera.x;
//			matView.target.x = player->player_.mat.trans.x;
//
//			matView.eye.x = min(matView.eye.x, 1050);
//			matView.eye.x = max(matView.eye.x, 0);
//
//			//stage->stage_.mat.trans.x = max(stage->stage_.mat.trans.x, minMapX);
//			bool checkGoal = goal->BoxCollision(player->player_);
//
//			if (checkGoal)
//			{
//				scene = true;
//			}
//		}
//		//�����܂�
//
//		//Escape�Ŕ�����
//		if (input->GetTrigger(DIK_ESCAPE))
//		{
//			break;
//		}
//
//		//Draw
//		dx->PrevDrawScreen();
//
//		//// �`��R�}���h
//
//		dx->PostDrawScreen();
//
//		//UIDraw
//		dx->PrevDraw();
//
//		//�X�N���[���`��
//		screen.Draw(texP);
//
//		//Actor�`��
//		player->Draw(texP, white);
//		enemy->Draw(enemyPng);
//		enemy2->Draw(enemyPng);
//		enemy3->Draw(enemyPng);
//		enemy4->Draw(enemyPng);
//		stage->Draw(brPng);
//		stageWhite->Draw(white);
//		goal->Draw(white);
//
//		if (scene == true)
//		{
//			pressText.Draw(clearTex);
//		}
//
//		dx->PostDraw();
//	}
//}
