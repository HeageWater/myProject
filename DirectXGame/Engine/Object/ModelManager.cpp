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
	//重複排除
	if (models_.contains(filename))
	{
		//return;
	}

	//モデル作成
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//モデル初期化
	model->Initialize(MyDirectX::GetInstance(), shader_, filename, pipeline_.get());

	//モデル格納
	models_.insert(std::make_pair(filename, std::move(model)));
}

void ModelManager::SetModel(const std::string& filename, Model* model)
{
	//>>
	model = models_.at(filename).get();

	//models_.at(filename).get()->mat_ >> model->mat_ mat_;
}

std::unique_ptr<Model> ModelManager::FindModel(const std::string& filename)
{
	////保存したモデルを探して返す
	//if (models_.contains(filename))
	//{
	//	std::unique_ptr<Model> data;
	//	data = std::make_unique<Model>();

	//	//SetModel(filename, data.get());

	//	//
	//	//data = std::move(models_.at(filename));

	//	//models_.erase(filename);

	//	return std::move(data);
	//}

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(models_.begin(), models_.end(), [&](std::pair<const std::string, std::unique_ptr<Model, std::default_delete<Model>>>& p)
		{
			return p.first == filename;
		});

	//保存したモデルを探して返す
	if (models_.contains(filename))
	{
		std::unique_ptr<Model> data;
		data = std::make_unique<Model>();

		//
		data = std::move(models_.at(filename));

		models_.erase(filename);

		return data;
	}

	//ない時
	return nullptr;
}

void ModelManager::Reset()
{
	models_.clear();
}

ModelManager* ModelManager::GetInstance()
{
	static ModelManager modelManager;
	return &modelManager;
}
