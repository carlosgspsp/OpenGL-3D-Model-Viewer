#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleProgram.h"
#include "MathGeoLib.h"
#include "Model.h"

class ModuleRenderExercise :
    public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	//update_status PreUpdate();
	update_status Update();
	//update_status PostUpdate();
	bool CleanUp();

	Model* GetModel() const { return model; }


private:
	ModuleProgram program;
	//unsigned vbo;
	unsigned program_id, texture_id;

	//unsigned CreateTriangleVBO();
	void RenderWorld();
	
	ModuleCamera* camera;
	Model* model;
};

