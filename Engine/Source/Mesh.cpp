#include "Application.h"
#include <.\GL\glew.h>
#include "Mesh.h"
#include "MathGeoLib.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "DebugDraw.h"

#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"


void Mesh::CreateProgram() {
	ModuleProgram program;
	const char* vertex_shader_file = "../Source/VertexShader.glsl";
	const char* fragment_shader_file = "../Source/FragmentShader.glsl";
	char* vertex_shader_source = program.LoadShaderSource(vertex_shader_file);
	char* fragment_shader_source = program.LoadShaderSource(fragment_shader_file);
	std::cout << "hola;";
	LOG("VERTEX");
	LOG(vertex_shader_source);
	LOG("FRAGMENT");
	LOG(fragment_shader_source);
	unsigned vertex_shader_id = program.CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
	unsigned fragment_shader_id = program.CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);
	programID = program.CreateProgram(vertex_shader_id, fragment_shader_id);

}
void Mesh::LoadVBO() {
	float vtx_data[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f,1.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
}

void Mesh::LoadEBO() {

	GLuint indices[] =
	{
		0, 1, 2,
		1, 3, 2

	};

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void Mesh::CreateVAO() {
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 4));

	glBindVertexArray(0);
}

void Mesh::Draw() {
	ModuleTexture tex;

	unsigned texture_id = tex.LoadTextureGPU(tex.LoadTextureFile(L"./Textures/Baboon.ppm"));

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(glGetUniformLocation(programID, "mytexture"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}