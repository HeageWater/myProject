#include "ModelManager.h"

void ModelManager::Initialize()
{
}

void ModelManager::Update()
{
}

void ModelManager::Draw()
{
}

void ModelManager::Finalize()
{
}

ModelManager* ModelManager::GetInstance()
{
	static ModelManager modelManager;
	return &modelManager;
}
