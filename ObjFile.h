#pragma once
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "VertBuff.h"

class ObjFile
{
private:
	FILE* file = NULL;

	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;
	int vertexSize;
	int uvSize;
	int normalSize;
	bool ReadFile();
public:
	ObjFile(const char* filename, std::vector<Vertex>& out_vertices);
};

