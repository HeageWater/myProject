#include "Number.h"

void Number::Initialize(MyDirectX* dx_, GPipeline* pipeline_, Shader* shader_, int numG[])
{
	num = 0;
	
	dx = dx_;
	pipeline = pipeline_;
	shader = shader_;
	for (size_t i = 0; i < 10; i++)
	{
		this->numG[i] = numG[i];
	}
}

Number::Number()
{
}

Number::Number(MyDirectX* dx, GPipeline* pipeline, Shader* shader, int numG[])
{
	Initialize(dx, pipeline, shader, numG);
}

void Number::SetSquareSize()
{
	int digits = 0;
	int number = num;
	if (number == 0) digits = 1;
	while (number > 0) {
		number /= 10.0f;
		digits++;
	}
	square.resize(digits);

	for (size_t i = 0; i < square.size(); i++)
	{
		square[i].Initialize(dx, pipeline, *shader);
	}
}

void Number::Update(Matrix matView, Matrix matProjection)
{
	for (size_t i = 0; i < square.size(); i++) {
		square[i].MatUpdate(matView, matProjection,0);
	}
}

void Number::Draw()
{
	int number = num;
	for (size_t i = 0; i < square.size(); i++) {
		int index = number % 10;
		int handle = numG[index];
		square[i].Draw(handle);
		number /= 10;
	}
}
