#pragma once
#include "Particle.h"
#include "BoxParticle.h"

/// <summary>
/// パーティクルマネージャ
/// </summary>
class ParticleManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 破棄
	/// </summary>
	void Finalize();

	/// <summary>
	/// 四角のパーティクルを作る
	/// </summary>
	/// <param name="pos"></param>
	void CreateBoxParticle(Vector3D pos);

	/// <summary>
	/// 円のパーティクルを作る
	/// </summary>
	void CreateCircleParticle(Vector3D pos);

	/// <summary>
	/// 壁に当たってるときに出す
	/// </summary>
	/// <param name="pos"></param>
	void CreateWallParticle(Vector3D pos);

	/// <summary>
	/// ViewやProdactionを送る
	/// </summary>
	void SetCamera(Matrix matview, Matrix matProdaction);

	/// <summary>
	/// shaderやpipelineを送る
	/// </summary>
	void SetDraw(Shader shader, GPipeline* pipeline);

	//シングルトン
	static ParticleManager* GetInstance()
	{
		static ParticleManager particleManager_;
		return &particleManager_;
	}
private:

	//箱型パーティクル格納用変数
	std::vector<BoxParticle*> boxParticles_;
	//std::forward_list<BoxParticle*> boxParticles_;
	std::vector<Particle*> particles_;
	//std::forward_list<Particle> particles_;

	//camera用変数
	Matrix matview_;
	Matrix matProdaction_;

	//描画用変数
	Shader shader_;
	GPipeline* pipeline_;

	//初期画像
	size_t whiteTex_ = 0;

	//
	ParticleManager() = default;
	~ParticleManager() = default;

	//
	ParticleManager& operator=(const ParticleManager&) = delete;
	ParticleManager(const ParticleManager&) = delete;
};