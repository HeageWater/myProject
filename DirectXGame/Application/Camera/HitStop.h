#pragma once

/// <summary>
/// 画面を止める処理
/// </summary>
class HitStop
{
private:
	float time = 0;
	bool countFlag;
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
	float GetTime() { return time; };
};