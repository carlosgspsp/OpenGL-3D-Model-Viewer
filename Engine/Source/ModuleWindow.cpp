#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL.h"


ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);

		screenSize.x = DM.w / 2.0f;
		screenSize.y = DM.h / 2.0f;
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ; // ||SDL_WINDOW_INPUT_GRABBED este para que no se salga el raton

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.x, screenSize.y, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(window, 50, 50);
	return ret;
}

update_status ModuleWindow::Update() {
	

	return UPDATE_CONTINUE;
	}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}


void ModuleWindow::SetFullScreen(bool fullScreen) {

	if (fullScreen)
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
	SDL_SetWindowFullscreen(window, 0);

}


void ModuleWindow::SetResizable(bool resizable) {
	if (resizable)
	SDL_SetWindowResizable(window, SDL_TRUE);
	else
	SDL_SetWindowResizable(window, SDL_FALSE);
}

void ModuleWindow::SetScreenSize(float2 screenSize) {
	this->screenSize.x = screenSize.x;
	this->screenSize.y = screenSize.y;
}
