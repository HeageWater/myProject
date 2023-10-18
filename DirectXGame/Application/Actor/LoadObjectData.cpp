#include "LoadObjectData.h"

void LoadObjectData::Initialize()
{
	//最初に呼び出すファイル
	//levelData_ = JsonFileOpen::FileOpen("untitled");
	StageLoad("untitled");

	//基本画像
	enemyTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
	stageTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/blockNormal.png");
}

void LoadObjectData::Update()
{
	//エネミー更新
	for (size_t i = 0; i < enemies_.size(); i++)
	{
		enemies_[i]->Update(view_, prodaction_);
	}

	//ステージ更新
	for (size_t i = 0; i < stages_.size(); i++)
	{
		stages_[i]->Update(view_, prodaction_);
	}
}

void LoadObjectData::Draw()
{
	//エネミー描画
	for (size_t i = 0; i < enemies_.size(); i++)
	{
		enemies_[i]->Draw(enemyTex_);
	}

	//ステージ描画
	for (size_t i = 0; i < stages_.size(); i++)
	{
		stages_[i]->Draw(stageTex_);
	}
}

void LoadObjectData::Finalize()
{
	//今あるステージを削除
	for (size_t i = 0; i < stages_.size(); i++)
	{
		stages_.erase(stages_.begin());
	}

	//今あるエネミーを削除
	for (size_t i = 0; i < enemies_.size(); i++)
	{
		enemies_.erase(enemies_.begin());
	}
}

LoadObjectData* LoadObjectData::GetInstance()
{
	static LoadObjectData loadObjectData;
	return &loadObjectData;
}

//ファイル名を入れるとそれを読み込む
void LoadObjectData::StageLoad(const std::string& filePath)
{
	//stageファイル
	levelData_ = JsonFileOpen::FileOpen(filePath);

	//今あるものの数
	size_t countEnemy = stages_.size();
	size_t countStage = enemies_.size();

	//今あるステージを削除
	for (size_t i = 0; i < countStage; i++)
	{
		stages_.erase(stages_.begin());
	}

	//今あるエネミーを削除
	for (size_t i = 0; i < countEnemy; i++)
	{
		enemies_.erase(enemies_.begin());
	}

	//ホットリロードでStageSelectごとに読み込むようにする
	//レベルデータからオブジェクトに生成、配置
	for (auto& objectdata : levelData_->objects)
	{
		//ファイル名から登録済みモデルを検索
		Stage* model_ = nullptr;
		decltype(newStage_)::iterator it = newStage_.find(objectdata.fileName);

		//終わりか
		if (it != newStage_.end())
		{
			model_ = it->second;
		}

		if (objectdata.fileName == "Enemy")
		{
			//モデルを指定して3Dオブジェクトを生成
			Enemy* newModel_ = new Enemy();
			newModel_->Initialize(shader_, pipeline_);

			//調整
			float scale = 10.0f;

			//trans
			newModel_->enemy_.mat.trans = objectdata.translation * scale;

			//rotation
			newModel_->enemy_.mat.rotAngle = objectdata.rotation;

			//scale;
			newModel_->enemy_.mat.scale = objectdata.scaling * scale;

			//Update
			newModel_->Update(view_, prodaction_);

			//格納
			enemies_.push_back(newModel_);

			//次へ
			continue;
		}

		//if (objectdata.fileName == "Stage")
		//{
			//モデルを指定して3Dオブジェクトを生成
		Stage* newModel_ = new Stage();
		newModel_->Initialize(shader_, pipeline_);

		//調整
		float scale = 10.0f;

		//trans
		newModel_->stage_.mat.trans = objectdata.translation * scale;

		//rotation
		newModel_->stage_.mat.rotAngle = objectdata.rotation;

		//scale;
		newModel_->stage_.mat.scale = objectdata.scaling * scale;

		//Update
		newModel_->Update(view_, prodaction_);

		//格納
		stages_.push_back(newModel_);

		//次へ
		//continue;
		//}
	}
}

void LoadObjectData::SetCamera(Matrix view, Matrix prodaction)
{
	view_ = view;
	prodaction_ = prodaction;
}

void LoadObjectData::SetModel(Shader shader, GPipeline* pipeline)
{
	shader_ = shader;
	pipeline_ = pipeline;
}