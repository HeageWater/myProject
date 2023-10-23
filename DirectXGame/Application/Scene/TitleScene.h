#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Controller.h"
#include "Sound.h"
#include "Player.h"
#include "TitleObj.h"

#include "LoadObjectData.h"

/// <summary>
/// ゲームのタイトルシーン
/// </summary>
class TitleScene :public BaseScene
{
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
	Player* player_ = new Player();

	//描画用行列
	MyMath::MatView matView_;
	Matrix matProjection_ = MyMath::PerspectiveFovLH(
		Window::window_width_, Window::window_height_,
		MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);

	//sprite用
	Matrix spriteProjection_ = MyMath::OrthoLH(Window::window_width_, Window::window_height_, 0.0f, 1.0f);

	//pipeline
	std::unique_ptr<GPipeline> pipeline_;

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline_;

	//tex
	size_t block_ = 0;
	size_t white_ = 0;
	size_t attackTex_ = 0;
	size_t titleTex_ = 0;

	//shader
	Shader shader_;
	Shader bilShader_;

	//3Dタイトル
	TitleObj* titleObject = new TitleObj();

	//common
	SpriteCommon* spriteCommon_ = new  SpriteCommon();
	//Attack
	Sprite* attackPng_ = new Sprite();
};