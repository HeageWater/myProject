#pragma once
#include "Window.h"
#include "DirectX.h"
#include "Input.h"
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "TextureData.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include <memory>
#include <random>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <map>
#include <imgui.h>
#include <wrl.h>
#include "Model.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Sound.h"
#include "Goal.h"
#include "Collision.h"
#include "JsonFileOpen.h"
#include "Easing.h"
#include "ImguiManager.h"
#include "Shader.h"

class GameScene
{
public:
	GameScene();
	~GameScene();
	void Update();
	void Initilize();
	void Draw();
	void Finalize();
	void Run();
	
	bool IsEndRequst() { return endRequest_; };
	void SetEndRwqust(bool flag) { endRequest_ = flag; };
private:

	//終了フラグ
	bool endRequest_ = false;

	//windowApi
	std::unique_ptr<Window> win;

	//dxCommon
	std::unique_ptr<MyDirectX> dx;

	//camera
	MyDebugCamera debugcamera;
	MyDebugCamera playcamera;

	//buff
	std::unique_ptr<ConstBuff> cBuff;

	//input
	std::unique_ptr<Input> input;

	//shader
	Shader shader;
	Shader bilShader;
	Shader spriteShader;

	//pipeline
	std::unique_ptr<GPipeline> pipeline;

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline;

	//screen
	Square screen;

	//gpipeline
	std::unique_ptr<GPipeline> uiPipeline;

	//sprite
	Matrix spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);

	//tex
	Square pressText;

	//描画用行列
	MyMath::MatView matView;
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//sound
	MyXAudio* sound_;
	size_t bgm = 0;
	size_t fanfare = 0;

	//player
	Player* player = new Player();

	//仮enemy置き
	Enemy* enemy = new Enemy();
	Enemy* enemy2 = new Enemy();
	Enemy* enemy3 = new Enemy();
	Enemy* enemy4 = new Enemy();

	//stage
	Stage* stage = new Stage();

	//stage2
	Stage* stageWhite = new Stage();

	//goal
	Goal* goal = new Goal();

	//sceneFlag
	bool scene = false;

	//tex
	size_t white=0;
	size_t texP = 0;
	size_t brPng = 0;
	size_t enemyPng = 0;
	size_t clearTex = 0;

	//Imgui
	ImguiManager* imgui = new ImguiManager();
};