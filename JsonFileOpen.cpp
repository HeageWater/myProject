#include "JsonFileOpen.h"
#include <fstream>
#include <cassert>

const std::string JsonFileOpen::SkDefaultBaseDirectory = "Resources/levels/";
const std::string JsonFileOpen::SkExtension = ".json";

LevelData* JsonFileOpen::FileOpen(const std::string& fileName)
{
	//�p�X��A������
	const std::string fullpath = SkDefaultBaseDirectory + fileName + SkExtension;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);

	//�t�@�C���`�F�b�N
	if (file.fail())
	{
		//���s���Ă邩�ǂ���
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialised;

	// ��
	file >> deserialised;

	//���������`�F�b�N
	assert(deserialised.is_object());
	assert(deserialised.contains("name"));
	assert(deserialised["name"].is_string());

	//name�𕶎���Ƃ��Ď擾
	std::string name = deserialised["name"].get < std::string>();

	//���������`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();

	//�ċA
	//levelData = CheckObjects(deserialised, levelData);

	//objects�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialised["objects"])
	{
		//�`�F�b�N
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ʂ̏���

		//Mesh
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::ObjectData{});

			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = levelData->objects.back();

			//�t�@�C���l�[����������
			if (object.contains("file_name"))
			{
				//�t�@�C����
				objectData.fileName = object["file_name"];
			}

			//Transform�̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];

			//transform�i�[
			SetMatrix(transform, objectData);
		}
		else if ((type.compare("MESH") == 0)
		{
			//�`�F�b�N
			assert(object.contains("type"));

			//��ʂ��擾
			std::string type = object["type"].get<std::string>();

			//��ʂ̏���

			//Mesh
			if (type.compare("CAMARA") == 0)
			{
				//�v�f�ǉ�
				levelData->objects.emplace_back(LevelData::ObjectData{});

				//���ǉ������v�f�̎Q�Ƃ𓾂�
				LevelData::ObjectData& objectData = levelData->objects.back();

				//�t�@�C���l�[����������
				if (object.contains("file_name"))
				{
					//�t�@�C����
					objectData.fileName = object["file_name"];
				}

				//Transform�̃p�����[�^�ǂݍ���
				nlohmann::json& transform = object["transform"];

				//transform�i�[
				SetMatrix(transform, objectData);
			}
		}
	}
	

	return levelData;
}

//transform�i�[
void JsonFileOpen::SetMatrix(nlohmann::json& transform, LevelData::ObjectData& objectData)
{
	//�i�[�p
	Vector3D vec;

	// ���s�ړ�
	vec.x_ = (float)transform["translation"][0];
	vec.y_ = -(float)transform["translation"][1];
	vec.z_ = (float)transform["translation"][2];
	objectData.translation.SetVector3(vec);

	// ��]�p
	vec.x_ = (float)transform["rotation"][0];
	vec.y_ = (float)transform["rotation"][1];
	vec.z_ = -(float)transform["rotation"][2];
	objectData.rotation.SetVector3(vec);

	// �X�P�[�����O
	vec.x_ = (float)transform["scaling"][0];
	vec.y_ = (float)transform["scaling"][1];
	vec.z_ = (float)transform["scaling"][2];
	objectData.scaling.SetVector3(vec);
}

//�ċA�֐�
LevelData* JsonFileOpen::CheckObjects(nlohmann::json deserialised, LevelData* levelData)
{
	//objects�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialised["objects"])
	{
		//�`�F�b�N
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ʂ̏���

		//Mesh
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::ObjectData{});

			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = levelData->objects.back();

			//�t�@�C���l�[����������
			if (object.contains("file_name"))
			{
				//�t�@�C����
				objectData.fileName = object["file_name"];
			}

			//Transform�̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];

			//transform�i�[
			SetMatrix(transform, objectData);
		}

		//�q�����邩
		if (object.contains("children"))
		{
			////�`�F�b�N
			//assert(object.contains("type"));

			////��ʂ��擾
			//std::string type = object["type"].get<std::string>();

			////��ʂ̏���

			////Mesh
			//if (type.compare("MESH") == 0)
			//{
			//	//�v�f�ǉ�
			//	levelData->objects.emplace_back(LevelData::ObjectData{});

			//	//���ǉ������v�f�̎Q�Ƃ𓾂�
			//	LevelData::ObjectData& objectData = levelData->objects.back();

			//	//�t�@�C���l�[����������
			//	if (object.contains("file_name"))
			//	{
			//		//�t�@�C����
			//		objectData.fileName = object["file_name"];
			//	}

			//	//Transform�̃p�����[�^�ǂݍ���
			//	nlohmann::json& transform = object["transform"];

			//	//transform�i�[
			//	SetMatrix(transform, objectData);
			//}
		}
	}

	return levelData;
}