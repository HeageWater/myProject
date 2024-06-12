#pragma once
//全てのシーンで必要なものをIncude
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

/// <summary>
/// 継承専用のシーンの基盤クラス
/// </summary>
class BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 破棄
	/// </summary>
	virtual void Finalize() = 0;

	virtual ~BaseScene() = default;

private:

	/// <summary>
	/// Imguiなどのデバッグ用
	/// </summary>
	virtual void Debug() = 0;
};