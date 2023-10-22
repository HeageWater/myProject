#pragma once
#include <vector>
#include "MyMath.h"
#include "VertBuff.h"

/// <summary>
/// objを開くクラス
/// </summary>
class ObjFile
{
private:

	//ファイル変数
	FILE* file;
	//std::FILE* file = NULL;
	
	//頂点データ
	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;

	//頂点
	std::vector<Vector3D> temp_vertices;

	//uv
	std::vector<Vector2D> temp_uvs;

	//normal
	std::vector<Vector3D> temp_normals;

	//頂点データサイズ
	size_t vertexSize;

	//uvデータサイズ
	size_t uvSize;

	//normalデータサイズ
	size_t normalSize;

	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <returns></returns>
	bool ReadFile();

public:

	ObjFile(const char* filename, std::vector<Vertex>& out_vertices);
};

