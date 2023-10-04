#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "Model.h"
#include "Square.h"

class BoxParticle
{
public:
	BoxParticle();
	~BoxParticle();
	void Initialize(Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex);
	void Update(Matrix matView, Matrix matProjection);
	bool IsDead() { return isDead; };
	void SetPos(Vector3D pos) { box.mat.trans = pos; };
	void SetVec(Vector3D vec) { velocity = vec; };
	Vector3D GetPos() { return box.mat.trans; };
	Vector3D GetVec() { return velocity; };
private:

	//Square box;
	Object3D box;

	bool isDead = false;
	float spd;
	float time = 0;
	Vector3D velocity = { 0,0,0 };
};