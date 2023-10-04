#include "Stage.h"

Stage::Stage()
{
	stage_.mat.Initialize();
	stage_.mat.scale = { 3,3,3 };
}

Stage::~Stage()
{
}

void Stage::Initialize(Shader shader, GPipeline* pipeline_)
{
	stage_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\box.obj", pipeline_);

	stage_.mat.Initialize();
	stage_.mat.scale = { 1020,5,120 };
	stage_.mat.trans.x = 0;
	stage_.mat.trans.y = 0;
	stage_.mat.trans.z = 0;

	drawFlag = true;
}

void Stage::Draw(size_t tex)
{
	if (drawFlag)
	{
		stage_.Draw(tex);
	}
}

void Stage::Update(Matrix matView, Matrix matProjection)
{
	stage_.MatUpdate(matView, matProjection);
}

void Stage::Reset()
{
	stage_.mat.trans.x = 0;
	stage_.mat.trans.y = 0;
}