#include "Actor.h"

void Actor::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	actor.Initialize(dx_, shader, "Resources\\kyu\\kyu.obj", pipeline_);

	actor.mat.Initialize();
	actor.mat.scale = { 1,1,1 };
	actor.mat.trans.x = 0;
	actor.mat.trans.y = 0;
	actor.mat.trans.z = 0;
}

void Actor::Draw(int tex)
{
	actor.Draw(tex);
}

void Actor::Update(Matrix matView, Matrix matProjection)
{
	actor.MatUpdate(matView, matProjection);
}