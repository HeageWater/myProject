#pragma once
#include "RE//Square.h"
#include <vector>
class Number
{
private:
	MyDirectX* dx = nullptr;
	GPipeline* pipeline = nullptr;
	Shader* shader = nullptr;
public:
	std::vector<Square> square;
	int numG[10];
	int num;
public:
	void Initialize(MyDirectX* dx_, GPipeline* pipeline_, Shader* shader_, int numG[]);
	Number();
	Number(MyDirectX* dx, GPipeline* pipeline, Shader* shader, int numG[]);
	void SetSquareSize();
	void Update(Matrix matView, Matrix matProjection);
	void Draw();
	void ScoreNum(int num_) { num = num_; }
};

