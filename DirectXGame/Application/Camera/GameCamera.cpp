#include "GameCamera.h"

void GameCamera::Initialize(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	matView_.eye_ = _eye;
	matView_.target_ = _target;
	matView_.up_ = _up;

}

void GameCamera::Update()
{
	//カメラ更新
	matView_.MatUpdate();
	//matView_ = MyMath::LookAtLH(eye_, target_, up_);
}

void GameCamera::Move(float camerapos)
{
	matView_.eye_.x_ = camerapos;
	matView_.target_.x_ = camerapos;

	Update();
}
