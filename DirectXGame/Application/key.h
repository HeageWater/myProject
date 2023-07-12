#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include <DirectXMath.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Key
{
private:

	//�L�[�{�[�h�f�o�C�X�̐���
	IDirectInputDevice8* keyboard = nullptr;

	//DirectInput�̏�����
	IDirectInput8* directInput = nullptr;

public:

	HRESULT result;

	BYTE keys[256] = {};

	//�S�L�[�̓��͏�Ԃ�ۑ�����
	BYTE oldkey[256] = {};

	//�C���X�^���X
	Key();
	Key(WNDCLASSEX a, HWND hw);
	~Key();

	//�X�V
	void Update();
	//��������
	bool Push(uint8_t key);
	//�����Ă��
	bool Keep(uint8_t key);
	//�������u��
	bool Release(uint8_t key);
};