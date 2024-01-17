#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include <memory>
#include "Model.h"

class Bullet
{
public:
	/// <summary>
	/// 
	/// </summary>
	Bullet();

	/// <summary>
	/// 
	/// </summary>
	void Initialize(Shader shader, GPipeline* pipeline);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	void Draw();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="view"></param>
	/// <param name="prodaction"></param>
	void SetCamera(Matrix view, Matrix prodaction);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3D pos) { bullet_.mat_.trans_ = pos; };

	/// <summary>
	/// 
	/// </summary>
	/// <param name="vec"></param>
	void SetVec(Vector3D vec) { Vec = vec; };
private:

	//
	Model bullet_;

	Vector3D Vec = { 0,0,0 };

	size_t tex_ = 0;

	Shader shader_;
	GPipeline* pipeline_;
	Matrix view_;
	Matrix prodaction_;

};