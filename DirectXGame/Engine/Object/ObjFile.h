#pragma once
#include <vector>
#include <string>
#include "MyMath.h"
#include "VertBuff.h"
#include "Model.h"

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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="out_vertices"></param>
	ObjFile(const std::string& filename, std::vector<Vertex>& out_vertices);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="model"></param>
	/// <param name="out_vertices"></param>
	ObjFile(const std::string& filename, Model* model, std::vector<Vertex>& out_vertices);

	//ObjFile(const std::string& filename, std::vector<Vertex>& out_vertices);
};

