#include "Re//Window.h"
#include "Re//DirectX.h"
#include "Re//Input.h"
#include "Re//GPipeline.h"
#include "Re//Object3D.h"
#include "Re//ConstBuff.h"
#include "Re//TextureData.h"
#include "Re//Shader.h"
#include "Re//MyDebugCamera.h"
#include "Re//Square.h"
#include "Re//Controller.h"
#include <memory>
#include <random>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <map>
#include <imgui.h>
#include <wrl.h>
#include "Re//Model.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Sound.h"
#include "Goal.h"
#include "Collision.h"
#include "JsonFileOpen.h"
#include "Easing.h"
#include "ImguiManager.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	std::unique_ptr<Window> win(new Window());

	std::unique_ptr<MyDirectX> dx(new MyDirectX(win.get()));

	MyDebugCamera debugcamera(Vector3D(0.0f, 30.0f, 10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	MyDebugCamera playcamera(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	std::unique_ptr<ConstBuff> cBuff(new ConstBuff(dx->GetDev(), (float)win->window_width, (float)win->window_height));

	std::unique_ptr<Input> input(new Input(win.get()));
	//Resources/shader/
	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");
	Shader spriteShader(L"SpriteVS.hlsl", L"SpritePS.hlsl");

	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline(new GPipeline(dx->GetDev(), bilShader));

	Square screen(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };

	std::unique_ptr<GPipeline> uiPipeline(new GPipeline(dx->GetDev(), bilShader));

	Matrix spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);

	Square pressText(dx.get(), uiPipeline.get(), bilShader);
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	//描画用行列
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 60.0f, -150.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	MyXAudio* sound_ = MyXAudio::Get();
	size_t bgm = sound_->SoundLoadWave("Resources/sound/BGM.wav");
	size_t fanfare = sound_->SoundLoadWave("Resources/sound/fanfare.wav");

	//player
	Player* player = new Player();
	player->Initialize(dx.get(), shader, pipeline.get());

	//仮enemy置き
	Enemy* enemy = new Enemy();
	enemy->Initialize(dx.get(), shader, pipeline.get());
	enemy->SetTrans(Vector3D{ 100,20,0 });

	Enemy* enemy2 = new Enemy();
	enemy2->Initialize(dx.get(), shader, pipeline.get());
	enemy2->SetTrans(Vector3D{ 300,40,0 });

	Enemy* enemy3 = new Enemy();
	enemy3->Initialize(dx.get(), shader, pipeline.get());
	enemy3->SetTrans(Vector3D{ 600,30,0 });

	Enemy* enemy4 = new Enemy();
	enemy4->Initialize(dx.get(), shader, pipeline.get());
	enemy4->SetTrans(Vector3D{ 800,50,0 });

	//stage
	Stage* stage = new Stage();
	stage->Initialize(dx.get(), shader, pipeline.get());
	float minMapX = stage->stage_.mat.scale.x - 200;
	stage->stage_.mat.trans.x = minMapX;

	Stage* stageWhite = new Stage();
	stageWhite->Initialize(dx.get(), shader, pipeline.get());
	stageWhite->stage_.mat.trans.y += 1;
	stageWhite->stage_.mat.scale.z = 10;
	stageWhite->Update(matView.mat, matProjection, input.get());

	Goal* goal = new Goal();
	goal->Initialize(dx.get(), shader, pipeline.get());

	bool scene = false;
	size_t white = dx->LoadTextureGraph(L"Resources/white1x1.png");
	size_t texP = dx->LoadTextureGraph(L"Resources/cube.jpg");
	size_t brPng = dx->LoadTextureGraph(L"Resources/br.png");
	size_t enemyPng = dx->LoadTextureGraph(L"Resources/ene/enemy.png");
	size_t clearTex = dx->LoadTextureGraph(L"Resources/gameclear.png");

	ImguiManager* imgui = new ImguiManager();
	imgui->Initialize(dx.get());

	//	ゲームループ
	while (true)
	{
		//WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }

		//Update
		input->Update();

		//座標更新
		matView.MatUpdate();

		//Imgui
		imgui->Begin();

		ImGui::InputFloat("matView.eye.x", &matView.eye.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("matView.eye.y", &matView.eye.y, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("matView.eye.z", &matView.eye.z, 0.0f, 10.0f, "%f");

		//ImGuiここまで
		imgui->End();

		if (scene == false)
		{
			//player更新
			player->Update(matView.mat, matProjection);

			//enemy更新
			enemy->Update(matView.mat, matProjection);
			bool sheikF = enemy->BoxCollision(player->GetAttackModel());

			//enemy更新
			enemy2->Update(matView.mat, matProjection);
			sheikF = enemy2->BoxCollision(player->GetAttackModel());

			//enemy更新
			enemy3->Update(matView.mat, matProjection);
			sheikF = enemy3->BoxCollision(player->GetAttackModel());

			//enemy更新
			enemy4->Update(matView.mat, matProjection);
			sheikF = enemy4->BoxCollision(player->GetAttackModel());

			//ステージ更新
			stage->Update(matView.mat, matProjection, input.get());
			goal->Update(matView.mat, matProjection);

			//debugcamera.Update(*input);

			//スクリーン更新
			screen.MatUpdate(matView.mat, matProjection, 0);

			Vector2D moveCamera = { 0,0 };

			moveCamera = player->GetController();

			//targetをplayerに
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
		//ここまで

		//Escapeで抜ける
		if (input->GetTrigger(DIK_ESCAPE))
		{
			break;
		}

		if (input->GetTrigger(DIK_K))
		{
			enemy->enemy_.mat.trans.x -= 1;
		}

		if (input->GetTrigger(DIK_P))
		{
			sound_->SoundPlayWave(bgm);
		}

		//Draw
		dx->PrevDrawScreen();

		//// 描画コマンド

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		//スクリーン描画
		screen.Draw(texP);

		//Actor描画
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
			pressText.Draw((int)clearTex);
		}

		//imgui->Draw(dx.get());

		dx->PostDraw();
	}

	imgui->Finalize();

	return 0;
}
