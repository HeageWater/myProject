#pragma once
#include "Particle.h"
#include "BoxParticle.h"

class ParticleManager
{
public:
	void Initalize();
	void Update();
	void Draw();
	void Finalize();

	//�l�p�̃p�[�e�B�N�������
	void CreateBoxParticle(Vector3D pos);

	//
	static ParticleManager* GetInstance()
	{
		static ParticleManager particleManager_;
		return &particleManager_;
	}
private:
	std::vector<BoxParticle*> boxs;

	//
	ParticleManager() = default;
	~ParticleManager() = default;

	//
	ParticleManager& operator=(const ParticleManager&) = delete;
	ParticleManager(const ParticleManager&) = delete;
};