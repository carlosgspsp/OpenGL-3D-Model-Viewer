#pragma once

#include<list>
#include<vector>
#include "Globals.h"
#include "Module.h"
#include "SDL.h"
#include <.\GL\glew.h>

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
    ModuleEditor* GetEditor() { return editor; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleTexture* GetTextureModule() { return textureModule; }
    ModuleRenderExercise* GetModuleRenderExercise() { return render_exercise; }
    
    void RequestBrowser(const char* url);
    std::vector<float>* GetFrameRate() { return &frameRate; };
    std::vector<float>* GetMilliseconds() { return &milliSeconds; };



    //Hardware CPU Info
    const char* GetSDLVersion() { return SDL_GetRevision(); }
    const int GetCPUsCount() { return SDL_GetCPUCount(); }
    const float GetChacheSize() { return SDL_GetCPUCacheLineSize(); }
    const float GetSystemRAM() { return SDL_GetSystemRAM(); }
    
    //GPU Info
    const GLubyte* GetGPU() { return glGetString(GL_RENDER); }

  

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleRenderExercise* render_exercise = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleDebugDraw* debug_draw = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleTexture* textureModule = nullptr;


   
    std::vector<float> frameRate;
    std::vector<float> milliSeconds;
    int vectorPos;

    std::list<Module*> modules;

};

extern Application* App;
