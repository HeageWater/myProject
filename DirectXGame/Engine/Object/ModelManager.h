#pragma once
#include <vector>
#include <map>
#include <string>
#include "MyMath.h"
#include "VertBuff.h"
#include "Model.h"

class ModelManager
{
private:
	//読み込んだことのあるモデルをここに格納
	std::map<std::string, std::unique_ptr<Model>> models;

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
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static ModelManager* GetInstance();
private:

	ModelManager() = default;
	~ModelManager() = default;

	//コピーコンストラクタ・代入演算子削除
	ModelManager& operator=(const ModelManager&) = delete;
	ModelManager(const ModelManager&) = delete;
};