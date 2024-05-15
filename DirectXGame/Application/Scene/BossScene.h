#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Controller.h"
#include "Sound.h"

//このシーンにのみ追加するもの
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Goal.h"
#include "Collision.h"
#include "HitStop.h"
#include "Shake.h"
#include "Emitter.h"
#include "ChengeScene.h"
#include "Warp.h"
#include "TitleObj.h"
#include "Shake.h"
#include "GameCamera.h"

#include "LoadObjectData.h"
#include "ParticleManager.h"
#include "BossEnemy.h"

/// <summary>
/// 基本的なゲーム用シーン
/// </summary>
class BossScene : public BaseScene
{
private:
	//キーボード
	Input* input_ = Input::GetInstance();

	//コントローラー
	Controller* controller_ = Controller::GetInstance();

	//sound
	MyXAudio* sound_ = MyXAudio::GetInstance();

	//screen
	Square screen_;

	//player
	std::unique_ptr<Player> player_;

	//Boss
	std::unique_ptr<BossEnemy> bossEnemy_;

	Goal* goal_ = new Goal();

	//描画用行列
	MyMath::MatView matView_;
	Matrix matProjection_ = MyMath::PerspectiveFovLH(
		Window::window_width_, Window::window_height_,
		MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);

	//camera
	std::unique_ptr<GameCamera> gameCamera_;

	//pipeline
	std::unique_ptr<GPipeline> pipeline_;

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline_;

	//shader
	Shader shader_;
	Shader bilShader_;

	//tex
	size_t blockTex_ = 0;
	size_t plyerTex_ = 0;
	size_t whiteTex_ = 0;
	size_t white_ = 0;
	size_t texP_ = 0;
	size_t blackTex_ = 0;

	//タイム
	size_t time_;

	//sprite
	SpriteCommon* normalSpriteCommon_ = new  SpriteCommon();
	Sprite* sprite_ = new Sprite();

	//Sprite
	Sprite* blackOut_ = new Sprite();

	//hitstop
	HitStop* hitStop_ = new HitStop();

	//BGM
	size_t hitSound_;

	//カラー
	Vector4D color_ = { 0,0,0,0 };

	//
	bool blackOutFlag_ = false;

	//
	bool movieF_ = false;

	//ムービー
	bool movieClearFlag_;
	bool movieOverFlag_;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 破棄
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// ステージが切り替わるときの暗転
	/// </summary>
	void BlackOut();
};
