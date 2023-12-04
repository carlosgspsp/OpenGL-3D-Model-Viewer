#pragma once
#include <vector>
#include "Mesh.h"
#include <Math/float3.h>



namespace DirectX
{
	class ScratchImage;
}


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
	const float3 GetMinPos() const { return minPos; }

	Model();
	~Model();

private:
	tinygltf::Model srcModel;
	std::vector<DirectX::ScratchImage*> scrImages;
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
	std::string filePath;
	float3 minPos, maxPos;
};

