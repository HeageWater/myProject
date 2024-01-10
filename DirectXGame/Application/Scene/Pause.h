#pragma once

class Pause
{
public:

	/// <summary>
	/// 終了フラグをONに
	/// </summary>
	void SetEndFlag() { isEndFlag_ = true; };

	/// <summary>
	/// 今動いているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetFlag() { return isEndFlag_; };

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static Pause* GetInstance();

private:

	//終了フラグがONか
	bool isEndFlag_ = false;
};