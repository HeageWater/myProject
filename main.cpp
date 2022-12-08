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
#include <memory>
#include "Re//Model.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	std::unique_ptr<Window> win(new Window());

	std::unique_ptr<MyDirectX> dx(new MyDirectX(win.get()));
	int white = dx->LoadTextureGraph(L"Resources/cube.jpg");
	//int white = dx->LoadTextureGraph(L"Resources/white1x1.png");

	MyDebugCamera debugcamera(Vector3D(0.0f, 0.0f, 900.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	std::unique_ptr<Input> input(new Input(win.get()));

	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline(new GPipeline(dx->GetDev(), bilShader));
	Square screen(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 0.1f;
	screen.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	//描画用行列
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1001.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//Initialize
	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));
	Object3D obj(dx.get(), pipeline.get(), shader);
	Model box(dx.get(), shader, "Resources\\Model\\box.obj", pipeline.get());

	obj.mat.trans = { 0,0,0 };
	obj.mat.scale = { 10,10,10 };
	obj.mat.rotAngle = { 0,0,0 };
	obj.MatUpdate(debugcamera.mat, matProjection);

	//	ゲームループ
	while (true)
	{
		//WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }

		//Update
		input->Update();

		debugcamera.Update(*input);

		screen.MatUpdate(matView.mat, orthoProjection);

		obj.mat.trans = debugcamera.target;

		obj.mat.scale.x += input->GetKey(DIK_RIGHT) - input->GetKey(DIK_LEFT);
		obj.mat.scale.y += input->GetKey(DIK_UP) - input->GetKey(DIK_DOWN);

		obj.mat.rotAngle.y += 0.01;

		obj.MatUpdate(debugcamera.mat, matProjection);

		//Draw
		dx->PrevDrawScreen();

		// 描画コマンド
		obj.Draw(white);

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		screen.Draw(0);

		dx->PostDraw();
	}
	return 0;
}