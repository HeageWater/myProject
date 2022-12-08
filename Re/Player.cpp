#include "Player.h"
//
//void Player::RespornInit()
//{
//	isLive = true;
//	dis = stage->GetMaxR();
//	//prevDis = dis;
//	hAttack = false;
//
//	knockBack = false;
//	//kbTime = 0;
//
//	//stun = false;
//	//stunTime = 0;
//
//	//combo = 0;
//}
//
//void Player::SetComboSpd()
//{
//}
//
//void Player::SetModelPos()
//{
//	float angle_ = angle * MyMath::PIx2;
//	Vector2D c_center(r * cos(angle_), r * sin(angle_));
//	model.mat.rotAngle.z = angle_;
//	model.mat.trans = { pos.x - c_center.x ,pos.y - c_center.y ,0.0f };
//}
//
//void Player::SetAccel()
//{
//	if (!knockBack) {
//		spd.x += 0.01f;
//
//		if (dis == stage->GetMaxR() && !stage->Feaver()) {
//			//	壁ずり(Feaver中はなし)
//			if (spd.x > minSpd) {
//				spd.x -= 0.015f;
//			}
//			if (spd.x < minSpd) {
//				spd.x = minSpd;
//			}
//		}
//		else {
//			if (spd.x > maxSpd) {
//				spd.x = maxSpd;
//			}
//		}
//	}
//}
//
//void Player::SetPosLimit()
//{
//	if (dis >= stage->GetMaxR()) {
//		if (stage->OnCollision(angle)) {
//			dis = stage->GetMaxR();
//			//combo = 0;
//			spd.y = 0.0f;
//		}
//	}
//
//	if (dis <= stage->GetMinR()) {
//		//stage->DamageCircle(combo);
//		dis = stage->GetMinR();
//		hAttack = true;
//		hAttackSpd = 20.0f;
//	}
//}
//
//void Player::PosUpdate()
//{
//#pragma region 速度処理
//	angle -= spd.x / (float)dis * MyMath::PIx2;
//#pragma endregion
//	if (angle <= -1) angle += 1;
//
//#pragma region 座標更新
//	float angle_ = angle * MyMath::PIx2;
//	pos.x = cos(angle_) * dis;
//	pos.y = sin(angle_) * dis;
//#pragma endregion
//}
//
//void Player::SetPrevInfo()
//{
//	prevOnStage = onStage;
//	prevDis = dis;
//}
//
//void Player::SetOnStage(bool collision)
//{
//}
//
//Player::Player(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, Stage* stage_, Input* input_)
//{
//	stage = stage_;
//	input = input_;
//	model.Initialize(dx_, shader, "Resource\\Model\\box.obj", pipeline_);
//	model.mat.scale = { r,r,2.0f };
//
//	Initialize();
//}
//
//void Player::Initialize()
//{
//	onStage = false;
//	prevOnStage = false;
//	maxSpd = 0.1f;
//	minSpd = 0.05f;
//	angle = 0.0f;
//
//	//maxCombo = 15;
//
//	RespornInit();
//}
//
//void Player::Update(Matrix matView, Matrix matProjection)
//{
//	SetPrevInfo();
//
//	if (isLive) {
//		SetAccel();
//
//		if (stage->Feaver()) {
//
//		}
//		else {
//#pragma region heavyAttack
//			if (hAttack) {
//				hAttackSpd += 1.0f;
//				dis += hAttackSpd;
//				if (dis >= stage->GetMaxR()) {
//					if (stage->OnCollision(angle, true, combo)) {
//						dis = stage->GetMaxR();
//						hAttack = false;
//						combo = 0;
//						spd.x = 0.0f;
//						spd.y = 0.0f;
//					}
//				}
//			}
//#pragma endregion
//
//#pragma region キー入力
//			if (input->GetKey(DIK_SPACE)) {
//				spd.y += 0.2f;
//				if (spd.y > 5.0f) spd.y = 5.0f;
//				dis -= spd.y;
//			}
//			else {
//				spd.y -= 0.5f;
//				if (spd.y < -3.0f) spd.y = -3.0f;
//				if (!hAttack) {
//					dis -= spd.y;
//				}
//			}
//#pragma endregion
//
//			SetPosLimit();
//
//			onStage = stage->OnCollision(angle + spd.x / (float)dis * MyMath::PIx2);
//
//#pragma region 死亡
//			if (onStage && !prevOnStage && dis > stage->GetMaxR() + r * 2) {
//				stage->SetRespornAngle(angle);
//				pos.x = cos(angle * MyMath::PIx2) * dis;
//				pos.y = sin(angle * MyMath::PIx2) * dis;
//				isLive = false;
//			}
//			if (dis > stage->GetMaxR() + 100) {
//				isLive = false;
//			}
//#pragma endregion
//
//			if (dis == stage->GetMaxR() && prevDis < stage->GetMaxR()) {
//				if (stage->OnCollision(angle, true, combo)) {
//					dis = stage->GetMaxR();
//					combo = 0;
//					spd.y = 0.0f;
//				}
//			}
//			else if (dis == stage->GetMaxR()) {
//				//	ついている間ダメージ
//			}
//		}
//		PosUpdate();
//	}
//	else {
//		if (input->GetTrigger(DIK_R)) {
//			if (dis > stage->GetDeadR()) {
//				stage->SetRespornAngle(angle);
//				RespornInit();
//			}
//			else {
//				RespornInit();
//			}
//		}
//	}
//
//	SetModelPos();
//	model.MatUpdate(matView, matProjection);
//}
//
//void Player::Draw(int handle)
//{
//	Vector2D center;
//
//	model.Draw(handle);
//}
