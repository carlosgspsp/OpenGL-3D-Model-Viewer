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
	std::string error, warning;


	bool loadOk = gltfContext.LoadASCIIFromFile(&srcModel , &error, &warning, assetFileName);

	if (!loadOk) {
		LOG("Error loading s%: s%", assetFileName, error.c_str());
	}
	else {
		filePath = assetFileName;
		size_t pos = filePath.rfind('/');
		if (pos != std::string::npos) {
			filePath.erase(pos + 1, filePath.size() - 1);
		}else{
			pos = filePath.rfind('\\');
			if (pos != std::string::npos) {
				filePath.erase(pos + 1, filePath.size() - 1);
			}
		}
		for (const auto& srcMesh : srcModel.meshes) {
			for (const auto& primitive : srcMesh.primitives) {
				Mesh* mesh = new Mesh;
				mesh->Load(srcModel, srcMesh, primitive);
				meshes.push_back(*mesh);
			}
		}
		LoadMaterials();
	}
}

void Model::LoadMaterials() {
	for (const auto& srcMaterial : srcModel.materials) {
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0) {
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			

			std::string path = filePath + image.uri;
			std::wstring widestr = std::wstring(path.begin(), path.end());

			DirectX::ScratchImage* scrImage = new DirectX::ScratchImage();
			App->GetTextureModule()->LoadTextureFile(*scrImage, widestr.c_str());
			textureId = App->GetTextureModule()->LoadTextureGPU(scrImage);
			scrImages.push_back(scrImage);
			//textureId = App->GetTextureModule()->Load(filePath+image.uri);
		}
		textures.push_back(textureId);
	}
}


void Model::DrawModel(unsigned program_id) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes.at(i).Draw(textures, program_id);
	}
}


void Model::Clear() {
	textures.clear();
	meshes.clear();
	srcModel = tinygltf::Model();
	filePath = "";
}