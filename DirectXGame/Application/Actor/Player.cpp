#include "Player.h"

Player::Player()
{
	player_.mat.Initialize();
	player_.mat.scale = { 3,3,3 };

	playerAttack_.mat.Initialize();
	playerAttack_.mat.scale = { 3,3,3 };

	attackF = false;

	sound_ = nullptr;
	volcano = 0;
}

Player::~Player()
{
	for (size_t i = 0; i < attack.size(); i++)
	{
		delete attack[i];
	}
}

void Player::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	player_.Initialize(dx_, shader, "Resources\\kyu\\kyu.obj", pipeline_);

	player_.mat.Initialize();
	player_.mat.scale = { 5,5,5 };
	player_.mat.trans.x = 950;
	player_.mat.trans.y = 11;
	player_.mat.trans.z = 0;

	playerAttack_.Initialize(dx_, shader, "Resources\\maru\\maru.obj", pipeline_);

	playerAttack_.mat.Initialize();
	playerAttack_.mat.scale = { 5,5,5 };
	playerAttack_.mat.trans = player_.mat.trans;

	controller = Controller::GetInstance();
	attackF = false;
	createAttackFlag = false;

	sound_ = MyXAudio::Get();
	volcano = sound_->SoundLoadWave("Resources/sound/BGM.wav");
}

void Player::Draw(size_t tex, size_t tex2)
{
	player_.Draw(tex);

	if (attackF)
	{
		playerAttack_.Draw(tex2);
	}

	for (size_t i = 0; i < attack.size(); i++)
	{
		attack[i]->Draw();
	}
}

Vector2D Player::MoveCamera(Matrix matView, Matrix matProjection, Input* input)
{
	controller->Update();

	Vector3D move = { 0 ,0 ,0 };

	move.x += input->GetKey(DIK_D) - input->GetKey(DIK_A);
	move.z += input->GetKey(DIK_W) - input->GetKey(DIK_S);

	player_.mat.trans += move;

	player_.MatUpdate(matView, matProjection);

	return Vector2D(move.x, move.z);
}

void Player::Update(Matrix matView, Matrix matProjection, MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	//�R���g���[���[Update
	controller->Update();

	//�ړ��p�ϐ�
	Vector3D colVec = { 0,0,0 };

	//���X�e�B�b�N�̊p�x���
	colVec = { controller->GetLeftStickVec().x, 0,0 };

	//�W�����v
	Jump();

	//���W�ɉ��Z
	player_.mat.trans += colVec;

	//�U��
	Attack(dx_, shader, pipeline_);

	//player�ł̉��e�X�g
	/*if (controller->ButtonTriggerPush(RT))
	{
		sound_->SoundPlayWave(2);
	}*/

	player_.mat.trans.y = max(player_.mat.trans.y, 11);
	player_.mat.trans.x = min(player_.mat.trans.x, 1050);
	player_.mat.trans.x = max(player_.mat.trans.x, 0);

	for (size_t i = 0; i < attack.size(); i++)
	{
		attack[i]->Update(matView, matProjection);

		if (attack[i]->GetIsDead())
		{
			attack.erase(attack.begin() + i);
		}
	}

	//���WUpdate
	player_.MatUpdate(matView, matProjection);
	playerAttack_.MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	player_.mat.trans.x = 0;
	player_.mat.trans.y = 0;
}

void Player::Jump()
{
	float gravity = 0.2f;
	float maxGravity = 3;
	float nowY = 11;
	float jump = 1.0f;
	float maxJunp = 10;

	if (jumpPower > 0)
	{
		jumpPower -= jump;
	}

	if (gravirtPower < maxGravity)
	{
		gravirtPower += gravity;
	}

	if (controller->ButtonTriggerPush(LT))
	{
		if (player_.mat.trans.y == nowY)
		{
			jumpPower = maxJunp;
			gravirtPower = 0;
		}
	}

	player_.mat.trans.y += jumpPower;
	player_.mat.trans.y -= gravirtPower;
}

void Player::Attack(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	//playre�̍��W
	playerAttack_.mat.trans = player_.mat.trans;

	//�`�悵�Ȃ�
	attackF = false;

	//�E�X�e�B�b�N�̒l
	Vector2D Rstick = controller->GetRightStickVec();

	//�Œ჉�C��
	Vector2D minLine = { 0.3f,0.3f };

	//�f�b�h�]�|�������Ă邩
	bool moreLineX = minLine.x < Rstick.x || -minLine.x > Rstick.x;
	bool moreLineY = minLine.y < Rstick.y || -minLine.y > Rstick.y;

	//�f�b�h�]�|�������Ă邩
	if (moreLineX || moreLineY)
	{
		//normalize
		Rstick.normalize();

		//�ړ��l
		float move = 30;
		Vector3D movePos = { move * Rstick.x, -(move * Rstick.y),0 };

		//�ڑ�
		playerAttack_.mat.trans += movePos;
		//playerAttack_.mat.rotAngle.z = movePos.x;

		//�`�悷��
		attackF = true;

		//attackModel����
		if (createAttackFlag)
		{
			PlayerAttack* newAttack = new PlayerAttack(dx_, shader, pipeline_);

			Vector3D pos = player_.mat.trans;
			newAttack->SetPos(pos);

			Vector2D vec = controller->GetRightStickVec();
			vec *= -1;
			newAttack->SetVec(vec);

			newAttack->SetUpdate();

			attack.push_back(newAttack);

			createAttackFlag = false;
		}
	}
	else if (!createAttackFlag)
	{
		//attackModel����
		createAttackFlag = true;
	}
}

void Player::PopPlayerAttack()
{
}

bool Player::CollisionAttackToEnemy(Model enemy)
{
	for (size_t i = 0; i < attack.size(); i++)
	{
		Vector3D nowPos = attack[i]->GetPos();
		Vector3D nowScale = attack[i]->GetScale();

		float a = (enemy.mat.trans.x - nowPos.x) * (enemy.mat.trans.x - nowPos.x);
		float b = (enemy.mat.trans.y - nowPos.y) * (enemy.mat.trans.y - nowPos.y);

		float c = enemy.mat.scale.x * nowScale.x;

		//�����蔻��
		if (a + b < c)
		{
			attack.erase(attack.begin() + i);
			return true;
		}
	}
	return false;
}
