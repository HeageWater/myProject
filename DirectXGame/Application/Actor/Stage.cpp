#include "Stage.h"

Stage::Stage()
{
	stage_.mat_.Initialize();
	stage_.mat_.scale_ = { 3,3,3 };
}

Stage::~Stage()
{
}

void Stage::Initialize(Shader shader, GPipeline* pipeline_)
{
	stage_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\box.obj", pipeline_);

	stage_.mat_.Initialize();
	stage_.mat_.scale_ = { 1020,5,120 };
	stage_.mat_.trans_.x_ = 0;
	stage_.mat_.trans_.y_ = 0;
	stage_.mat_.trans_.z_ = 0;

	drawFlag_ = true;
}

void Stage::Draw(size_t tex)
{
	if (drawFlag_)
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
	stage_.mat_.trans_.x_ = 0;
	stage_.mat_.trans_.y_ = 0;
}