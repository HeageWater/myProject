#pragma once
#include "MyMath.h"

/// <summary>
/// 画面揺れ
/// </summary>
class Shake
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// どれくらいの時間揺らすかをセット
	/// </summary>
	/// <param name="time"></param>
	void SetTime(float time);

	/// <summary>
	/// 残りフレームを取得
	/// </summary>
	/// <returns></returns>
	float GetTime() { return time_; };

	/// <summary>
	/// 揺れの値を返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetShake() { return shake_; };

private:
	float time_ = 0;
	bool countFlag_;
	Vector3D shake_;

public:
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static Shake* GetInstance();
private:

	Shake() = default;
	~Shake() = default;

	//コピーコンストラクタ・代入演算子削除
	Shake& operator=(const Shake&) = delete;
	Shake(const Shake&) = delete;
};