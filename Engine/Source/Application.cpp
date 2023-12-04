#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleRenderExercise.h"
#include "ModuleEditor.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"



using namespace std;

Application::Application()
{
	vectorPos = 0;
	frameRate.resize(100);
	milliSeconds.resize(100);
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(render_exercise = new ModuleRenderExercise());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(debug_draw = new ModuleDebugDraw());
	modules.push_back(textureModule = new ModuleTexture());



}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	Uint64 start = SDL_GetPerformanceCounter();
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	
	Uint64 end = SDL_GetPerformanceCounter();

	frameRate[vectorPos] = 1.0f / ((end - start) / (float)SDL_GetPerformanceFrequency());
	milliSeconds[vectorPos] = (end - start) / (float)SDL_GetPerformanceFrequency()*1000.0f;

	if (vectorPos != frameRate.size() - 1) {
		vectorPos++;
	}
	else {
		for (int i = 0; i < frameRate.size() -1; i++) {
			frameRate[i] = frameRate[i + 1];
			milliSeconds[i] = milliSeconds[i + 1];
		}
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}


void Application::RequestBrowser(const char* url) {
	ShellExecuteA(NULL, "open", url, NULL, NULL, 0);
}

