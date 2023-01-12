#pragma once
#include "Number.h"
class Timer
{
private:
	int timer;
	Number num;
public:
	void Init();
	Timer(MyDirectX* dx_, GPipeline* pipeline_, Shader* shader_, int numG[]);
	void ClearUpdate(Matrix matView, Matrix matProjection);
	void Update(Matrix matView, Matrix matProjection);
	void Draw();
	void Reset();
};

