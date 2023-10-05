#include "ParticleManager.h"
#include "DirectX.h"

void ParticleManager::Initalize()
{
}

void ParticleManager::Update()
{
}

void ParticleManager::Draw()
{
}

void ParticleManager::Finalize()
{
}

void ParticleManager::CreateBoxParticle(Vector3D pos)
{
	size_t minRange = 10;
	size_t maxRange = 30;

	size_t play = MyMath::GetRandom(minRange, maxRange);

	for (size_t i = 0; i < play; i++)
	{
		BoxParticle* newP = new BoxParticle();

		//newP->Initialize(MyDirectX::GetInstance(), shader, pipeline.get());

		newP->SetPos(pos);

		boxs.push_back(newP);
	}
}
