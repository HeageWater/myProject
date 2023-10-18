#pragma once
#pragma once
#include <string>
#include <vector>
#include "MyMath.h"

#pragma warning (push)
#pragma warning (disable:26800)
#include "json.hpp"
#pragma warning (pop)

// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;

		//追加要素
		std::string otherName;

		//Matrix
		Matrix matrix;

		// 平行移動
		Vector3D translation;
		// 回転角
		Vector3D rotation;
		// スケーリング
		Vector3D scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};

class JsonFileOpen
{
public:
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;

	// ファイル拡張子
	static const std::string kExtension;
public:
	//jsonファイルOpen
	static LevelData* FileOpen(const std::string& fileName);

	//transform格納
	static void SetMatrix(nlohmann::json& transform, LevelData::ObjectData& objectData);

	//再帰関数
	static LevelData* CheckObjects(nlohmann::json deserialised, LevelData* levelData);
};