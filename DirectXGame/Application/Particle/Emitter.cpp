#include "Emitter.h"

void Emitter::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	//dx = dx_;
	//pipeline = pipeline_;

	sample_.Initialize(dx_, shader, "Resources\\kyu\\kyu.obj", pipeline_);

	sample_.mat_.Initialize();
}

void Emitter::Update(Matrix matView, Matrix matProjection)
{
	//パーティクル
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i]->Update(matView, matProjection);

		//削除
		if (particles_[i]->IsDead() == true)
		{
			particles_.erase(particles_.begin() + i);
		}
	}
}

void Emitter::Draw(size_t tex)
{
	//パーティクル
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i]->Draw(tex);
	}
}

void Emitter::Create()
{
	size_t play = MyMath::GetRandom(10, 30);

	for (size_t i = 0; i < play; i++)
	{
		Particle* newP = new Particle();

		//newP->Initialize(dx.get(), shader, pipeline.get(), player->GetPos());
		//newP->Initialize(&sample_);

		particles_.push_back(newP);
	}
}
