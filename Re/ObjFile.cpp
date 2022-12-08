#include "ObjFile.h"
#include <string>

bool ObjFile::ReadFile()
{
	if (file == NULL) {
		return false;
	}
	while (true)
	{
		char lineHeader[128];
		// �s�̍ŏ��̕������ǂݍ��݂܂��B
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF)	break;

		if (strcmp(lineHeader, "v") == 0) {
			Vector3D vertex;
			fscanf_s(file, "%f %f %fn", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vector2D uv;
			fscanf_s(file, "%f %fn", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vector3D normal;
			fscanf_s(file, "%f %f %fn", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	return true;
}

ObjFile::ObjFile(const char* filename, std::vector<Vertex>& out_vertices)
{
	fopen_s(&file, filename, "r");
	
	if (ReadFile()) {
		out_vertices.resize(vertexIndices.size());
		for (size_t i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			out_vertices[i].pos = temp_vertices[vertexIndex - 1];
			unsigned int uvIndex = uvIndices[i];
			out_vertices[i].uv = temp_uvs[uvIndex - 1];
			out_vertices[i].uv.y = 1.0f - out_vertices[i].uv.y;
			unsigned int normalIndex = normalIndices[i];
			out_vertices[i].normal = temp_normals[normalIndex - 1];
		}
	}
	fclose(file);
}
