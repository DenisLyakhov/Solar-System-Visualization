//vcpkg install assimp
#pragma once

#include<iostream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h> 
#include <assimp/cimport.h>


class Model
{
	private:
	const aiScene* scene;
	Assimp::Importer importer;

	struct Mesh {
		unsigned int VAO, VBO2, VBO3, EBO; //Buffer
	};

};