#include "Stage.h"

Stage::Stage()
{
	model.clear();
}

Stage::~Stage()
{
	for (int i = 0; i < model.size(); i++)
	{
		delete model[i];
	}
}

void Stage::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	int Left = 58;
	int Up = 48;

	//�X�e�[�W�̏�Q���̐�
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (map[i][j] != Blank) {
				//�����̒��g�����
				Model* newmodel = new Model();
				
				float size = 10;

				newmodel->Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);

				newmodel->mat.trans = { -(size * j) + Left,-(size * i) + Up, 0 };

				newmodel->mat.scale = { 5,5,5};

				//�i�[
				model.push_back(newmodel);
			}
		}
	}

	box3.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
}

void Stage::Draw(int tex)
{
	//box3.Draw(tex);

	for (int i = 0; i < model.size(); i++)
	{
		model[i]->Draw(tex);
	}
}

void Stage::Update(Matrix matView, Matrix matProjection)
{
	box3.MatUpdate(matView,matProjection);

	for (int i = 0; i < model.size(); i++)
	{
		model[i]->MatUpdate(matView, matProjection);;
	}
}
