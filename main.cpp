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
#include "Re//Model.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Sound.h"
#include "Collision.h"
#include "JsonFileOpen.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	std::unique_ptr<Window> win(new Window());

	std::unique_ptr<MyDirectX> dx(new MyDirectX(win.get()));
	int white = dx->LoadTextureGraph(L"Resources/white1x1.png");
	int texP = dx->LoadTextureGraph(L"Resources/cube.jpg");
	int brPng = dx->LoadTextureGraph(L"Resources/br.png");

	MyXAudio sound;
	int bgm = sound.SoundLoadWave("Resources/sound/bgm.wav");

	Controller* controller = nullptr;
	controller = Controller::GetInstance();

	MyDebugCamera debugcamera(Vector3D(0.0f, 30.0f, 10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	MyDebugCamera playcamera(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	std::unique_ptr<ConstBuff> cBuff(new ConstBuff(dx->GetDev(), win->window_width, win->window_height));

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
	screen.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	std::unique_ptr<GPipeline> uiPipeline(new GPipeline(dx->GetDev(), bilShader));

	//スフィア
	Square playText(dx.get(), uiPipeline.get(), bilShader);
	playText.obj.trans.z = 0;
	playText.obj.scale = { 720,420,0.3f };

	Square LifeText(dx.get(), uiPipeline.get(), bilShader);
	LifeText.obj.trans.x = -500;
	LifeText.obj.trans.y = 280;
	LifeText.obj.scale = { 40,40,0.0f };

	Matrix spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);
	LifeText.MatUpdate(Matrix(), spriteProjection, 0);

	//描画用行列
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 60.0f, -150.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//player
	Player* player = new Player();
	player->Initialize(dx.get(), shader, pipeline.get());

	//stage
	Stage* stage = new Stage();
	stage->Initialize(dx.get(), shader, pipeline.get());

	//	ゲームループ
	while (true)
	{
		//WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }

		//Update
		input->Update();
		controller->Update();

		matView.MatUpdate();

		player->Update(matView.mat, matProjection,input.get());
		player->Update(matView.mat, matProjection, controller);

		stage->Update(matView.mat, matProjection,input.get());

		//debugcamera.Update(*input);

		screen.MatUpdate(matView.mat, matProjection, 0);

		matView.eye.x += input->GetKey(DIK_D) - input->GetKey(DIK_A);
		//matView.target.x += input->GetKey(DIK_D) - input->GetKey(DIK_A);
		matView.target.x = player->player.mat.trans.x;

		//ここまで

		if (input->GetTrigger(DIK_ESCAPE))
		{
			break;
		}

		//Draw
		dx->PrevDrawScreen();

		//// 描画コマンド

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		screen.Draw(texP);

		player->Draw(texP);
		stage->Draw(brPng);

		dx->PostDraw();
	}

	delete player;
	delete stage;

	return 0;
}
