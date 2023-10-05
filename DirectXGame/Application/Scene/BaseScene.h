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
	//������
	virtual void Initilize() = 0;

	//�X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;

	//�j��
	virtual void Finalize() = 0;

	virtual ~BaseScene() = default;
private:
};