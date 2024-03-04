#include "ObjFile.h"
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include <cassert>
using namespace std;

bool ObjFile::ReadFile()
{
	if (file_ == NULL) {
		return false;
	}
	while (true)
	{
		char lineHeader[128] = { 0 };

		// 行の最初の文字列を読み込みます。
		uint32_t res = fscanf_s(file_, "%s", &lineHeader, static_cast<uint32_t>(_countof(lineHeader)));

		if (res == EOF)
		{
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			Vector3D vertex;
			fscanf_s(file_, "%f %f %fn", &vertex.x_, &vertex.y_, &vertex.z_);
			tempVertices_.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vector2D uv;
			fscanf_s(file_, "%f %fn", &uv.x_, &uv.y_);
			tempUvs_.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vector3D normal;
			fscanf_s(file_, "%f %f %fn", &normal.x_, &normal.y_, &normal.z_);
			tempNormals_.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			int32_t vertexIndex[3] = { 0,0,0 }, uvIndex[3] = { 0,0,0 }, normalIndex[3] = { 0,0,0 };
			int32_t matches = fscanf_s(file_, "%d/%d/%d %d/%d/%d %d/%d/%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				return false;
			}
			vertexIndices_.push_back((unsigned short)vertexIndex[0]);
			vertexIndices_.push_back((unsigned short)vertexIndex[1]);
			vertexIndices_.push_back((unsigned short)vertexIndex[2]);
			uvIndices_.push_back((unsigned short)uvIndex[0]);
			uvIndices_.push_back((unsigned short)uvIndex[1]);
			uvIndices_.push_back((unsigned short)uvIndex[2]);
			normalIndices_.push_back((unsigned short)normalIndex[0]);
			normalIndices_.push_back((unsigned short)normalIndex[1]);
			normalIndices_.push_back((unsigned short)normalIndex[2]);
		}
	}
	return true;
}

ObjFile::ObjFile(const std::string& filename, std::vector<Vertex>& out_vertices)
{
	const char* filename_ = filename.c_str();

	fopen_s(&file_, filename_, "r");

	if (ReadFile()) {
		out_vertices.resize(vertexIndices_.size());
		for (auto i = 0; i < vertexIndices_.size(); i++)
		{
			int32_t vertexIndex = vertexIndices_[i];
			out_vertices[i].pos_ = tempVertices_[vertexIndex - 1];
			int32_t uvIndex = uvIndices_[i];
			out_vertices[i].uv_ = tempUvs_[uvIndex - 1];
			out_vertices[i].uv_.y_ = 1.0f - out_vertices[i].uv_.y_;
			int32_t normalIndex = normalIndices_[i];
			out_vertices[i].normal_ = tempNormals_[normalIndex - 1];
		}
	}

	fclose(file_);
}
