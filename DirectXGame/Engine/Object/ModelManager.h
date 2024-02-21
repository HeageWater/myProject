#pragma once
#include <vector>
#include <map>
#include <string>
#include "MyMath.h"
#include "VertBuff.h"
#include "Model.h"

class ModelManager
{
private:
	//読み込んだことのあるモデルをここに格納
	std::map<std::string, std::unique_ptr<Model>> models;


};