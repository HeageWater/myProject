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
#include "Re//Model.h"
#include "Player.h"
#include "Sound.h"

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

	MyDebugCamera debugcamera(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	MyDebugCamera playcamera(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	std::unique_ptr<ConstBuff> cBuff(new ConstBuff(dx->GetDev(), win->window_width, win->window_height));

	std::unique_ptr<Input> input(new Input(win.get()));
	//Resources/shader/
	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");
	Shader spriteShader(L"SpriteVS.hlsl", L"SpritePS.hlsl");

	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));

	//ï`âÊèâä˙âª
	std::unique_ptr<GPipeline> multipathPipeline(new GPipeline(dx->GetDev(), bilShader));

	Square screen(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 0.1f;
	screen.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	std::unique_ptr<GPipeline> uiPipeline(new GPipeline(dx->GetDev(), bilShader));

	Square playText(dx.get(), uiPipeline.get(), bilShader);
	playText.obj.trans.z = 0;
	playText.obj.scale = { 720,420,0.3f };

	Square LifeText(dx.get(), uiPipeline.get(), bilShader);
	LifeText.obj.trans.x = -500;
	LifeText.obj.trans.y = 280;
	LifeText.obj.scale = { 40,40,0.0f };

	Matrix spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);
	LifeText.MatUpdate(Matrix(), spriteProjection, 0);

	//ï`âÊópçsóÒ
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//Initialize
	Player player;
	player.Initialize(dx.get(), shader, pipeline.get());

	player.player.mat.trans = { 5,0,0 };
	player.player.mat.scale = { 5,5,5 };
	player.player.mat.rotAngle = { 0,0,0 };

	sound.SoundPlayLoopWave(bgm);

	bool drawFlag = false;
	//	ÉQÅ[ÉÄÉãÅ[Év
	while (true)
	{
		//WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }

		//Update
		input->Update();
		controller->Update();

		debugcamera.Update(*input);

		screen.MatUpdate(matView.mat, orthoProjection, 0);

		//Ç±Ç±Ç‹Ç≈

		if (input->GetTrigger(DIK_SPACE))
		{
			drawFlag = !drawFlag;
		}

		if (input->GetTrigger(DIK_ESCAPE))
		{
			break;
		}


		if (input->GetTrigger(DIK_R) && drawFlag)
		{
			player.Reset();
			debugcamera.Init(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
		}

		player.player.mat.trans.x -= input->GetKey(DIK_D) - input->GetKey(DIK_A);
		player.player.mat.trans.y += input->GetKey(DIK_W) - input->GetKey(DIK_S);

		Vector2D move = controller->GetLeftStickVec();

		if (move.x > 0.5f || move.x < -0.5f)
		{
			player.player.mat.trans.x -= controller->GetLeftStickVec().x;
		}

		if (move.y > 0.5f || move.y < -0.5f)
		{
			player.player.mat.trans.y -= controller->GetLeftStickVec().y;
		}

		//Draw
		dx->PrevDrawScreen();

		// ï`âÊÉRÉ}ÉìÉh
		player.Draw(texP);

		if (drawFlag)
		{
			LifeText.Draw(brPng);
			player.Update(debugcamera.mat, matProjection);
		}
		else
		{
			LifeText.Draw(white);
			player.Update(playcamera.mat, matProjection);
		}

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		screen.Draw(0);

		dx->PostDraw();
	}
	return 0;
}