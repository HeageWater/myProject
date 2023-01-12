#pragma once
#include "Re//GPipeline.h"
#include "Re//Object3D.h"
#include "Re//ConstBuff.h"
#include "Re//TextureData.h"
#include "Re//Shader.h"
#include "Re//MyDebugCamera.h"
#include "Re//Square.h"
#include "Re//Controller.h"
#include <memory>
#include "Re//Model.h"
#include "Re//Controller.h"

class Particle {
public:
	Particle();
	~Particle();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(int tex, Matrix matView, Matrix matProjection);
	void Update(Matrix matView, Matrix matProjection);

	Model particle;
	Vector3D trans;
	Vector3D rot;
	bool hitF = 0;
	float spd = 0.2f;


private:
	Model model;
	Vector2D pos;
	Vector2D angle;
	float radias = 1;
	float countDown;
	int count;
	bool isDied = false;
public:
	/*Particle(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, Vector2D pos);
	~Particle();

	void SetModelPos();
	void Draw(int handle);
	void Update(Matrix matView, Matrix matProjection);

	bool GetIsDied() { return isDied; };*/
};