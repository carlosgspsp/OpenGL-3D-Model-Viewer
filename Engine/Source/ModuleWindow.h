#pragma once
#include "Module.h"
#include <Math/float2.h>

class Application;
class SDL_Window;
class SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	update_status Update();


	//The window we'll be rendering to
	SDL_Window* window = NULL;


	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;


	const float2 GetScreenSize() const { return screenSize; }
	void SetScreenSize(float2 screenSize);
	void SetFullScreen(bool fullScreen);
	void SetResizable(bool resizable);
private:
	float2 screenSize;

};

