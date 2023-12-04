#include "Application.h"
#include <.\GL\glew.h>
#include "ModuleWindow.h"
#include "SDL.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "DebugDraw.h"
#include "Model.h"
#include "Math/float2.h"
#include "Math/float3.h"
#include "Math/float4x4.h"


ModuleRenderExercise::ModuleRenderExercise() {
	model = new Model();
}

ModuleRenderExercise::~ModuleRenderExercise() {
	delete model;
}
bool ModuleRenderExercise::Init() {

	

	ModuleProgram program;
	const char* vertex_shader_file = "./Shaders/VertexShader.glsl";
	const char* fragment_shader_file = "./Shaders/FragmentShader.glsl";
	char* vertex_shader_source = program.LoadShaderSource(vertex_shader_file);
	char* fragment_shader_source = program.LoadShaderSource(fragment_shader_file);

	LOG("VERTEX");
	LOG(vertex_shader_source);
	LOG("FRAGMENT");
	LOG(fragment_shader_source);

	unsigned vertex_shader_id = program.CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
	unsigned fragment_shader_id = program.CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);
	program_id = program.CreateProgram(vertex_shader_id, fragment_shader_id);

	

	//model.Load("./Models/TriangleWithoutIndices/TriangleWithoutIndices.gltf");
	//model.Load("./Models/Triangle/Triangle.gltf");
	//model.Load("./Models/BoxInterleaved/BoxInterleaved.gltf");
	//model.Load("./Models/Box/Box.gltf");
	//model.Load("./Models/BoxTextured/BoxTextured.gltf");
	model->Load("./Models/BakerHouse/BakerHouse.gltf");
	//model.Load("./Models/Duck/Duck.gltf");
	
	camera = App->GetCamera();

	return true;
}

update_status ModuleRenderExercise::Update() {

	RenderWorld();
	
	model->DrawModel(program_id);
	
	return UPDATE_CONTINUE;
}

/*unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] =
	{ 
		-1.0f, -1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f, 
		
		1.0f, -1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f,

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
}*/


void ModuleRenderExercise::RenderWorld()
{
	
	float2 screenSize = App->GetWindow()->GetScreenSize();
	float4x4 model_matrix, view_matrix, proj_matrix;
	float3 translation(0.0f, 0.0f, 0.0f);
	model_matrix = float4x4::FromTRS(translation, float4x4::RotateZ(0), float3(1.0f, 1.0f, 1.0f));
	
	camera->SetFOV(60.0f); 

	view_matrix = camera->GetViewMatrix();
	proj_matrix = camera->GetProjectionMatrix();
	
	dd::axisTriad(float4x4::identity, 0.1f, 1.0f);
	dd::xzSquareGrid(-10, 10, 0.0f, 1.0f, dd::colors::Gray);
	App->GetDebugDraw()->Draw(view_matrix, proj_matrix,screenSize.x, screenSize.y);

	glUseProgram(program_id);
	glUniformMatrix4fv(0, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj_matrix[0][0]);
	
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteProgram(program_id);
	return true;
}

 Model* ModuleRenderExercise::GetModel() const { return model; }


