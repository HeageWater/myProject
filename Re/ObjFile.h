#pragma once
#include <vector>
#include "MyMath.h"
#include "VertBuff.h"

class ObjFile
{
private:
	FILE* file = NULL;

	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3D> temp_vertices;
	std::vector<Vector2D> temp_uvs;
	std::vector<Vector3D> temp_normals;
	size_t vertexSize;
	size_t uvSize;
	size_t normalSize;
	bool ReadFile();
public:
	ObjFile(const char* filename, std::vector<Vertex>& out_vertices);
};

