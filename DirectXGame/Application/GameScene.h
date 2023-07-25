#pragma once
#include "Input.h"
#include "Object3D.h"
#include "TextureData.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include "Model.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Sound.h"
#include "Goal.h"
#include "Collision.h"
#include "Easing.h"
#include "ImguiManager.h"
#include "Shader.h"
#include "HitStop.h"
#include "Shake.h"
#include "Sprite.h"
//#include "Particle.h"
#include "Emitter.h"

#include "FlameWork.h"

//flameworkに移動
#include "Window.h"
#include "DirectX.h"
#include "JsonFileOpen.h"
#include "ConstBuff.h"
#include "GPipeline.h"
#include <memory>
#include <random>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <map>
#include <imgui.h>
#include <wrl.h>


class GameScene :public FlameWork
{
public:
	void Update() override;
	void Initilize()override;
	void Draw()override;
	void Finalize()override;
	void Run()override;

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

	//tex
	Square pressText;

	//描画用行列
	MyMath::MatView matView;
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);
	//sprite
	Matrix spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);

	//sound
	MyXAudio* sound_ = nullptr;
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
	size_t white = 0;
	size_t texP = 0;
	size_t brPng = 0;
	size_t enemyPng = 0;
	size_t clearTex = 0;

	////const uint32_t size = 20;
	//float dev[20];

	//Imgui
	ImguiManager* imgui = new ImguiManager();

	//hitstop
	HitStop* hitStop = new HitStop();

	//sprite
	SpriteCommon* spriteCommon = new SpriteCommon();
	Sprite* sprite_ = new Sprite();

	std::vector<Particle*> particles_;
	Particle* copyParticle_ = new Particle;
	//Emitter* emitter_ = new Emitter();
};