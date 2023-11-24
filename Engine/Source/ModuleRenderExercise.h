#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleProgram.h"
#include "MathGeoLib.h"
#include "Mesh.h"
#include "Model.h"

class ModuleRenderExercise :
    public Module
{
public:
	ModuleRenderExercise();
	//~ModuleRenderExercise();

	bool Init();
	//update_status PreUpdate();
	update_status Update();
	//update_status PostUpdate();
	bool CleanUp();


private:
	ModuleProgram program;
	//unsigned vbo;
	unsigned program_id;
	unsigned texture_id;

	unsigned CreateTriangleVBO();
	void RenderWorld();
	void DestroyVBO(unsigned vbo);
	float4x4 LookAt(float3 camera_pos, float3 target_pos, float3 up_vector);
	
	ModuleCamera* camera;


	Mesh mesh;
	Model model;
};

