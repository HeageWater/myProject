#pragma once
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Sound.h"
#include "Goal.h"
#include "Collision.h"
#include "JsonFileOpen.h"
#include "Scene.h"

class Scene
{
private:
	//player
	Player* player = new Player();

	//enemy
	Enemy* enemy = new Enemy();
	Enemy* enemy2 = new Enemy();
	Enemy* enemy3 = new Enemy();
	Enemy* enemy4 = new Enemy();

	//stage
	Stage* stage = new Stage();

	Stage* stageWhite = new Stage();

	Goal* goal = new Goal();

	/*int white = dx->LoadTextureGraph(L"Resources/white1x1.png");
	int texP = dx->LoadTextureGraph(L"Resources/cube.jpg");
	int brPng = dx->LoadTextureGraph(L"Resources/br.png");
	size_t enemyPng = dx->LoadTextureGraph(L"Resources/ene/enemy.png");
	int clearTex = dx->LoadTextureGraph(L"Resources/gameclear.png");*/

public:

	void Initialize();
};