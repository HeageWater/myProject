#pragma once
#include <vector>
#include <map>
#include <string>
#include "MyMath.h"
#include "VertBuff.h"
#include "Model.h"
#include "ObjFile.h"

#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"

class ModelManager
{
private:
	//読み込んだことのあるモデルをここに格納
	std::map<std::string, std::unique_ptr<Model>> models_;
	//std::unordered_map<std::string, std::unique_ptr<Model>> models_;

	//pipeline
	std::unique_ptr<GPipeline> pipeline_;

	//shader
	Shader shader_;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 破棄
	/// </summary>
	void Finalize();

	/// <summary>
	/// 
	/// </summary>
	void LoadModel(const std::string& filename);

	/// <summary>
	/// 
	/// </summary>
	void SetModel(const std::string& filename,Model* model);

	/// <summary>
	/// 指定されたモデルを返す
	/// </summary>
	/// <returns></returns>
	std::unique_ptr<Model> FindModel(const std::string& filename);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
public:

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