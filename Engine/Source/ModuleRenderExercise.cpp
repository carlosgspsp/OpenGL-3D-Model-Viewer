#include "Application.h"
#include "SDL.h"
#include "ModuleWindow.h"
#include "ModuleRenderExercise.h"
#include <.\GL\glew.h>
#include <iostream>
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "DebugDraw.h"
#include "ModuleTexture.h"





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



	ModuleCamera* camera = App->GetCamera();

	return true;
}

update_status ModuleRenderExercise::Update() {

	RenderVBO(vbo);
	
		
	

	return UPDATE_CONTINUE;
}

float4x4 ModuleRenderExercise::LookAt(float3 camera_pos, float3 target_pos, float3 up_vector) {
	float3 forward = (target_pos - camera_pos).Normalized();
	float3 right = Cross(forward, up_vector).Normalized();
	float3 up = Cross(right, forward).Normalized();

	return float4x4(right[0], up[0], -forward[0], camera_pos[0], right[1], up[1], -forward[1], camera_pos[1], right[2], up[2], -forward[2], camera_pos[2], 0, 0, 0, 1);


}

unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] =
	{ 
		-1.0f, -1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f, 
		
		1.0f, -1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f,

		//0.0f, 0.0f,
		//1.0f,0.0f,
		//0.0f,1.0f,
		
		//1.0f, 0.0f,
		//1.0f,1.0f,
		//0.0f,1.0f,

		1.0f,1.0f,
		0.0f,1.0f,
		1.0f,0.0f,

		0.0f, 1.0f,
		0.0f,0.0f,
		1.0f,0.0f

	
	};

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

	ModuleTexture tex;
	
	unsigned texture_id = tex.LoadTextureGPU(tex.LoadTextureFile(L"./Textures/Baboon.ppm"));

	int w;
	int h;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);


	float4x4 model, camera, view, proj;

	float3 translation(0.0f, 0.0f, 0.0f);

	float3 camera_pos(0.0f, 4.0f, 8.0f);
	float3 target_pos(0.0f, 0.0f, 0.0f);

	model = float4x4::FromTRS(translation, float4x4::RotateZ(0), float3(1.0f, 1.0f, 1.0f));
	
	camera = LookAt(camera_pos, target_pos, float3::unitY);

	//float4x4 multiplication_matrix(float4(1,0,0,0), float4(0,1,0,0), float4(0,0,1,0), float4(-translation[0], -translation[1], -translation[2], 1));

	//view = camera.Transposed() * multiplication_matrix;
	view = camera.Inverted();

	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f)) * ((float)(w))/((float)(h));
	
	//view = frustum.ViewMatrix();
	proj = frustum.ProjectionMatrix();
	
	
	dd::axisTriad(float4x4::identity, 0.1f, 1.0f);
	dd::xzSquareGrid(-10, 10, 0.0f, 1.0f, dd::colors::Gray);
	App->GetDebugDraw()->Draw(view, proj,w, h);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	//TEXTURES
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*3*3*2));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(glGetUniformLocation(program_id,"mytexture"), 0);


	glUseProgram(program_id);
	// 1 triangle to draw = 3 vertices

	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &vbo);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteProgram(program_id);
	DestroyVBO(vbo);
	return true;
}


