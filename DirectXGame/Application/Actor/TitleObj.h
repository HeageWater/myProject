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

/// <summary>
/// タイトル
/// </summary>
class TitleObj
{
public:
	TitleObj();
	~TitleObj();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(Shader shader, GPipeline* pipeline_);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	void Draw(size_t tex);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void Update(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 四角と四角の判定(collisionManagerに移行後削除)
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	bool BoxCollision(Model model);

	/// <summary>
	/// 
	/// </summary>
	bool BoxCollision(Model* model);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// Movie中なら動かす
	/// </summary>
	void Movie();

	//今Movieモードか
	bool IsMovie_ = false;

	//Movieモードが終わったか
	bool EndMovie_ = false;

	//タイトルモデル
	Model titleObj_;

private:

	//今どれくらいたったか
	float time_ = 0;
};
