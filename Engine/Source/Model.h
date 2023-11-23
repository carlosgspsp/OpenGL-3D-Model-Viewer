#pragma once
#include <vector>
#include "Mesh.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "tiny_gltf.h"

class Model
{
public:
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model srcModel);
	void DrawModel(unsigned program_id);

private:
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
};

