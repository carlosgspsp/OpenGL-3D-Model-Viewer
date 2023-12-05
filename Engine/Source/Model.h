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

	inline const tinygltf::Model* GetSrcModel() const { return srcModel; }
	inline const std::vector<Mesh*>* GetMeshes() const { return &meshes; } 
	inline const std::vector<DirectX::ScratchImage*> GetScrImages() const { return scrImages; }
	inline const float3 GetMaxPos() const { return maxPos; }
	inline const float3 GetMinPos() const { return minPos; }

	Model();
	~Model();

private:
	tinygltf::Model* srcModel = nullptr;
	std::vector<DirectX::ScratchImage*> scrImages;
	std::vector<unsigned> textures;
	std::vector<Mesh*> meshes;
	std::string filePath = "";
	float3 minPos = float3::zero, maxPos = float3(FLT_MAX, FLT_MAX, FLT_MAX);
};

