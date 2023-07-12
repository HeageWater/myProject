#pragma once
#include "Window.h"
#include "DirectX.h"
#include "Input.h"
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "TextureData.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include <memory>
#include <random>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <map>
#include "Model.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Sound.h"
#include "Goal.h"
#include "Collision.h"
#include "JsonFileOpen.h"

class Scene
{
private:
//#pragma region Initialize
//	std::unique_ptr<Window> win(new Window());
//
//	std::unique_ptr<MyDirectX> dx(new MyDirectX(win.get()));
//
//	MyDebugCamera debugcamera(Vector3D(0.0f, 30.0f, 10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
//	MyDebugCamera playcamera(Vector3D(0.0f, 30.0f, 150.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
//
//	std::unique_ptr<ConstBuff> cBuff(new ConstBuff(dx->GetDev(), win->window_width, win->window_height));
//
//	std::unique_ptr<Input> input(new Input(win.get()));
//	//Resources/shader/
//	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
//	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");
//	Shader spriteShader(L"SpriteVS.hlsl", L"SpritePS.hlsl");
//
//	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));
//
//	//ï`âÊèâä˙âª
//	std::unique_ptr<GPipeline> multipathPipeline(new GPipeline(dx->GetDev(), bilShader));
//
//	Square screen(dx.get(), multipathPipeline.get(), bilShader);
//	screen.obj.trans.z = 100.1f;
//	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };
//
//	std::unique_ptr<GPipeline> uiPipeline(new GPipeline(dx->GetDev(), bilShader));
//
//	Matrix spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);
//
//	Square pressText(dx.get(), uiPipeline.get(), bilShader);
//	pressText.obj.trans.y = -200;
//	pressText.obj.scale = { Window::window_width,Window::window_height ,0.2f };
//	pressText.MatUpdate(Matrix(), spriteProjection, 0);
//
//	//ï`âÊópçsóÒ
//	MyMath::MatView matView;
//	matView.Init(Vector3D(0.0f, 60.0f, -150.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
//	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
//	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);
//
//	//player
//	Player* player = new Player();
//
//	//enemy
//	Enemy* enemy = new Enemy();
//	Enemy* enemy2 = new Enemy();
//	Enemy* enemy3 = new Enemy();
//	Enemy* enemy4 = new Enemy();
//
//	//stage
//	Stage* stage = new Stage();
//
//	Stage* stageWhite = new Stage();
//
//	Goal* goal = new Goal();
//
//	bool scene = false;
//	int white = dx->LoadTextureGraph(L"Resources/white1x1.png");
//	int texP = dx->LoadTextureGraph(L"Resources/cube.jpg");
//	int brPng = dx->LoadTextureGraph(L"Resources/br.png");
//	size_t enemyPng = dx->LoadTextureGraph(L"Resources/ene/enemy.png");
//	int clearTex = dx->LoadTextureGraph(L"Resources/gameclear.png");

public:
	Scene();
	~Scene();
	void Initialize();
	void Run();
};