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

/// <summary>
/// 基本的なゲーム用シーン
/// </summary>
class PlayScene : public BaseScene
{
private:
	//キーボード
	Input* input_ = Input::GetInstance();

	//コントローラー
	Controller* controller_ = Controller::GetInstance();

	//sound
	MyXAudio* sound_ = MyXAudio::GetInstance();

	//screen
	Square screen;

	//player
	Player* player = new Player();

	//描画用行列
	MyMath::MatView matView;
	Matrix matProjection = MyMath::PerspectiveFovLH(
		Window::window_width, Window::window_height,
		MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);

	//pipeline
	std::unique_ptr<GPipeline> pipeline;

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline;

	//tex
	size_t white = 0;

	//shader
	Shader shader;
	Shader bilShader;

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
};