#pragma once
#include <vector>
#include <Math/float3.h>

namespace DirectX
{
	class ScratchImage;
}

namespace tinygltf
{
	class Model;
}

class Mesh;

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
	inline const AABB* GetAABB() const { return modelAABB; }
	Model();
	~Model();

private:
	tinygltf::Model* srcModel = nullptr;
	std::vector<DirectX::ScratchImage*> scrImages;
	std::vector<unsigned> textures;
	std::vector<Mesh*> meshes;
	std::string filePath = "";
	AABB* modelAABB;
};

