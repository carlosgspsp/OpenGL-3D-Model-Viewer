#include "Application.h"
#include <.\GL\glew.h>
#include "Mesh.h"
#include "MathGeoLib.h"
#include "SDL.h"



void Mesh::Load(const tinygltf::Model& srcModel, const tinygltf::Mesh& srcMesh, const tinygltf::Primitive& primitive) {
	name = srcMesh.name;
	textureID = primitive.material;
	LoadVBO(srcModel, srcMesh, primitive);
	LoadEBO(srcModel, srcMesh, primitive);
	CreateVAO();
	//CreateProgram();
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

		posByteOffset = posView.byteOffset;
		posByteStride = posView.byteStride;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count*2, nullptr, GL_STATIC_DRAW);
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
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}



	if (itTexCoord != primitive.attributes.end()) {
		const tinygltf::Accessor& texCoordAcc = srcModel.accessors[itTexCoord->second];
		SDL_assert(texCoordAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texCoordAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& texCoordView = srcModel.bufferViews[texCoordAcc.bufferView];
		const tinygltf::Buffer& texCoordBuffer = srcModel.buffers[texCoordView.buffer];
		const unsigned char* bufferTexCoord = &(texCoordBuffer.data[texCoordAcc.byteOffset + texCoordView.byteOffset]);

		texByteOffset = texCoordAcc.byteOffset + texCoordView.byteOffset;
		texByteStride = texCoordView.byteStride;

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

	glBindVertexArray(0);
}

void Mesh::Draw(const std::vector<unsigned>& textures, unsigned program_id) {

	glUseProgram(program_id);

	if (textures.size() > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[textureID]);
		glUniform1i(glGetUniformLocation(program_id, "mytexture"), 0);
	}

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
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);
}