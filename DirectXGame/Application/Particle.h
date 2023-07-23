#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include <memory>
#include "Model.h"

class Particle
{
public:
	Particle();
	~Particle();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_,Vector3D pos);
	void Draw(size_t tex);
	void Update(Matrix matView, Matrix matProjection);
	bool IsDead() { return isDead; };
private:
	Model particle_;

	bool isDead = false;
	float spd;
	float time = 0;
	Vector3D velocity = { 0,0,0 };
};