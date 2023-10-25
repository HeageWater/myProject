#include "ParticleManager.h"
#include "DirectX.h"

void ParticleManager::Initalize()
{
}

void ParticleManager::Update()
{
	//ボックスパーティクル
	for (size_t i = 0; i < boxParticles_.size(); i++)
	{
		boxParticles_[i]->Update(matview_, matProdaction_);
	}

	//サークルパーティクル
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i]->Update(matview_, matProdaction_);
	}
}

void ParticleManager::Draw()
{
	//ボックスパーティクル
	for (size_t i = 0; i < boxParticles_.size(); i++)
	{
		boxParticles_[i]->Draw(0);
	}

	//サークルパーティクル
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i]->Draw(0);
	}
}

void ParticleManager::Finalize()
{
	//ボックスパーティクル
	for (size_t i = 0; i < boxParticles_.size(); i++)
	{
		boxParticles_.erase(boxParticles_.begin());
	}

	//サークルパーティクル
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_.erase(particles_.begin());
	}
}

void ParticleManager::CreateBoxParticle(Vector3D pos)
{
	//飛び散る範囲の最小値,最大値
	size_t minRange = 10;
	size_t maxRange = 30;

	//どれくらい飛び散るかの値
	size_t play = MyMath::GetRandom(minRange, maxRange);

	for (size_t i = 0; i < play; i++)
	{
		//仮クラス制作
		BoxParticle* newP = new BoxParticle();

		//初期化
		newP->Initialize(shader_, pipeline_);

		//場所をセット
		newP->SetPos(pos);

		//格納
		boxParticles_.push_back(newP);
	}
}

void ParticleManager::CreateCircleParticle(Vector3D pos)
{
	//飛び散る範囲の最小値,最大値
	size_t minRange = 10;
	size_t maxRange = 30;

	//どれくらい飛び散るかの値
	size_t play = MyMath::GetRandom(minRange, maxRange);

	for (size_t i = 0; i < play; i++)
	{
		//仮クラス制作
		Particle* newP = new Particle();

		//初期化
		newP->Initialize(shader_, pipeline_);

		//場所をセット
		newP->SetPos(pos);

		//格納
		particles_.push_back(newP);
	}
}

void ParticleManager::SetCamera(Matrix matview, Matrix matProdaction)
{
	matview_ = matview;
	matProdaction_ = matProdaction;
}

void ParticleManager::SetDraw(Shader shader, GPipeline* pipeline)
{
	shader_ = shader;
	pipeline_ = pipeline;
}
