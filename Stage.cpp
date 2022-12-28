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
	for (int i = 0; i < 20; i++)
	{
		//‰¼‰«‚Ì’†g‚ðì‚é
		Model* newmodel = new Model();

		newmodel->Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);

		newmodel->mat.scale = { 1,1,1 };

		newmodel->mat.trans.x = stage[i].y * stageSize - 10;
		newmodel->mat.trans.y = stage[i].x * stageSize;
		newmodel->mat.trans.z = 0;

		//Ši”[
		model.push_back(newmodel);
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
