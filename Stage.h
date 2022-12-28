#pragma once
#include "Re//GPipeline.h"
#include "Re//Object3D.h"
#include "Re//ConstBuff.h"
#include "Re//TextureData.h"
#include "Re//Shader.h"
#include "Re//MyDebugCamera.h"
#include "Re//Square.h"
#include "Re//Controller.h"
#include <memory>
#include "Re//Model.h"

class Stage {
public:
	Stage();
	~Stage();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(int tex);
	void Update(Matrix matView, Matrix matProjection);

	Vector2D stage[20] =
	{ {0,1},
	{0,2},
	{0,3},
	{0,4},
	{0,5},
	{0,6},
	{0,7},
	{0,8},
	{0,9},
	{1,10}, 
	{2,10}, 
	{3,10}, 
	{4,10}, 
	{5,10}, 
	{6,10}, 
	{7,10}, 
	{8,10}, 
	{9,10}, 
	{10,10}, 
	{1,1}, };

	std::vector<Model*> model;
	std::vector<Model*> getmodel;
	Model box3;

	const float stageSize = 2;
};
