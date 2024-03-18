#pragma once
#include<list>
#include"Collision.h"
#include <forward_list>
#include <Imgui.h>
#include"GameModel.h"

class CollisionManager
{
private:
	//全ての判定があるオブジェクトをここに
	std::list<GameModel*>objects;

public:

	/// <summary>
	/// 全ての当たり判定
	/// </summary>
	void Update();

	/// <summary>
	/// 判定追加
	/// </summary>
	/// <param name="object"></param>
	void AddCollision(GameModel* object);

	/// <summary>
	/// 判定変更
	/// </summary>
	/// <param name="object"></param>
	void SubCollision(GameModel* object);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

public:

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static CollisionManager* GetInstance();

private:

	/// <summary>
	/// 円の判定
	/// </summary>
	/// <param name="player"></param>
	/// <param name="enemy"></param>
	/// <param name="scale"></param>
	/// <returns></returns>
	bool CircleCollision(MyMath::ObjMatrix object1, MyMath::ObjMatrix object2);

	/// <summary>
	/// 四角の判定
	/// </summary>
	/// <param name="player"></param>
	/// <param name="enemy"></param>
	/// <returns></returns>
	bool BoxCollision(MyMath::ObjMatrix object1, MyMath::ObjMatrix object2);

	/// <summary>
	/// AABB
	/// </summary>
	/// <returns></returns>
	//bool AABBCollision(MyMath::ObjMatrix object1, MyMath::ObjMatrix object2);

	CollisionManager() = default;
	~CollisionManager() = default;

	//コピーコンストラクタ・代入演算子削除
	CollisionManager& operator=(const CollisionManager&) = delete;
	CollisionManager(const CollisionManager&) = delete;
};