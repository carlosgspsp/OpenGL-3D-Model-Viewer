#include "ModuleRenderExercise.h"
#include <.\GL\glew.h>
#include <iostream>

ModuleRenderExercise::ModuleRenderExercise() {

}

bool ModuleRenderExercise::Init() {
	vbo = CreateTriangleVBO();

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

	program_id = program.CreateProgram(vertex_shader_id, fragment_shader_id);
	return true;
}

update_status ModuleRenderExercise::Update() {

	RenderVBO(vbo);
	return UPDATE_CONTINUE;
}



unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	return vbo;
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderVBO(unsigned vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// 1 triangle to draw = 3 vertices 
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteProgram(program_id);
	DestroyVBO(vbo);
	return true;
}