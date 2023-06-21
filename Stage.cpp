#include "Stage.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	stage_.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
}	
void Stage::Draw(size_t tex)
{
	stage_.Draw(tex);
}

void Stage::Update(Matrix matView, Matrix matProjection)
{
	stage_.MatUpdate(matView, matProjection);
}

void Stage::Reset()
{
}