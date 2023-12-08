#include "Application.h"
#include <.\GL\glew.h>
#include "Mesh.h"
#include "MathGeoLib.h"
#include "SDL.h"
#include "ModuleCamera.h"


Mesh::Mesh() {
	meshAABB = new AABB();
}
Mesh::~Mesh() {
	delete meshAABB;
}

void Mesh::Load(const tinygltf::Model& srcModel, const tinygltf::Mesh& srcMesh, const tinygltf::Primitive& primitive) {
	name = srcMesh.name;
	textureID = primitive.material;
	LoadVBO(srcModel, srcMesh, primitive);
	LoadEBO(srcModel, srcMesh, primitive);
	if (indexCount != 0) {
		CreateVAO();
	}
}

void Mesh::LoadVBO(const tinygltf::Model& srcModel, const tinygltf::Mesh& srcMesh, const tinygltf::Primitive& primitive) {
	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itTexCoord = primitive.attributes.find("TEXCOORD_0");
	const auto& itNormal = primitive.attributes.find("NORMAL");

	size_t bufferSize = 0;

	if (itPos != primitive.attributes.end()) {
		bufferSize += sizeof(float) * 3;
	}
	if (itTexCoord != primitive.attributes.end()) {
		bufferSize += sizeof(float) * 2;
	}
	if (itNormal != primitive.attributes.end()) {
		bufferSize += sizeof(float) * 3;
	}

	if (itPos != primitive.attributes.end()) {
		const tinygltf::Accessor& posAcc = srcModel.accessors[itPos->second];
		vertexCount = posAcc.count;

		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		posAcc.maxValues;
		const tinygltf::BufferView& posView = srcModel.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = srcModel.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);


		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,  bufferSize *= posAcc.count , nullptr, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		for (size_t i = 0; i < posAcc.count; i++) {
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			if (posView.byteStride != 0) {
				bufferPos += posView.byteStride;
			}
			else {
				bufferPos += sizeof(float) * 3;
			}
			
		}
		meshAABB->SetFrom(ptr, vertexCount);
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}



	if (itTexCoord != primitive.attributes.end()) {
		const tinygltf::Accessor& texCoordAcc = srcModel.accessors[itTexCoord->second];
		SDL_assert(texCoordAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texCoordAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& texCoordView = srcModel.bufferViews[texCoordAcc.bufferView];
		const tinygltf::Buffer& texCoordBuffer = srcModel.buffers[texCoordView.buffer];
		const unsigned char* bufferTexCoord = &(texCoordBuffer.data[texCoordAcc.byteOffset + texCoordView.byteOffset]);

		textureCount = texCoordAcc.count;

		float2* ptr = reinterpret_cast<float2*>(reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))+sizeof(float)*3*vertexCount);
		//float2* ptr = reinterpret_cast<float2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))+(sizeof(float)*3*vertexCount)/sizeof(float2); //This does not work because the division should encapsulate everything
		

		for (size_t i = 0; i < texCoordAcc.count; i++) {
			ptr[i] = *reinterpret_cast<const float2*>(bufferTexCoord);
			if (texCoordView.byteStride != 0) {
				bufferTexCoord += texCoordView.byteStride;
			}
			else {
				bufferTexCoord += sizeof(float) * 2;
			}

		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}


	if (itNormal != primitive.attributes.end()) {
		const tinygltf::Accessor& normalAcc = srcModel.accessors[itNormal->second];
		SDL_assert(normalAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(normalAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& normalView = srcModel.bufferViews[normalAcc.bufferView];
		const tinygltf::Buffer& normalBuffer = srcModel.buffers[normalView.buffer];
		const unsigned char* bufferNormal = &(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);

		float3* ptr = nullptr;
		if (itTexCoord != primitive.attributes.end()) {
			ptr = reinterpret_cast<float3*>(reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) + sizeof(float) * 5 * vertexCount);
		}
		else {
			ptr = reinterpret_cast<float3*>(reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) + sizeof(float) * 3 * vertexCount);
		}
		
		//float2* ptr = reinterpret_cast<float2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))+(sizeof(float)*3*vertexCount)/sizeof(float2); //This does not work because the division should encapsulate everything


		for (size_t i = 0; i < normalAcc.count; i++) {
			ptr[i] = *reinterpret_cast<const float3*>(bufferNormal);
			if (normalView.byteStride != 0) {
				bufferNormal += normalView.byteStride;
			}
			else {
				bufferNormal += sizeof(float) * 3;
			}

		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}

}

void Mesh::LoadEBO(const tinygltf::Model& srcModel, const tinygltf::Mesh& srcMesh, const tinygltf::Primitive& primitive) {

	if (primitive.indices >= 0) {

		const tinygltf::Accessor& indAcc = srcModel.accessors[primitive.indices];
		const tinygltf::BufferView& indView = srcModel.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(srcModel.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);
		indexCount = indAcc.count;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		uint32_t* ptr = reinterpret_cast<uint32_t*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT) {
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; i++) {
				ptr[i] = bufferInd[i];
			}
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT) {
			const unsigned short* bufferInd = reinterpret_cast<const unsigned short*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; i++) {
				ptr[i] = bufferInd[i];
			}
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE) {
			const unsigned char* bufferInd = reinterpret_cast<const unsigned char*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; i++) {
				ptr[i] = bufferInd[i];
			}
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	else {
		indexCount = 0;
	}

}

void Mesh::CreateVAO() {
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));

	glEnableVertexAttribArray(2);
	if (textureCount != 0) {
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 5 * vertexCount));
	}
	else {
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));
	}
	

	glBindVertexArray(0);
}

void Mesh::Draw(const std::vector<unsigned>& textures, unsigned program_id) {

	glUseProgram(program_id);

	if (textures.size() > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[textureID]);
		glUniform1i(glGetUniformLocation(program_id, "diffuse_texture"), 0);
	}

	glUniform1f(glGetUniformLocation(program_id, "diffuse_constant"), 0.640f);
	glUniform1f(glGetUniformLocation(program_id, "specular_constant"), 0.550f);
	glUniform1f(glGetUniformLocation(program_id, "n"), 29.25f);

	glUniform3f(glGetUniformLocation(program_id, "light_color"), 0.992f, 0.857f, 0.510f);
	glUniform3f(glGetUniformLocation(program_id, "light_direction"), -0.800f, 8.100f, -6.700);
	glUniform3f(glGetUniformLocation(program_id, "ambient_color"), 0.802f, 0.739f, 0.739f);
	glUniform3f(glGetUniformLocation(program_id, "camera_position"), App->GetCamera()->GetPosition()->x, App->GetCamera()->GetPosition()->y, App->GetCamera()->GetPosition()->z);

	if (indexCount > 0) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
	else { //Without index and with textures coords
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*3*vertexCount));

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	}
	

}


void Mesh::DestroyBuffers() {
	glDeleteBuffers(1, &VBO);
	if (indexCount != 0) {
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VAO);
	}
	
}