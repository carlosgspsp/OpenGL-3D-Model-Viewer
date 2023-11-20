#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"
#include <MathGeoLib.h>

class Application;

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


	float2 GetScreenSize();
	void SetScreenSize(float2 screenSize);


private:
	float2 screenSize;

};

#endif // __ModuleWindow_H__