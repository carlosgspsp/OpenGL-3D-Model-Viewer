#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleProgram.h"
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
	unsigned vbo;
	unsigned program_id;

	unsigned CreateTriangleVBO();
	void RenderVBO(unsigned vbo);
	void DestroyVBO(unsigned vbo);

	
	
};

