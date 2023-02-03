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
#include "Collision.h"

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

	//描画用行列
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//Initialize
	Player player;
	player.Initialize(dx.get(), shader, pipeline.get());

	player.player.mat.trans = { 5,0,0 };
	player.player.mat.scale = { 15,15,15 };
	player.player.mat.rotAngle = { 0,0,0 };

	Model kyu;
	kyu.Initialize(dx.get(), shader, "Resources\\kyu\\kyu.obj", pipeline.get());

	kyu.mat.trans = { 15,0,0 };
	kyu.mat.scale = { 1,1,1 };
	kyu.mat.rotAngle = { 0,0,0 };

	Model tri;
	tri.Initialize(dx.get(), shader, "Resources\\tiriangle\\tiriangle.obj", pipeline.get());

	tri.mat.trans = { 0,0,0 };
	tri.mat.scale = { 1.0,1.0,1.0 };
	tri.mat.rotAngle = { 0,0,1.55f };

	//sound.SoundPlayLoopWave(bgm);

	bool drawFlag = false;

	Sphere sphere;

	sphere.center = Vector3D(0, 2, 1);
	sphere.radius = 1.0;

	Plane plane;

	plane.normal = Vector3D(0, 1, 0);
	plane.distance = 0.0f;

	Triangle triangle;

	triangle.p0 = { 0.0f, 0.0f,0.0f };
	triangle.p1 = { -5.0f, 0.0f,+5.0f };
	triangle.p2 = { +5.0f, 0.0f ,-5.0f };
	triangle.normal = { 0.0f,1.0f,0.0f };

	int NowFlag = 1;

	bool hit = false;
	bool hit2 = false;

	int count = 0;

	float spd = -0.02f;

	//	ゲームループ
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

		//ここまで

		if (input->GetTrigger(DIK_ESCAPE))
		{
			break;
		}

		if (input->GetKey(DIK_1))
		{
			NowFlag = 1;
			count = 0;
			spd = -0.02f;

			sphere.center = Vector3D(0, 2, 1);
			sphere.radius = 1.0;

			player.player.mat.trans = { 5,0,0 };
			player.player.mat.scale = { 15,15,15 };
			player.player.mat.rotAngle = { 0,0,0 };

			kyu.mat.trans = { 15,0,0 };
			kyu.mat.scale = { 1,1,1 };
			kyu.mat.rotAngle = { 0,0,0 };
		}

		if (input->GetKey(DIK_2))
		{
			NowFlag = 2;
			count = 0;
			spd = -0.02f;

			sphere.center = Vector3D(0, 3, 1);
			sphere.radius = 1.0;

			kyu.mat.trans = { 15,5,0 };
			kyu.mat.scale = { 1,1,1 };
			kyu.mat.rotAngle = { 0,0,0 };

			tri.mat.trans = { 0,0,0 };
			tri.mat.scale = { 1.0,1.0,1.0 };
			tri.mat.rotAngle = { 0,0,1.55f };

			triangle.p0 = { 0.0f, 0.0f,0.0f };
			triangle.p1 = { -5.0f, 0.0f,+5.0f };
			triangle.p2 = { +5.0f, 0.0f ,-5.0f };
			triangle.normal = { 0.0f,1.0f,0.0f };
		}

		if (NowFlag == 1)
		{
			kyu.mat.trans = sphere.center;
			player.player.mat.trans = plane.normal;

			player.player.mat.trans.y -= 1.0f;

			player.player.mat.scale = { 10,0,10 };
			kyu.mat.scale = { sphere.radius,sphere.radius,sphere.radius };

			count++;

			if (count > 200)
			{
				spd = -spd;
				count = 0;
			}

			sphere.center.y += spd;

			player.Update(debugcamera.mat, matProjection);
			kyu.MatUpdate(debugcamera.mat, matProjection);

			hit = Collision::CheckSphereToPlane(sphere, plane);
		}

		if (NowFlag == 2)
		{
			kyu.mat.trans = sphere.center;

			kyu.mat.scale = { sphere.radius -0.25f,sphere.radius - 0.25f,sphere.radius - 0.25f };

			tri.mat.trans = triangle.p0;

			tri.mat.scale = { 2.0f,2.0f,2.0f };

			count++;

			if (count > 200)
			{
				spd = -spd;
				count = 0;
			}

			sphere.center.y += spd;

			tri.MatUpdate(debugcamera.mat, matProjection);
			kyu.MatUpdate(debugcamera.mat, matProjection);

			hit2 = Collision::CheckSphereToTriangle(sphere, triangle);
		}

		//Draw
		dx->PrevDrawScreen();

		if (NowFlag == 1)
		{
			player.Update(debugcamera.mat, matProjection);
			kyu.MatUpdate(debugcamera.mat, matProjection);

			if (hit)
			{
				player.Draw(texP);
				kyu.Draw(texP);
			}
			else
			{
				player.Draw(brPng);
				kyu.Draw(brPng);
			}
		}

		if (NowFlag == 2)
		{
			tri.MatUpdate(debugcamera.mat, matProjection);
			kyu.MatUpdate(debugcamera.mat, matProjection);

			if (hit2)
			{
				kyu.Draw(brPng);
				tri.Draw(brPng);
			}
			else
			{
				kyu.Draw(texP);
				tri.Draw(texP);
			}
		}


		//// 描画コマンド

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		screen.Draw(0);

		dx->PostDraw();
	}
	return 0;
}
