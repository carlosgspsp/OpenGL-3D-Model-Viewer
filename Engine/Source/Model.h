#pragma once
#include <vector>
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "tiny_gltf.h"
#include "DirectXTex/DirectXTex.h"
#include "Mesh.h"
#include <Math/float3.h>



class Model
{
public:
	void Load(const char* assetFileName);
	void LoadMaterials();
	void DrawModel(unsigned program_id);
	void Clear();

	const tinygltf::Model* GetSrcModel() const { return &srcModel; }
	const std::vector<Mesh>* GetMeshes() const { return &meshes; } 
	const std::vector<DirectX::ScratchImage*> GetScrImages() const { return scrImages; }
	const float3 GetMaxPos() const { return maxPos; }

	~Model();

private:
	tinygltf::Model srcModel;
	std::vector<DirectX::ScratchImage*> scrImages;
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
	std::string filePath;
	float3 maxPos;
};

