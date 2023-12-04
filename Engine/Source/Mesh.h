#pragma once
#include <vector>
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#include "tiny_gltf.h"


namespace tinygltf
{
	class Model;
	class Mesh;
	class Primitive;
}

class Mesh
{
private:
	unsigned VBO = 0, EBO = 0, VAO = 0, programID = 0, textureID = 0;
	int vertexCount = 0, indexCount = 0;
	size_t posByteOffset = 0, posByteStride = 0, texByteOffset = 0, texByteStride = 0;
	std::string name = "";
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

