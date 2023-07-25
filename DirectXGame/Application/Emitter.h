#pragma once
#include "Particle.h"
//field �̒��Ƀp�[�e�B�N������������̏������G�~�b�^�[�ɏ���
//�p�[�e�B�N���ɂȂ�ׂ�if���Ȃǂ������Ȃ�

class Emitter
{
public:
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Update(Matrix matView, Matrix matProjection);
	void Draw(size_t tex);
	void Create();

	void SetPos(Vector3D pos) { sample_.mat.trans = pos; };

private:
	std::vector<Particle*> particles_;
	Model sample_;

	//MyDirectX* dx = nullptr;
	//GPipeline* pipeline = nullptr;
};