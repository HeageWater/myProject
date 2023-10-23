#pragma once
#include "JsonFileOpen.h"

#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include <memory>
#include "Model.h"
#include "Sound.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "Shader.h"

//ファイルを読み込んで反映するもの
#include "Enemy.h"
#include "Stage.h"

//warp
//cameraPos
//playerInitPos

/// <summary>
/// jsonファイルから読み込んだものを反映する
/// </summary>
class LoadObjectData
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 破棄
	/// </summary>
	void Finalize();

	/// <summary>
	///  ステージ読み込み
	/// </summary>
	/// <param name="filePath">読み込むファイル名(拡張子なし)</param>
	void StageLoad(const std::string& filePath);

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="view"></param>
	/// <param name="prodaction"></param>
	void SetCamera(Matrix view,Matrix prodaction);

	/// <summary>
	/// シェーダーとパイプラインをセット
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline"></param>
	void SetModel(Shader shader, GPipeline* pipeline);

	std::vector<Stage*> GetStage()
	{
		return stages_;
	}

private:

	LoadObjectData() = default;
	~LoadObjectData() = default;

	//コピーコンストラクタ・代入演算子削除
	LoadObjectData& operator=(const LoadObjectData&) = delete;
	LoadObjectData(const LoadObjectData&) = delete;

public:

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static LoadObjectData* GetInstance();

private:
	//ステージ読み込み
	LevelData* levelData_ = nullptr;

	//ステージ格納用
	std::map<std::string, Stage*> newStage_;
	std::map<std::string, Enemy*> newEnemy_;

	//制作時に使用
	std::vector<Stage*> stages_;
	std::vector<Enemy*> enemies_;

	Matrix view_;
	Matrix prodaction_;

	size_t enemyTex_;
	size_t stageTex_;

	Shader shader_;
	GPipeline* pipeline_;
};