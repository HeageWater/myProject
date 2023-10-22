#pragma once
#include "Particle.h"
#include "BoxParticle.h"
//field の中にパーティクルが入ったらの処理をエミッターに書く
//パーティクルになるべくif文などを書かない

/// <summary>
/// パーティクルの出現場所などの細かい調整用クラス
/// </summary>
class Emitter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dx_"></param>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void Update(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	void Draw(size_t tex);

	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	/// <summary>
	/// pos格納
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3D pos) { sample_.mat_.trans_ = pos; };

private:

	//格納用変数
	std::vector<Particle*> particles_;

	//モデル
	Model sample_;
};