#include "CollisionManager.h"

//判定が必要なもの一覧
//player -> enemy,bullet,stage,goal
//enemy -> player,bullet,stage,attack
//bullet -> player,stage,enemy
//stage -> player,enemy,bullet
//attack -> enemy,bullet
//goal -> player

//state
//main
//object
//attack

void CollisionManager::Update()
{
	//全ての判定のあるオブジェクトをこのリストに
	objects.remove_if([](GameModel* object) { return object->GetDeleteFlag(); });

	//
	for (auto object1 : objects)
	{
		//tagを入手
		std::string name = object1->GetName();

		//判定
		for (auto object2 : objects)
		{
			//tagを入手
			std::string name2 = object2->GetName();

			//tagが同じなら
			bool SameName = name == name2;

			//同じオブジェクトなら抜ける
			if (SameName)
			{
				continue;
			}

			//player
			if (name == "player")
			{
				//bulletなら
				if (name2 == "bullet")
				{
					//playerとbulletの判定
					if (!CircleCollision(object1->model_, object2->model_))
					{
						continue;
					}

					//プレイヤーにダメージ
					object1->OnCollision();

					//弾を消す
					object2->OnCollision();
					object2->Destroy();
				}

				//enemyなら
				if (name2 == "enemy")
				{

				}

				//stageなら
				if (name2 == "stage")
				{

				}

				//goalなら
				if (name2 == "goal")
				{

				}
			}

			//attack
			if (name == "attack")
			{
				//bulletなら
				if (name2 == "bullet")
				{
					//playerとbulletの判定
					if (!CircleCollision(object1->model_, object2->model_))
					{
						continue;
					}

					//弾を消す
					object2->OnCollision();
					object2->Destroy();
				}

				//enemyなら
				if (name2 == "enemy")
				{

				}
			}

			//bullet
			if (name == "bullet")
			{
				//enemyなら
				if (name2 == "enemy")
				{

				}

				//stageなら
				if (name2 == "stage")
				{
					if (!BoxCollision(object1->model_, object2->model_))
					{
						continue;
					}

					//弾を消す
					object2->OnCollision();
					object2->Destroy();
				}
			}
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

bool CollisionManager::CircleCollision(Model object1, Model object2)
{
	//scale
	float Scale = (object1.mat_.scale_.x_ + object2.mat_.scale_.x_);

	//判定
	float Dist = (object1.mat_.trans_ - object2.mat_.trans_).length();

	return Dist <= Scale;
}

bool CollisionManager::BoxCollision(Model object1, Model object2)
{
	float a = (object1.mat_.trans_.x_ - object2.mat_.trans_.x_) * (object1.mat_.trans_.x_ - object2.mat_.trans_.x_);
	float b = (object1.mat_.trans_.y_ - object2.mat_.trans_.y_) * (object1.mat_.trans_.y_ - object2.mat_.trans_.y_);

	float c = object1.mat_.scale_.x_ + object2.mat_.scale_.x_ * 100;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;
}