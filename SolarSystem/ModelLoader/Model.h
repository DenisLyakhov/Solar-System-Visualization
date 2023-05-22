#pragma once

#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<map>
#include<string>

class Model {
public:
	Model() {

	}
	~Model() {

	}
	int fileType;
	void loadFromFile(const char* path) {
		std::vector<Position> verticies;
		std::vector<Normal> normals;
		std::vector<Texture> textures;

		std::ifstream file(path);
		if (file) {
			char currentMtlName[100];
			std::string line;
			while (std::getline(file, line)) {
				if (startWith(line,"mtllib")) {
					char mtlFileName[100];
					(void)sscanf_s(line.c_str(), "newmtl1 $s", mtlFileName, sizeof(mtlFileName));
					loadMaterialFromFile(mtlFileName);
				}
				if (startWith(line, "v")) {
					Position pos;
					sscanf_s(line.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z);
					verticies.push_back(pos);
				}
				//Todo do zmiany !!!!
				if (startWith(line, "vn")) {
					Normal norm;
					sscanf_s(line.c_str(), "vn %f %f %f", &norm.x, &norm.y, &norm.z);
					normals.push_back(norm);
				}
				if (startWith(line, "vt")) {
					Texture texture;
					sscanf_s(line.c_str(), "vn %f %f %f", &texture.x, &texture.y, &texture.z);
					textures.push_back(texture);
				}
				if (startWith(line, "usemtl")) {
					(void)sscanf_s(line.c_str(), "newmtl1 $s", currentMtlName, sizeof(currentMtlName));

				}
				if (startWith(line, "f")) {
					int v1, v2, v3,v4;
					int n1, n2, n3,n4;
					sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &v1, &n1, &v2, &n2, &v3, &n3 , &v4, &n4);

					addVertexData(v1, n1, currentMtlName, verticies, textures);
					addVertexData(v2, n2, currentMtlName, verticies, textures);
					addVertexData(v3, n3, currentMtlName, verticies, textures);
					addVertexData(v4, n4, currentMtlName, verticies, textures);

				}
			}
		}
		else std::cout << "Loading file failed 1" << std::endl;

	}
	std::vector<float> getVertexData() {
		return VertexData;
	}
	int getVertexCount() {
		return VertexData.size() / 9;
	}
private:
	struct Position { float x, y, z; };
	struct Color{float r, g, b;};
	struct Normal{float x, y, z;};
	struct Texture { float x, y, z; };

	void loadMaterialFromFile(const char* path) {
		std::ifstream file(path);
		if (file) {
			std::string line;
			while (std::getline(file,line)) {
				char mtlName[100];
				if (startWith(line, "newmtl1")) {
					(void)sscanf_s(line.c_str(), "newmtl1 $s", mtlName, sizeof(mtlName));
					mMaterialMap[mtlName] = Color();
				}
				if (startWith(line, "Kd")) {
					Color& color = mMaterialMap[mtlName];
					sscanf_s(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);

				}
			}
		}
		else std::cout << "Loading file failed" << std::endl;
	}
	bool startWith(std::string& line, const char* text) {
		size_t textLen = strlen(text);
		if (line.size() < textLen) return false;
		
		for (size_t i = 0; i < textLen; i++) {
			if (line[i] != text[i]) return false;
		}

		return true;

	}

	void addVertexData(int v1, int n1, const char* mtl,
		std::vector<Position>& vertices, std::vector<Normal>& normals) {

		Color c = mMaterialMap[mtl];
		Position p = vertices[v1 - 1];
		Normal normal = normals[n1 - 1];

		VertexData.push_back(p.x);
		VertexData.push_back(p.y);
		VertexData.push_back(p.z);

		VertexData.push_back(c.r);
		VertexData.push_back(c.g);
		VertexData.push_back(c.b);

		VertexData.push_back(normal.x);
		VertexData.push_back(normal.y);
		VertexData.push_back(normal.z);
	}

	void addVertexData(int v1, int t1, const char* mtl,
		std::vector<Position>& vertices, std::vector<Texture>& texture) {

		Color c = mMaterialMap[mtl];
		Position p = vertices[v1 - 1];
		Texture text = texture[t1 - 1];

		VertexData.push_back(p.x);
		VertexData.push_back(p.y);
		VertexData.push_back(p.z);

		VertexData.push_back(c.r);
		VertexData.push_back(c.g);
		VertexData.push_back(c.b);

		VertexData.push_back(text.x);
		VertexData.push_back(text.y);
		VertexData.push_back(text.z);
	}

	void addVertexData(int v1, int n1, int t1, const char* mtl,
		std::vector<Position>& vertices, std::vector<Normal>& normals, std::vector<Texture>& texture) {

		Color c = mMaterialMap[mtl];
		Position p = vertices[v1 - 1];
		Normal normal = normals[n1 - 1];
		Texture text = texture[t1 - 1];

		VertexData.push_back(p.x);
		VertexData.push_back(p.y);
		VertexData.push_back(p.z);

		VertexData.push_back(c.r);
		VertexData.push_back(c.g);
		VertexData.push_back(c.b);

		VertexData.push_back(normal.x);
		VertexData.push_back(normal.y);
		VertexData.push_back(normal.z);

		VertexData.push_back(text.x);
		VertexData.push_back(text.y);
		VertexData.push_back(text.z);
	}


	std::map<std::string, Color> mMaterialMap;
	std::vector<float> VertexData;
};