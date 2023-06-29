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
	screen.obj.trans.z = 0.1f;
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
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//ここから
	LevelData* levelData = nullptr;

	//親子ありファイル
	levelData = JsonFileOpen::FileOpen("0620");

	//複数個ファイル
	//levelData = JsonFileOpen::FileOpen("Test");

	std::map<std::string, Model*> models;
	std::vector<Model*> objects;

	//レベルデータからオブジェクトに生成、配置
	for (auto& objectdata : levelData->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectdata.fileName);

		//終わりか
		if (it != models.end())
		{
			model = it->second;
		}

		//モデルを指定して3Dオブジェクトを生成
		Model* newModel = new Model();
		newModel->Initialize(dx.get(), shader, "Resources\\Model\\box.obj", pipeline.get());

		//trans
		newModel->mat.trans = objectdata.translation;
		newModel->mat.trans.y = -newModel->mat.trans.y;

		newModel->mat.trans = { 0,0,0 };

		//rotation
		newModel->mat.rotAngle = objectdata.rotation;

		//scale;
		newModel->mat.scale = objectdata.scaling;

		//Update
		newModel->MatUpdate(matView.mat, orthoProjection);

		//格納
		objects.push_back(newModel);
	}

	Player* player = new Player();
	player->Initialize(dx.get(), shader, pipeline.get());


	//	ゲームループ
	while (true)
	{
		//WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }

		//Update
		input->Update();
		controller->Update();

		player->Update(matView.mat, orthoProjection);

		//debugcamera.Update(*input);

		screen.MatUpdate(matView.mat, orthoProjection, 0);

		//ここまで

		if (input->GetTrigger(DIK_ESCAPE))
		{
			break;
		}

		if (input->GetTrigger(DIK_SPACE))
		{
			//matView.eye.x = objects[2]->mat.trans.x;
		}

		//読み込んだモデルのUpdate
		for (auto& object : objects)
		{
			//object->mat.trans.x -= input->GetKey(DIK_D) - input->GetKey(DIK_A);
			//object->mat.trans.y -= input->GetKey(DIK_S) - input->GetKey(DIK_W);
			//object->mat.trans.z -= input->GetKey(DIK_E) - input->GetKey(DIK_Q);

			object->MatUpdate(matView.mat, orthoProjection);

			//break;
		}

		player->player.mat.trans.x += input->GetKey(DIK_D) - input->GetKey(DIK_A);
		player->player.mat.trans.y -= input->GetKey(DIK_S) - input->GetKey(DIK_W);
		player->player.mat.trans.z -= input->GetKey(DIK_E) - input->GetKey(DIK_Q);

		//Draw
		dx->PrevDrawScreen();

		//読み込んだモデルのDraw(White)
		for (auto& object : objects) {
			object->Draw(white);

			object->mat.trans;
		}

		//// 描画コマンド

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		screen.Draw(0);

		player->Draw(texP);

		dx->PostDraw();
	}

	for (auto& object : objects)
	{
		delete object;
	}

	return 0;
}
