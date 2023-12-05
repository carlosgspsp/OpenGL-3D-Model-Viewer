#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Model.h"
#include "DirectXTex/DirectXTex.h"
#include <.\GL\glew.h>

Model::Model() {
	srcModel = new tinygltf::Model;
}

Model::~Model() {

	delete srcModel;

	for (int i = 0; i < scrImages.size(); i++) {
		delete scrImages[i];
	}

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->DestroyBuffers();
		delete meshes[i];
	}

	for (int i = 0; i < textures.size(); i++) {
		glDeleteTextures(1, &textures[i]);
	}
}

void Model::Load(const char* assetFileName) {
	tinygltf::TinyGLTF gltfContext;
	std::string error, warning;

	maxPos = float3::zero;

	bool loadOk = gltfContext.LoadASCIIFromFile(srcModel , &error, &warning, assetFileName);

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
		for (const auto& srcMesh : srcModel->meshes) {
			for (const auto& primitive : srcMesh.primitives) {
				Mesh* mesh = new Mesh;
				mesh->Load(*srcModel, srcMesh, primitive);
				
				const auto& itPos = primitive.attributes.find("POSITION");
				if (itPos != primitive.attributes.end()) {
					const tinygltf::Accessor& posAcc = srcModel->accessors[itPos->second];
					
					if (posAcc.maxValues[0] > maxPos.x) {
						maxPos.x = posAcc.maxValues[0];
					}
					if (posAcc.maxValues[1] > maxPos.y) {
						maxPos.y = posAcc.maxValues[1];
					}
					if (posAcc.maxValues[2] > maxPos.z) {
						maxPos.z = posAcc.maxValues[2];
					}

					if (posAcc.minValues[0] < minPos.x) {
						minPos.x = posAcc.minValues[0];
					}
					if (posAcc.minValues[1] < minPos.y) {
						minPos.y = posAcc.minValues[1];
					}
					if (posAcc.minValues[2] < minPos.z) {
						minPos.z = posAcc.minValues[2];
					}


				}

				meshes.push_back(mesh);
			}
		}
		LoadMaterials();
	}
}

void Model::LoadMaterials() {
	for (const auto& srcMaterial : srcModel->materials) {
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0) {
			const tinygltf::Texture& texture = srcModel->textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel->images[texture.source];
			

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
		meshes.at(i)->Draw(textures, program_id);
	}
}


void Model::Clear() {

	for (int i = 0; i < textures.size(); i++) {
		glDeleteTextures(1, &textures[i]);
	}
	textures.clear();
	delete srcModel;
	srcModel = new tinygltf::Model();

	for (int i = 0; i < scrImages.size(); i++) {
		delete scrImages[i];
	}
	scrImages.clear();

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->DestroyBuffers();
		delete meshes[i];
	}
	meshes.clear();
		
	filePath = "";
}