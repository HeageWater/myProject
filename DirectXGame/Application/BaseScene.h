#pragma once
#include "Controller.h"
#include "Model.h"
#include "Input.h"
#include "Object3D.h"
#include "TextureData.h"
#include "Square.h"
#include "Sprite.h"

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