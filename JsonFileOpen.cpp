#include "JsonFileOpen.h"
#include <fstream>
#include <cassert>

const std::string JsonFileOpen::SkDefaultBaseDirectory = "Resources/levels/";
const std::string JsonFileOpen::SkExtension = ".json";

LevelData* JsonFileOpen::FileOpen(const std::string& fileName)
{
	//パスを連結する
	const std::string fullpath = SkDefaultBaseDirectory + fileName + SkExtension;

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

	//再帰
	//levelData = CheckObjects(deserialised, levelData);

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
			levelData->objects.emplace_back(LevelData::ObjectData{});

			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			//ファイルネームだったら
			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//Transformのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//transform格納
			SetMatrix(transform, objectData);
		}
		else if ((type.compare("MESH") == 0)
		{
			//チェック
			assert(object.contains("type"));

			//種別を取得
			std::string type = object["type"].get<std::string>();

			//種別の処理

			//Mesh
			if (type.compare("CAMARA") == 0)
			{
				//要素追加
				levelData->objects.emplace_back(LevelData::ObjectData{});

				//今追加した要素の参照を得る
				LevelData::ObjectData& objectData = levelData->objects.back();

				//ファイルネームだったら
				if (object.contains("file_name"))
				{
					//ファイル名
					objectData.fileName = object["file_name"];
				}

				//Transformのパラメータ読み込み
				nlohmann::json& transform = object["transform"];

				//transform格納
				SetMatrix(transform, objectData);
			}
		}
	}
	

	return levelData;
}

//transform格納
void JsonFileOpen::SetMatrix(nlohmann::json& transform, LevelData::ObjectData& objectData)
{
	//格納用
	Vector3D vec;

	// 平行移動
	vec.x_ = (float)transform["translation"][0];
	vec.y_ = -(float)transform["translation"][1];
	vec.z_ = (float)transform["translation"][2];
	objectData.translation.SetVector3(vec);

	// 回転角
	vec.x_ = (float)transform["rotation"][0];
	vec.y_ = (float)transform["rotation"][1];
	vec.z_ = -(float)transform["rotation"][2];
	objectData.rotation.SetVector3(vec);

	// スケーリング
	vec.x_ = (float)transform["scaling"][0];
	vec.y_ = (float)transform["scaling"][1];
	vec.z_ = (float)transform["scaling"][2];
	objectData.scaling.SetVector3(vec);
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
			levelData->objects.emplace_back(LevelData::ObjectData{});

			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			//ファイルネームだったら
			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//Transformのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//transform格納
			SetMatrix(transform, objectData);
		}

		//子がいるか
		if (object.contains("children"))
		{
			////チェック
			//assert(object.contains("type"));

			////種別を取得
			//std::string type = object["type"].get<std::string>();

			////種別の処理

			////Mesh
			//if (type.compare("MESH") == 0)
			//{
			//	//要素追加
			//	levelData->objects.emplace_back(LevelData::ObjectData{});

			//	//今追加した要素の参照を得る
			//	LevelData::ObjectData& objectData = levelData->objects.back();

			//	//ファイルネームだったら
			//	if (object.contains("file_name"))
			//	{
			//		//ファイル名
			//		objectData.fileName = object["file_name"];
			//	}

			//	//Transformのパラメータ読み込み
			//	nlohmann::json& transform = object["transform"];

			//	//transform格納
			//	SetMatrix(transform, objectData);
			//}
		}
	}

	return levelData;
}