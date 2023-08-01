#pragma once
#include "TitleScene.h"
#include "PlayScene.h"

#include "FlameWork.h"

//flamework�Ɉړ�

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

	//�I���t���O
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

	//�`��p�s��
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

	//��enemy�u��
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
	size_t playerTex = 0;
	size_t titleTex = 0;

	//Imgui
	ImguiManager* imgui = new ImguiManager();

	//hitstop
	HitStop* hitStop = new HitStop();

	//sprite
	SpriteCommon* semiArphaSpriteCommon = new SpriteCommon();
	SpriteCommon* normalSpriteCommon = new  SpriteCommon();
	Sprite* sprite_ = new Sprite();
	Sprite* titlePng = new Sprite();

	//�p�[�e�B�N���i�[�p
	std::vector<Particle*> particles_;
	std::vector<BoxParticle*> boxParticles_;
	//Emitter* emitter_ = new Emitter();

	//�X�e�[�W�ǂݍ���
	LevelData* levelData_ = nullptr;

	//�X�e�[�W�i�[�p
	std::map<std::string, Stage*> stages_;
	std::vector<Stage*> objects_;

	//�V�[���`�F���W
	ChengeScene* chengeScene = new ChengeScene();

	bool imguiDrawFlag = true;
	bool goalFlag = false;

	Sphere titleObj;
};