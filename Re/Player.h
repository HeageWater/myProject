//#pragma once
//#include "Model.h"
//#include "Stage.h"
//#include "Input.h"
//class Player
//{
//private:
//	Stage* stage = nullptr;
//	Input* input = nullptr;
//	Model model;
//	//std::vector<int> lvMaxSpd;
//	//static const int MaxLv = 5;
//	//int level;
//
//	bool prevOnStage;
//	bool onStage;
//
//	bool isLive;
//	Vector2D pos;
//	float r = 16.0f;
//	float dis;	//	プレイヤーと中心の距離
//	float prevDis;
//	float angle;
//
//	Vector2D spd;
//	float maxSpd;
//	float minSpd;
//
//	//	heavyAttack
//	bool hAttack;
//	float hAttackSpd;
//	//bool hAttackHalf;	//	feaver用
//
//	////	スタン
//	//bool stun;
//	//int stunTime;
//
//	//	KnockBack
//	bool knockBack;
//	//int kbTime;
//	//Vector2 backSpd;	//	x:spd,y:dis
//
//	//	combo
//	//int maxCombo;
//	int combo;
//
//	void RespornInit();
//	void SetComboSpd();
//	void SetModelPos();
//	void SetAccel();
//	void SetPosLimit();
//	void PosUpdate();
//	void SetPrevInfo();
//	void SetOnStage(bool collision);
//public:
//	Player(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, Stage* stage_, Input* input_);
//	void Initialize();
//	void Update(Matrix matView, Matrix matProjection);
//	void Draw(int handle);
//
//	//void KnockBack(Vector2& e_spd);
//	Vector2D GetPos() {
//		Vector2D c_center(r * cos(angle * MyMath::PIx2), r * sin(angle * MyMath::PIx2));
//		return pos - c_center;
//	};
//	//void AddCombo() { combo++; }
//};
//
