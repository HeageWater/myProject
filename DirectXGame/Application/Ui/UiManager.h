#pragma once
#include "Sprite.h"
#include "Input.h"
#include "Object3D.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include "Model.h"
#include "Sound.h"
#include "Easing.h"
#include "ImguiManager.h"
#include "Shader.h"

class UiManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 破棄
	/// </summary>
	void Finalize();

	/// <summary>
	/// ライフの残りを取得
	/// </summary>
	void SetLife(float life) { life_ = life; };

	//UIを描画する
	void UiDraw();

	//ライフを描画する
	void LifeDraw();

	//シングルトン
	static UiManager* GetInstance()
	{
		static UiManager uiManager;
		return &uiManager;
	}

	//
	UiManager() = default;
	~UiManager() = default;

	//
	UiManager& operator=(const UiManager&) = delete;
	UiManager(const UiManager&) = delete;
private:

	//描画用行列
	MyMath::MatView matView_;
	Matrix matProjection_ = MyMath::PerspectiveFovLH(
		Window::window_width_, Window::window_height_,
		MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);

	//sprite
	SpriteCommon* normalSpriteCommon_ = new  SpriteCommon();
	Sprite* sprite_ = new Sprite();

	Sprite* lifePng_ = new Sprite();
	Sprite* lesPng_ = new Sprite();
	Sprite* lesPng2_ = new Sprite();
	Sprite* lesPng3_ = new Sprite();
	Sprite* havePng_ = new Sprite();
	Sprite* havePng2_ = new Sprite();
	Sprite* havePng3_ = new Sprite();
	Sprite* UILStick_ = new Sprite();
	Sprite* UIRStick_ = new Sprite();
	Sprite* UILT_ = new Sprite();
	Sprite* UIAButton_ = new Sprite();
	Sprite* UIPress_ = new Sprite();

	//tex
	size_t white_ = 0;
	size_t texP_ = 0;
	size_t brPng_ = 0;
	size_t enemyPng_ = 0;
	size_t clearTex_ = 0;
	size_t overTex_ = 0;
	size_t playerTex_ = 0;
	size_t titleTex_ = 0;
	size_t heartLesTex_ = 0;
	size_t heartHaveTex_ = 0;
	size_t lifeTex_ = 0;
	size_t backTex_ = 0;
	size_t LTex_ = 0;
	size_t RTex_ = 0;
	size_t AbuttonTex_ = 0;
	size_t PressTex_ = 0;
	size_t LTTex_ = 0;

	//残りライフ格納用変数
	float life_ = 0;

	//カラー
	Vector4D color_ = { 0,0,0,0 };
};