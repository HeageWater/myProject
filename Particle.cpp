#include "Particle.h"
#include "enemy.h"

Particle::Particle()
{

}

Particle::~Particle()
{

}

void Particle::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	particle.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
}

void Particle::Draw(int tex, Matrix matView, Matrix matProjection)
{
	particle.Draw(tex);
}

void Particle::Update(Matrix matView, Matrix matProjection)
{
	particle.MatUpdate(matView, matProjection);
}