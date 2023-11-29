#pragma once
#include <vector>
#include "Mesh.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "tiny_gltf.h"
#include "DirectXTex/DirectXTex.h"

class Model
{
public:
	void Load(const char* assetFileName);
	void LoadMaterials();
	void DrawModel(unsigned program_id);
	void Clear();
	const tinygltf::Model GetSrcModel() { return srcModel; }
	const std::vector<Mesh> GetMeshes() { return meshes; }
	const std::vector<DirectX::ScratchImage*> GetScrImages() { return scrImages; }



private:
	tinygltf::Model srcModel;
	std::vector<DirectX::ScratchImage*> scrImages;
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
	std::string filePath;
};

