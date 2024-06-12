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
	Square screen_;

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
	size_t whiteTex_ = 0;

	//sprite
	SpriteCommon* normalSpriteCommon_ = new  SpriteCommon();
	Sprite* sprite_ = new Sprite();

	//Sprite
	Sprite* blackOut_ = new Sprite();

	//カラー
	Vector4D color_ = { 0,0,0,0 };
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

private:

	/// <summary>
	/// Imguiなどのデバッグ用
	/// </summary>
	void Debug()override;

};