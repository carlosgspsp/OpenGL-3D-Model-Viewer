#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleDebugDraw* GetDebugDraw() { return debug_draw; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleTexture* GetTextureModule() { return textureModule; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleRenderExercise* render_exercise = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleDebugDraw* debug_draw = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleTexture* textureModule = nullptr;

    std::list<Module*> modules;

};

extern Application* App;
