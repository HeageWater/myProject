#include "CollisionManager.h"

//判定が必要なもの一覧
//player -> enemy,bullet,stage,goal
//enemy -> player,bullet,stage,attack
//bullet -> player,stage,enemy
//stage -> player,enemy,bullet
//attack -> enemy,bullet
//goal -> player

void CollisionManager::Update()
{
	//全ての判定のあるオブジェクトをこのリストに
	objects.remove_if([](GameModel* object) { return object->GetDeleteFlag(); });

	for (auto object1 : objects)
	{
		bool IsName = object1->GetName() == "";

		for (auto object2 : objects)
		{
			//object1.


		}
	}

}

void CollisionManager::AddCollision(GameModel* object)
{
	objects.push_back(object);
}

void CollisionManager::SubCollision(GameModel* object)
{
	objects.remove(object);
}

void CollisionManager::Reset()
{
	objects.clear();
}

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::CircleCollision(Model player, Model enemy, float scale)
{
	//scale
	float Scale = ((player.mat_.scale_.x_ * scale) + enemy.mat_.scale_.x_);

	//判定
	float Dist = (player.mat_.trans_ - enemy.mat_.trans_).length();

	return Dist <= Scale;
}
