//#include "Particle.h"
//#include <random>
//
//Particle::Particle()
//{
//
//}
//
//Particle::~Particle()
//{
//
//}
//
//void Particle::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
//{
//	particle.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
//}
//
//void Particle::Draw(int tex, Matrix matView, Matrix matProjection)
//{
//	particle.Draw(tex);
//}
//
//void Particle::Update(Matrix matView, Matrix matProjection)
//{
//	particle.MatUpdate(matView, matProjection);
//}
//
//
//
//float GetRandomPos(float min, float max)
//{
//	std::random_device rd;
//	std::default_random_engine eng(rd());
//	std::uniform_real_distribution<float> distr(min, max);
//	return distr(eng);
//}
//
//void Particle::SetModelPos()
//{
//	float angle_ = 0.0f;
//	Vector2D c_center(radias * cos(angle_), radias * sin(angle_));
//	model.mat.rotAngle.z = 0.0f;
//	model.mat.trans = { pos.x - c_center.x ,pos.y - c_center.y ,0.0f };
//
//	//エフェクトが3Dになる
//	model.mat.scale = { radias,radias ,radias };
//
//	//エフェクトが2Dになる
//	//model.mat.scale = { radias,radias ,1.0f };
//}
//
//void Particle::Draw(int handle) {
//
//	model.Draw(handle);
//}
//
//void Particle::Update(Matrix matView, Matrix matProjection) {
//
//	if (count < 2) {
//		pos += angle;
//		radias += countDown;
//		countDown += 0.1;
//	}
//
//	if (radias > 10.0f && count < 10) {
//		angle.normalize();
//		count++;
//		pos += angle;
//	}
//
//	if (count > 9) {
//		radias -= 1.0f;
//		pos.x -= 1.0f;
//	}
//
//	if (radias < 0)
//		isDied = true;
//
//	SetModelPos();
//	model.MatUpdate(matView, matProjection);
//}
//
//Particle::Particle(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, Vector2D pos) {
//	int speed = 5;
//	this->pos = pos;
//	angle.x = GetRandomPos(-speed, speed);
//	angle.y = GetRandomPos(-speed, speed);
//	countDown = GetRandomPos(0.1f, 0.3f);
//	count = 0;
//
//	model.Initialize(dx_, shader, "Resource\\Model\\box.obj", pipeline_);
//	model.mat.scale = { radias,radias,2.0f };
//
//	//Initialize();
//}
//
//Particle::~Particle() {
//
//}