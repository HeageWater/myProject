#include "LoadObjectData.h"

void LoadObjectData::Initialize()
{
	////最初に呼び出すファイル
	//StageLoad("TitleStage");

	////基本画像
	//enemyTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
	//stageTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/blockNormal.png");
}

void LoadObjectData::Update()
{
	////エネミー更新
	//for (size_t i = 0; i < enemies_.size(); i++)
	//{
	//	enemies_[i]->Update(view_, prodaction_);

	//	if (enemies_[i]->GetIsDead())
	//	{
	//		enemies_.erase(enemies_.begin() + i);
	//	}
	//}

	////ステージ更新
	//for (size_t i = 0; i < stages_.size(); i++)
	//{
	//	stages_[i]->Update(view_, prodaction_);
	//}
}

void LoadObjectData::Draw()
{
	////エネミー描画
	//for (size_t i = 0; i < enemies_.size(); i++)
	//{
	//	enemies_[i]->Draw();
	//}

	////ステージ描画
	//for (size_t i = 0; i < stages_.size(); i++)
	//{
	//	stages_[i]->Draw(stageTex_);
	//}
}

void LoadObjectData::Finalize()
{
	////今あるステージを削除
	//for (size_t i = 0; i < stages_.size(); i++)
	//{
	//	stages_.erase(stages_.begin());
	//}

	////今あるエネミーを削除
	//for (size_t i = 0; i < enemies_.size(); i++)
	//{
	//	enemies_.erase(enemies_.begin());
	//}

	////保存したposを削除
	//for (size_t i = 0; i < cameraPos_.size(); i++)
	//{
	//	cameraPos_.erase(cameraPos_.begin());
	//}
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

	////今あるものの数
	//size_t countStage = stages_.size();
	//size_t countEnemy = enemies_.size();

	////今あるステージを削除
	//for (size_t i = 0; i < countStage; i++)
	//{
	//	stages_.erase(stages_.begin());
	//}

	////今あるエネミーを削除
	//for (size_t i = 0; i < countEnemy; i++)
	//{
	//	enemies_.erase(enemies_.begin());
	//}

	////調整用変数
	//const float enemyScale = 3.0f;
	//const float stageScale = 10.0f;
	//const float pos = 10.0f;

	//ホットリロードでStageSelectごとに読み込むようにする
	//レベルデータからオブジェクトに生成、配置
	//for (auto& objectdata : levelData_->objects_)
	//{
	//	//ファイル名から登録済みモデルを検索
	//	//Stage* model_ = nullptr;
	//	//decltype(newStage_)::iterator it = newStage_.find(objectdata.fileName_);

	//	////終わりか
	//	//if (it != newStage_.end())
	//	//{
	//	//	model_ = it->second;
	//	//}

	//	////Enemyだったら
	//	//if (objectdata.fileName_ == "Enemy")
	//	//{
	//	//	//モデルを指定して3Dオブジェクトを生成
	//	//	//Enemy* newModel_ = new Enemy();
	//	//	BulletEnemy* newModel_ = new BulletEnemy();
	//	//	newModel_->Initialize(shader_, pipeline_);

	//	//	//trans
	//	//	newModel_->SetTrans(objectdata.translation_ * pos);

	//	//	//rotation
	//	//	newModel_->SetRot(objectdata.rotation_);

	//	//	//scale;
	//	//	newModel_->SetScale(objectdata.scaling_ * enemyScale);

	//	//	//Update
	//	//	newModel_->Update(view_, prodaction_);

	//	//	//格納
	//	//	enemies_.push_back(newModel_);

	//	//	//次へ
	//	//	continue;
	//	//}

	//	////Stageだったら
	//	//if (objectdata.fileName_ == "Stage")
	//	//{
	//	//	//モデルを指定して3Dオブジェクトを生成
	//	//	Stage* newModel_ = new Stage();
	//	//	newModel_->Initialize(shader_, pipeline_);

	//	//	//trans
	//	//	newModel_->SetTrans(objectdata.translation_ * pos);

	//	//	//rotation
	//	//	newModel_->SetRot(objectdata.rotation_);

	//	//	//scale;
	//	//	newModel_->SetScale(objectdata.scaling_ * stageScale);

	//	//	//Update
	//	//	newModel_->Update(view_, prodaction_);

	//	//	//格納
	//	//	stages_.push_back(newModel_);

	//	//	//次へ
	//	//	continue;
	//	//}

	//	////Cameraだったら
	//	//if (objectdata.fileName_ == "Camera")
	//	//{
	//	//	Vector3D cameraPos = objectdata.translation_;
	//	//	cameraPos_.push_back(&cameraPos);
	//	//}

	//	////Warpだったら
	//	//if (objectdata.fileName_ == "Warp")
	//	//{
	//	//}

	//	////Startだったら
	//	//if (objectdata.fileName_ == "Start")
	//	//{
	//	//	StartPos_ = objectdata.translation_ * pos;
	//	//}

	//	////Endだったら
	//	//if (objectdata.fileName_ == "End")
	//	//{
	//	//	EndPos_ = objectdata.translation_ * pos;
	//	//}

	//	////Eventだったら
	//	//if (objectdata.fileName_ == "Event")
	//	//{
	//	//	//縦に長いイベントボックスとの判定をとって
	//	//	//カメラの位置を固定にして連続雑魚戦
	//	//	EventPos_ = objectdata.translation_ * pos;
	//	//	EventScale_ = objectdata.scaling_ * pos;
	//	//}

	//	////EventCameraだったら
	//	//if (objectdata.fileName_ == "EventCamera")
	//	//{
	//	//	//縦に長いイベントボックスとの判定をとって
	//	//	//カメラの位置を固定にして連続雑魚戦
	//	//	EventCameraPos_ = objectdata.translation_ * pos;
	//	//}

	//	////Back(背景)だったら
	//	//if (objectdata.fileName_ == "Back")
	//	//{
	//	//	//立方体を出す

	//	//}
	//}
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

//std::vector<Stage*> LoadObjectData::GetStage()
//{
//	return stages_;
//}
//
////std::vector<Enemy*> LoadObjectData::GetEnemy()
//std::vector<BaseEnemy*> LoadObjectData::GetEnemy()
//{
//	return enemies_;
//}