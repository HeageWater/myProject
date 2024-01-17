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
#include "BaseEnemy.h"
#include "BulletEnemy.h"
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
	void SetCamera(Matrix view, Matrix prodaction);

	/// <summary>
	/// シェーダーとパイプラインをセット
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline"></param>
	void SetModel(Shader shader, GPipeline* pipeline);

	/// <summary>
	/// ステージの情報を渡す
	/// </summary>
	/// <returns></returns>
	std::vector<Stage*> GetStage();

	/// <summary>
	/// エネミーの情報を渡す
	/// </summary>
	/// <returns></returns>
	//std::vector<Enemy*> GetEnemy();
	std::vector<BaseEnemy*> GetEnemy();

	/// <summary>
	/// 読み込んだステージの開始地点を返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetStartPos() { return StartPos_; };

	/// <summary>
	/// 読み込んだステージのゴール地点を返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetEndPos() { return EndPos_; };

	/// <summary>
	/// 読み込んだステージのイベント地点を返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetEvwntPos() { return EventPos_; };

	//
	//void EventCollision();

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
	//std::map<std::string, Enemy*> newEnemy_;

	//制作時に使用
	std::vector<Stage*> stages_;
	//std::vector<Enemy*> enemies_;
	std::vector<BaseEnemy*> enemies_;

	//カメラのposを記録しておく
	std::vector<Vector3D*> cameraPos_;

	Matrix view_;
	Matrix prodaction_;

	size_t enemyTex_;
	size_t stageTex_;

	Shader shader_;
	GPipeline* pipeline_;

	//スタート位置を格納する
	Vector3D StartPos_;
	//ゴール位置を格納する
	Vector3D EndPos_;
	//イベント位置を格納する
	Vector3D EventPos_;
	Vector3D EventScale_;

	//イベント時のカメラの位置を格納する
	Vector3D EventCameraPos_;
};