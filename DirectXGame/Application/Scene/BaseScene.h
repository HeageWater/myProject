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

class BaseScene
{
public:
	//初期化
	virtual void Initilize() = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	//破棄
	virtual void Finalize() = 0;

	virtual ~BaseScene() = default;
private:
};