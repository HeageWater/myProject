#pragma once
#include "ImGuiManager.h"
#include "Player.h"


/// <summary>
/// ゲーム中の自機
/// </summary>
class MoviePlayer
{

	//関数
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MoviePlayer();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MoviePlayer();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(Shader shader, GPipeline* pipeline_);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	/// <param name="tex2"></param>
	void Draw(size_t tex);

	/// <summary>
	/// プレイヤー基準でカメラ動かす
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="input"></param>
	/// <returns></returns>
	Vector2D MoveCamera(Matrix matView, Matrix matProjection, Input* input);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="shader"></param>
	void Update(Matrix matView, Matrix matProjection, Shader shader);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// posを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return player_.mat_.trans_; };

	/// <summary>
	///posをセットする
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3D pos) { player_.mat_.trans_ = pos; };

	/// <summary>
	/// rotaionを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetRot() { return player_.mat_.rotAngle_; };

	/// <summary>
	/// scaleを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetScale() { return player_.mat_.scale_; };

	/// <summary>
	/// modelを返す
	/// </summary>
	/// <returns></returns>
	Model GetModel() { return player_; };

	/// <summary>
	/// matview,matProdactionをセット
	/// </summary>
	void SetCamera(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool PlayerSet() { return player_.mat_.trans_.y_ <= 17.0f; };

private:

private:

	GPipeline* pipeline_;

	//音
	MyXAudio* sound_ = nullptr;

	//モデルデータ
	Model player_;

	//移動用変数
	Vector3D colVec_ = { 0,0,0 };

	Matrix matView_;
	Matrix matProjection_;

	bool vecFlag_;

	size_t time_;
	float MaxTime = 1;
};
