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
#include "Sound.h"
#include "Collision.h"
#include "JsonFileOpen.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	std::unique_ptr<Window> win_(new Window());

	std::unique_ptr<MyDirectX> dx_(new MyDirectX(win_.get()));
	size_t white_ = dx_->LoadTextureGraph(L"Resources/white1x1.png");

	MyXAudio sound_;
	//size_t bgm = sound_.SoundLoadWave("Resources/sound/bgm.wav");

	Controller* controller_ = nullptr;
	controller_ = Controller::GetInstance();

	MyDebugCamera debugcamera_(Vector3D(0.0f, 30.0f, 10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	MyDebugCamera playcamera_(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	std::unique_ptr<ConstBuff> cBuff_(new ConstBuff(dx_->GetDev(), win_->window_width, win_->window_height));

	std::unique_ptr<Input> input_(new Input(win_.get()));
	//Resources/shader/
	Shader shader_(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader_(L"VShader.hlsl", L"PShader.hlsl");
	Shader spriteShader_(L"SpriteVS.hlsl", L"SpritePS.hlsl");

	std::unique_ptr<GPipeline> pipeline_(new GPipeline(dx_->GetDev(), shader_));

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline_(new GPipeline(dx_->GetDev(), bilShader_));

	Square screen_(dx_.get(), multipathPipeline_.get(), bilShader_);
	screen_.obj.trans_.z_ = 0.1f;
	screen_.obj.scale_ = { Window::window_width / 2,Window::window_height / 2,0.2f };

	std::unique_ptr<GPipeline> uiPipeline_(new GPipeline(dx_->GetDev(), bilShader_));

	//スフィア
	Square playText_(dx_.get(), uiPipeline_.get(), bilShader_);
	playText_.obj.trans_.z_ = 0;
	playText_.obj.scale_ = { 720,420,0.3f };

	Square LifeText_(dx_.get(), uiPipeline_.get(), bilShader_);
	LifeText_.obj.trans_.x_ = -500;
	LifeText_.obj.trans_.y_ = 280;
	LifeText_.obj.scale_ = { 40,40,0.0f };

	Matrix spriteProjection_ = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);
	LifeText_.MatUpdate(Matrix(), spriteProjection_, 0);

	//描画用行列
	MyMath::MatView matView_;
	matView_.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection_ = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection_ = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//ここから
	LevelData* levelData_ = nullptr;

	//親子ありファイル
	levelData_ = JsonFileOpen::FileOpen("test2");
	//levelData = JsonFileOpen::FileOpen("TestStage");

	//複数個ファイル
	///levelData = JsonFileOpen::FileOpen("Test");

	std::map<std::string, Model*> models_;
	std::vector<Model*> objects_;

	//レベルデータからオブジェクトに生成、配置
	for (auto& objectdata : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model_ = nullptr;
		decltype(models_)::iterator it = models_.find(objectdata.fileName);

		//終わりか
		if (it != models_.end())
		{
			model_ = it->second;
		}

		//モデルを指定して3Dオブジェクトを生成
		Model* newModel_ = new Model();
		newModel_->Initialize(dx_.get(), shader_, "Resources\\Model\\box.obj", pipeline_.get());

		//trans
		newModel_->mat_.trans_ = objectdata.translation;

		//rotation
		newModel_->mat_.rotAngle_ = objectdata.rotation;

		//scale;
		newModel_->mat_.scale_ = objectdata.scaling;

		//Update
		newModel_->MatUpdate(debugcamera_.mat, matProjection_);

		//格納
		objects_.push_back(newModel_);
	}

	//	ゲームループ
	while (true)
	{
		//WinMsg
		win_->MsgUpdate();
		if (win_->EndLoop()) { break; }

		//Update
		input_->Update();
		controller_->Update();

		debugcamera_.Update(*input_);

		screen_.MatUpdate(matView_.mat_, orthoProjection_, 0);

		//ここまで

		if (input_->GetTrigger(DIK_ESCAPE))
		{
			break;
		}

		if (input_->GetTrigger(DIK_R))
		{
			
		}

		//読み込んだモデルのUpdate
		for (auto& object : objects_)
		{
			object->mat_.trans_.x_ -= (float)(input_->GetKey(DIK_D) - input_->GetKey(DIK_A));
			object->mat_.trans_.y_ -= (float)(input_->GetKey(DIK_S) - input_->GetKey(DIK_W));
			object->mat_.trans_.z_ -= (float)(input_->GetKey(DIK_E) - input_->GetKey(DIK_Q));
																					  
			object->MatUpdate(debugcamera_.mat, matProjection_);
		}

		//Draw
		dx_->PrevDrawScreen();

		//読み込んだモデルのDraw(White)
		for (auto& object : objects_) {
			object->Draw(white_);

			object->mat_.trans_;
		}

		//// 描画コマンド

		dx_->PostDrawScreen();

		//UIDraw
		dx_->PrevDraw();

		screen_.Draw(0);

		dx_->PostDraw();
	}

	for (auto& object : objects_)
	{
		delete object;
	}

	return 0;
}
