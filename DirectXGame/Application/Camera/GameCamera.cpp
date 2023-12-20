#include "GameCamera.h"

GameCamera::GameCamera()
{

}

GameCamera::GameCamera(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	//初期化
	Init(_eye, _target, _up);
	//frontVec = target - eye;
	//disEyeTarget = frontVec.length();
}

void GameCamera::Update()
{

	//更新
	MatUpdate();
}

void GameCamera::Init(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	matView_.eye_ = _eye;
	matView_.target_ = _target;
	matView_.up_ = _up;

	MatUpdate();
}

void GameCamera::MatUpdate()
{
	//matView_ = MyMath::LookAtLH(eye_, target_, up_);
}

void GameCamera::Move(float camerapos)
{
	matView_.eye_.x_ = camerapos;
	matView_.target_.x_ = camerapos;

	MatUpdate();
}
