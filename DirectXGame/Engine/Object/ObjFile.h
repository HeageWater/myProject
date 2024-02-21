#pragma once
#include <vector>
#include <string>
#include "MyMath.h"
#include "VertBuff.h"

/// <summary>
/// objを開くクラス
/// </summary>
class ObjFile
{
private:

	//ファイル変数
	FILE* file_;
	//std::FILE* file = NULL;

	//頂点データ
	std::vector<unsigned short> vertexIndices_, uvIndices_, normalIndices_;

	//頂点
	std::vector<Vector3D> tempVertices_;

	//uv
	std::vector<Vector2D> tempUvs_;

	//normal
	std::vector<Vector3D> tempNormals_;

	//頂点データサイズ
	size_t vertexSize_;

	//uvデータサイズ
	size_t uvSize_;

	//normalデータサイズ
	size_t normalSize_;

	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <returns></returns>
	bool ReadFile();
public:

	ObjFile(const char* filename, std::vector<Vertex>& out_vertices);
};

