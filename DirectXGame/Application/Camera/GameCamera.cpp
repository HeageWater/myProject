#include "GameCamera.h"

GameCamera::GameCamera()
{

}

GameCamera::GameCamera(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	Init(_eye, _target, _up);
	frontVec = target - eye;
	disEyeTarget = frontVec.length();
}

void GameCamera::Update(Input& input)
{
	/*if (eye.x)
	{

	}*/

	MatUpdate();
}

void GameCamera::Init(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	eye = _eye;
	target = _target;
	up = _up;

	MatUpdate();
}

void GameCamera::MatUpdate()
{
	mat = MyMath::LookAtLH(eye, target, up);
}

void GameCamera::Move(float camerapos)
{
	eye.x = camerapos;
	target.x = camerapos;

	MatUpdate();
}
