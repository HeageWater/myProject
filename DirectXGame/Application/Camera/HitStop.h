#pragma once

/// <summary>
/// 画面を止める処理
/// </summary>
class HitStop
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	HitStop();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HitStop();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// どれくらい止めるかを引数に
	/// </summary>
	/// <param name="time"></param>
	void SetTime(float time);

	/// <summary>
	/// あと何フレームかを取得する
	/// </summary>
	/// <returns></returns>
	bool GetTimeFlag() { return time_ > 0; };

private:
	float time_ = 0;
	bool countFlag_;
};