#include "JsonFileOpen.h"
#include <fstream>
#include <cassert>

const std::string JsonFileOpen::kDefaultBaseDirectory_ = "Resources/levels/";
const std::string JsonFileOpen::kExtension_ = ".json";

LevelData* JsonFileOpen::FileOpen(const std::string& fileName)
{
	//パスを連結する
	const std::string fullpath = kDefaultBaseDirectory_ + fileName + kExtension_;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);

	//ファイルチェック
	if (file.fail())
	{
		//失敗してるかどうか
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialised;

	// 解凍
	file >> deserialised;

	//正しいかチェック
	assert(deserialised.is_object());
	assert(deserialised.contains("name"));
	assert(deserialised["name"].is_string());

	//nameを文字列として取得
	std::string name = deserialised["name"].get < std::string>();

	//正しいかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	//objectsの全オブジェクトを走査
	for (nlohmann::json& object : deserialised["object"])
	{
		//チェック
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種別の処理

		//Mesh
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects_.emplace_back(LevelData::ObjectData{});

			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects_.back();

			//ファイルネームだったら
			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName_ = object["file_name"];
			}

			//他に要素があったら
			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.otherName_ = object["file_name"];
			}

			//Transformのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//transform格納
			SetMatrix(transform, objectData);
		}
	}

	return levelData;
}

//transform格納
void JsonFileOpen::SetMatrix(nlohmann::json& transform, LevelData::ObjectData& objectData)
{
	//格納用
	Vector3D vec;

	float size = 1.0f;
	//float size = 10.0f;

	// 平行移動
	vec.x_ = -(float)transform["translation"][1] * size;
	vec.y_ = (float)transform["translation"][2] * size;
	vec.z_ = (float)transform["translation"][0] * size;
	objectData.translation_.SetVector3(vec);

	//float pi = 3.14;
	float angle = 60;

	// 回転角
	vec.x_ = -(float)transform["rotation"][1] / (angle);
	vec.y_ = -(float)transform["rotation"][2] / (angle);
	vec.z_ = -(float)transform["rotation"][0] / (angle);
	objectData.rotation_.SetVector3(vec);

	// スケーリング
	vec.x_ = (float)transform["scaling"][1] * size;
	vec.y_ = (float)transform["scaling"][2] * size;
	vec.z_ = (float)transform["scaling"][0] * size;
	objectData.scaling_.SetVector3(vec);
}

//再帰関数
LevelData* JsonFileOpen::CheckObjects(nlohmann::json deserialised, LevelData* levelData)
{
	//objectsの全オブジェクトを走査
	for (nlohmann::json& object : deserialised["objects"])
	{
		//チェック
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種別の処理

		//Mesh
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects_.emplace_back(LevelData::ObjectData{});

			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects_.back();

			//ファイルネームだったら
			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName_ = object["file_name"];
			}

			//Transformのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//transform格納
			SetMatrix(transform, objectData);
		}
	}

	return levelData;
}