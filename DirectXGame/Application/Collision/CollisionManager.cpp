#include "CollisionManager.h"
#include "ParticleManager.h"
#include "Shake.h"
#include "HitStop.h"

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
					Vector3D playVec = (object2->GetPos() - object1->GetPos());

					//弾を向きを変える
					object2->SetVec(playVec.normalize());

					//playerとbulletの判定
					if (!CircleCollision(object1->GetMat(), object2->GetMat()))
					{
						continue;
					}

					//プレイヤーにダメージ
					object1->OnCollision();

					//プレイヤーの位置にパーティクル
					ParticleManager::GetInstance()->CreateBoxParticle(object1->GetPos());

					//弾を消す
					object2->OnCollision();
					object2->Destroy();
				}

				//enemyなら
				if (name2 == "enemy")
				{
					//playerとbulletの判定
					if (!CircleCollision(object1->GetMat(), object2->GetMat()))
					{
						continue;
					}

					//プレイヤーにダメージ
					object1->OnCollision();

					//プレイヤーの位置にパーティクル
					ParticleManager::GetInstance()->CreateBoxParticle(object1->GetPos());
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
					if (!CircleCollision(object1->GetMat(), object2->GetMat()))
					{
						continue;
					}

					//弾を向きを変える
					object2->SetVec(object1->GetPos() - object2->GetPos());

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
					if (!BoxCollision(object1->GetMat(), object2->GetMat()))
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

bool CollisionManager::CircleCollision(MyMath::ObjMatrix object1, MyMath::ObjMatrix object2)
{
	//scale
	float Scale = (object1.scale_.x_ + object2.scale_.x_);

	//判定
	float Dist = (object1.trans_ - object2.trans_).length();

	return Dist <= Scale;
}

bool CollisionManager::BoxCollision(MyMath::ObjMatrix object1, MyMath::ObjMatrix object2)
{
	float a = (object1.trans_.x_ - object2.trans_.x_) * (object1.trans_.x_ - object2.trans_.x_);
	float b = (object1.trans_.y_ - object2.trans_.y_) * (object1.trans_.y_ - object2.trans_.y_);

	float c = object1.scale_.x_ + object2.scale_.x_ * 100;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;
}