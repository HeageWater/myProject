#include "ModelManager.h"
#include "DirectX.h"

void ModelManager::Initialize()
{
	//shader
	shader_.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");

	//pipeline
	pipeline_ = std::make_unique<GPipeline>();
	pipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), shader_);
}

void ModelManager::Finalize()
{
}

void ModelManager::LoadModel(const std::string& filename)
{
	std::string a = filename;
	a = "E";

	////重複排除
	//if (models.contains(filename))
	//{
	//	return;
	//}

	////モデル作成
	//std::unique_ptr<Model> model = std::make_unique<Model>();

	////モデル初期化
	//model->Initialize(MyDirectX::GetInstance(), shader_, filename, pipeline_.get());

	////モデル格納
	//models.insert(std::make_pair(filename, std::move(model)));
}

//Model* ModelManager::FindModel(const std::string& filename)
//{
//	//保存したモデルを探して返す
//	if (models.contains(filename))
//	{
//		return models.at(filename).get();
//	}
//
//	//ない時
//	return nullptr;
//}

ModelManager* ModelManager::GetInstance()
{
	static ModelManager modelManager;
	return &modelManager;
}
