#include "Globals.h"
#include "Application.h"
#include "Model.h"
#include "ModuleTexture.h"


#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE 
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"





void Model::Load(const char* assetFileName) {
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model srcModel;
	std::string error, warning;


	bool loadOk = gltfContext.LoadASCIIFromFile(&srcModel , &error, &warning, assetFileName);

	if (!loadOk) {
		LOG("Error loading s%: s%", assetFileName, error.c_str());
	}
	else {
		for (const auto& srcMesh : srcModel.meshes) {
			for (const auto& primitive : srcMesh.primitives) {
				Mesh* mesh = new Mesh;
				mesh->Load(srcModel, srcMesh, primitive);
				meshes.push_back(*mesh);
			}
		}
		LoadMaterials(srcModel);
	}
}

void Model::LoadMaterials(const tinygltf::Model srcModel) {
	for (const auto& srcMaterial : srcModel.materials) {
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0) {
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];

			textureId = App->GetTextureModule()->Load(image.uri);
		}
		textures.push_back(textureId);
	}
}


void Model::DrawModel(unsigned program_id) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes.at(i).Draw(textures, program_id);
	}
}