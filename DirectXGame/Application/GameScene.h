#pragma once
#include "TitleScene.h"
#include "PlayScene.h"

#include "FlameWork.h"

//flameworkに移動

enum Scene
{
	Title = 0,
	Select = 1,
	Play = 2,
	Pause = 3,
	Movie = 4,
	GameClear = 5,
	GameOver = 6,
};

class GameScene :public FlameWork
{
public:
	void Update() override;
	void Initilize()override;
	void Draw()override;
	void Finalize()override;
	void Run()override;
	void Reset();

	bool IsEndRequst() { return endRequest_; };
	void SetEndRwqust(bool flag) { endRequest_ = flag; };
	void StageReload();
private:
	void CreatePatricle(Vector3D pos);

	//ステージ読み込み
	void StageLoad();

	//終了フラグ
	bool endRequest_ = false;

	//camera
	MyDebugCamera debugcamera;
	MyDebugCamera playcamera;

	//screen
	Square screen;

	//tex
	Square pressText;

	//input
	std::unique_ptr<Input> input;
	//std::unique_ptr<Controller> controller;
	Controller* controller = nullptr;

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
	size_t playerHit = 0;
	size_t enemyHit = 0;

	//player
	Player* player = new Player();

	//Warp
	Warp* warp = new Warp();

	//仮enemy置き
	Enemy* enemy = new Enemy();
	Enemy* enemy2 = new Enemy();
	Enemy* enemy3 = new Enemy();
	Enemy* enemy4 = new Enemy();
	std::vector<Enemy*> enemys_;

	//stage
	Stage* stage = new Stage();

	//stage2
	Stage* stageWhite = new Stage();

	//goal
	Goal* goal = new Goal();

	//sceneFlag
	size_t scene = 0;

	//tex
	size_t white = 0;
	size_t texP = 0;
	size_t brPng = 0;
	size_t enemyPng = 0;
	size_t clearTex = 0;
	size_t overTex = 0;
	size_t playerTex = 0;
	size_t titleTex = 0;
	size_t heartLesTex = 0;
	size_t heartHaveTex = 0;
	size_t lifeTex = 0;
	size_t blackTex = 0;
	size_t backTex = 0;
	size_t LTex = 0;
	size_t RTex = 0;
	size_t AbuttonTex = 0;
	size_t PressTex = 0;
	size_t LTTex = 0;

	//Imgui
	ImguiManager* imgui = new ImguiManager();

	//hitstop
	HitStop* hitStop = new HitStop();

	//sprite
	SpriteCommon* semiArphaSpriteCommon = new SpriteCommon();
	SpriteCommon* normalSpriteCommon = new  SpriteCommon();
	Sprite* sprite_ = new Sprite();
	Sprite* titlePng = new Sprite();
	Sprite* lifePng = new Sprite();
	Sprite* lesPng = new Sprite();
	Sprite* lesPng2 = new Sprite();
	Sprite* lesPng3 = new Sprite();
	Sprite* havePng = new Sprite();
	Sprite* havePng2 = new Sprite();
	Sprite* havePng3 = new Sprite();
	Sprite* UILStick = new Sprite();
	Sprite* UIRStick = new Sprite();
	Sprite* UILT = new Sprite();
	Sprite* UIAButton = new Sprite();
	Sprite* UIPress = new Sprite();


	//パーティクル格納用
	std::vector<Particle*> particles_;
	std::vector<BoxParticle*> boxParticles_;
	//Emitter* emitter_ = new Emitter();

	//ステージ読み込み
	LevelData* levelData_ = nullptr;

	//ステージ格納用
	std::map<std::string, Stage*> stages_;
	std::vector<Stage*> objects_;

	//シーンチェンジ
	ChengeScene* chengeScene = new ChengeScene();

	bool imguiDrawFlag = true;
	bool goalFlag = false;
	bool overFlag = false;

	Sphere titleObj;
};