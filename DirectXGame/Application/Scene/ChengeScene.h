#pragma once
#include "Sprite.h"

/// <summary>
/// シーンチェンジ用クラス
/// </summary>
class ChengeScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="matProjection"></param>
	void Initialize(Matrix matProjection);

	/// <summary>
	/// 描画(テクスチャセットあり)
	/// </summary>
	/// <param name="tex"></param>
	void Draw(size_t tex);

	/// <summary>
	/// 描画(テクスチャセットなし)
	/// </summary>
	void Draw();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 再利用可能に
	/// </summary>
	void Reset();

	/// <summary>
	/// これを一回呼ぶとシーンチェンジが始まる
	/// </summary>
	/// <param name="next">次に飛ぶシーンの名前</param>
	void SetPlayFlag(std::string next);

	/// <summary>
	/// 今動いているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetPlayFlag() { return isPlayFlag; };

	/// <summary>
	/// 今どれくらい動いたか
	/// </summary>
	/// <returns></returns>
	float GetTime() { return time; };

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static ChengeScene* GetInstance();
private:

	//これをONにするとシーンチェンジが開始する
	bool isPlayFlag = false;

	//sprite
	//SpriteCommon* spriteCommon = SpriteCommon::Get();
	SpriteCommon* spriteCommon = new SpriteCommon();
	Sprite* sprite_ = new Sprite();

	size_t tex = 0;

	float time = 0;

	std::string next_;
private:

	ChengeScene() = default;
	~ChengeScene() = default;

	//コピーコンストラクタ・代入演算子削除
	ChengeScene& operator=(const ChengeScene&) = delete;
	ChengeScene(const ChengeScene&) = delete;
};