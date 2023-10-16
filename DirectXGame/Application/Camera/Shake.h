#pragma once
#include "MyMath.h"

/// <summary>
/// 画面揺れ
/// </summary>
class Shake
{
private:
	float time_ = 0;
	bool countFlag;
	Vector3D shake;
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Shake();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Shake();

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
};