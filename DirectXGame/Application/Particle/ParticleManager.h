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

	//四角のパーティクルを作る
	void CreateBoxParticle(Vector3D pos);

	//シングルトン
	static ParticleManager* GetInstance()
	{
		static ParticleManager particleManager_;
		return &particleManager_;
	}
private:

	//箱型パーティクル格納用変数
	std::vector<BoxParticle*> boxs_;

	//
	ParticleManager() = default;
	~ParticleManager() = default;

	//
	ParticleManager& operator=(const ParticleManager&) = delete;
	ParticleManager(const ParticleManager&) = delete;
};