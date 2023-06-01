#pragma once
#pragma once
#include <string>
#include <vector>
#include "Re/MyMath.h"
#include "json.hpp"

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;

		//Matrix
		Matrix matrix;

		// ���s�ړ�
		Vector3D translation;
		// ��]�p
		Vector3D rotation;
		// �X�P�[�����O
		Vector3D scaling;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
};

class JsonFileOpen
{
public:
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;

	// �t�@�C���g���q
	static const std::string kExtension;
public:
	//json�t�@�C��Open
	static LevelData* FileOpen(const std::string& fileName);

	//transform�i�[
	static void SetMatrix(nlohmann::json& transform, LevelData::ObjectData& objectData);

	//�ċA�֐�
	static LevelData* CheckObjects(nlohmann::json deserialised, LevelData* levelData);
};