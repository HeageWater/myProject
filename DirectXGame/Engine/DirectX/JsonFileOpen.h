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
struct LevelData
{
	struct ObjectData 
	{
		// ファイル名
		std::string fileName_;

		//追加要素
		std::string otherName_;

		//Matrix
		Matrix matrix_;

		// 平行移動
		Vector3D translation_;
		// 回転角
		Vector3D rotation_;
		// スケーリング
		Vector3D scaling_;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects_;
};

/// <summary>
/// jsonファイルを開くクラス
/// </summary>
class JsonFileOpen
{
public:

	/// <summary>
	/// jsonファイルOpen
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static LevelData* FileOpen(const std::string& fileName);

	/// <summary>
	/// transform格納
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="objectData"></param>
	static void SetMatrix(nlohmann::json& transform, LevelData::ObjectData& objectData);

	/// <summary>
	/// 再帰関数
	/// </summary>
	/// <param name="deserialised"></param>
	/// <param name="levelData"></param>
	/// <returns></returns>
	static LevelData* CheckObjects(nlohmann::json deserialised, LevelData* levelData);

public:
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory_;

	// ファイル拡張子
	static const std::string kExtension_;
};