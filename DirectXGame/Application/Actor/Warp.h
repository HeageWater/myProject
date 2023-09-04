#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include <memory>
#include "Model.h"

class Warp
{
public:
	Warp();
	~Warp();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex);
	void Update(Matrix matView, Matrix matProjection);
	void Reset();
	bool BoxCollision(Model model);

	void SetFlag(bool flag) { drawFlag = flag; };
	Vector3D GetInPos() { return warp_[0].mat.trans; };
	Vector3D GetOutPos() { return warp_[1].mat.trans; };

	void CheckMode(uint32_t mode);

	Model warp_[2];
private:
	float size = 2;
	bool drawFlag = true;
};