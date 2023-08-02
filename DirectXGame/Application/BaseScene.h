#pragma once
//�S�ẴV�[���ŕK�v�Ȃ��̂�Incude
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
	virtual void Update();
	virtual void Initilize();
	virtual void Draw();
	virtual void Finalize();
	//virtual ~BaseScene() = delete;

private:
};