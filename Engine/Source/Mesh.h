#pragma once
#include <vector>
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "tiny_gltf.h"

class Mesh
{
private:
	unsigned VBO, EBO, VAO, programID, textureID;
	int vertexCount, indexCount;
	size_t posByteOffset, posByteStride, texByteOffset, texByteStride;
	std::string name;
public:
	
	int GetIndexCount() const { return indexCount; }
	int GetVertexCount() const { return vertexCount; }
	std::string GetName() const { return name; }

	void Load(const tinygltf::Model& srcModel, const tinygltf::Mesh& srcMesh, const tinygltf::Primitive& primitive);
	void LoadVBO(const tinygltf::Model& srcModel, const tinygltf::Mesh& srcMesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& srcModel, const tinygltf::Mesh& srcMesh, const tinygltf::Primitive& primitive);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& textures, unsigned program_id);
	void DestroyBuffers();
};

