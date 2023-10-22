#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include <memory>
#include "Model.h"

/// <summary>
/// ステージ
/// </summary>
class Stage
{
public:
	Stage();
	~Stage();

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 描画フラグ
	/// </summary>
	/// <param name="flag"></param>
	void SetFlag(bool flag) { drawFlag_ = flag; };

	//モデル
	Model stage_;
private:

	//描画フラグ
	bool drawFlag_ = true;
};