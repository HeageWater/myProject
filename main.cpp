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
#include "Sound.h"
#include "Collision.h"
#include "JsonFileOpen.h"
#include "ImGUi.h"

#include "GameScene.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	std::unique_ptr<Window> win_(new Window());

	std::unique_ptr<MyDirectX> dx_(new MyDirectX(win_.get()));
	size_t white_ = dx_->LoadTextureGraph(L"Resources/white1x1.png");
	size_t playerPng_ = dx_->LoadTextureGraph(L"Resources/cube.jpg");
	size_t stagePng_ = dx_->LoadTextureGraph(L"Resources/br.png");

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

	//player
	Player* player_ = new Player();
	player_ ->Initialize(dx_.get(), shader_, pipeline_.get());

	Stage* stage_ = new Stage();
	stage_->Initialize(dx_.get(), shader_, pipeline_.get());

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

		player_->Update(matView_.mat_, matProjection_);

		stage_->Update(matView_.mat_, matProjection_);

		//ここまで

		if (input_->GetTrigger(DIK_ESCAPE))
		{
			break;
		}

		//Draw
		dx_->PrevDrawScreen();

		//// 描画コマンド

		dx_->PostDrawScreen();

		//UIDraw
		dx_->PrevDraw();

		screen_.Draw(0);

		player_->Draw(playerPng_);

		stage_->Draw(stagePng_);

		dx_->PostDraw();
	}

	return 0;
}
