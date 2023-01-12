#include "Timer.h"

void Timer::Init()
{
	num.ScoreNum(0);
	num.SetSquareSize();
}

Timer::Timer(MyDirectX* dx_, GPipeline* pipeline_, Shader* shader_, int numG[])
{
	num.Initialize(dx_, pipeline_, shader_, numG);
}

void Timer::ClearUpdate(Matrix matView, Matrix matProjection)
{
	num.ScoreNum(timer / 60);
	num.SetSquareSize();
	for (size_t i = 0; i < num.square.size(); i++)
	{
		num.square[i].obj.scale = { 32,64,0.2f };
		num.square[i].obj.trans.x = 64;
		num.square[i].obj.trans.x -= (64 + 10) * i;
		num.square[i].obj.trans.y = 0;
	}
	num.Update(matView, matProjection);
}

void Timer::Update(Matrix matView, Matrix matProjection)
{
	timer++;
	num.ScoreNum(timer / 60);
	num.SetSquareSize();
	for (size_t i = 0; i < num.square.size(); i++)
	{
		num.square[i].obj.scale = { 16,32,0.2f };
		num.square[i].obj.trans.x = 604;
		num.square[i].obj.trans.x -= (32 + 5) * i;
		num.square[i].obj.trans.y += 324;
	}
	num.Update(matView, matProjection);
}

void Timer::Draw()
{
	num.Draw();
}

void Timer::Reset() {
	timer = 0;
}
