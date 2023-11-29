#include "ModuleInput.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "SDL.h"
#include <backends/imgui_impl_sdl2.h>
#include "Model.h"



#define MAX_KEYS 300


ModuleInput::ModuleInput()
{
	keyboard = new KeyState[MAX_KEYS];
}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{


	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;


}

update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;

	mouse_motion = { 0, 0 };
	mouse_wheel = { 0, 0 };
	
	//memset(windowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return UPDATE_STOP;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED ) //|| event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
				App->GetOpenGL()->WindowResized(event.window.data1, event.window.data2);
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel;
			mouse_motion.y = event.motion.yrel;
		
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
			break;
		case SDL_MOUSEWHEEL:
			mouse_wheel.x = event.wheel.x;
			mouse_wheel.y = event.wheel.y;
			break;
		case SDL_DROPFILE:
			LOG("FILE DROPPED: %s", event.drop.file);
			Model* model = App->GetModuleRenderExercise()->GetModel();

			model->Clear();
			model->Load(event.drop.file);
			break;
		}
	}

	ImGui_ImplSDL2_ProcessEvent(&event);

	//if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		//return UPDATE_STOP;




	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{
	SDL_Window *window = App->GetWindow()->window;
	float2 screenSize = App->GetWindow()->GetScreenSize();
	if (GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT || GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		if (mouse.x <= 0.0f) {
			SDL_WarpMouseInWindow(window, screenSize.x - 5.0f, mouse.y);
			mouse.x = screenSize.x - 5.0f;
			last_mouse_position.x = screenSize.x - 5.0f;
		}
		if (mouse.x >= screenSize.x - 1) {
			SDL_WarpMouseInWindow(window, 5.0f, mouse.y);
			last_mouse_position.x = 5.0f;
			mouse.x = 5.0f;
		}

		if (mouse.y <= 0.0f) {
			SDL_WarpMouseInWindow(window, mouse.x, screenSize.y - 5.0f);
			mouse.y = screenSize.y - 5.0f;
			last_mouse_position.y = screenSize.y - 5.0f;
		}
		if (mouse.y >= screenSize.y - 1) {
			SDL_WarpMouseInWindow(window, mouse.x, 5.0f);
			mouse.y = 5.0f;
			last_mouse_position.y = 5.0f;
		}

	}
	if (GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;
 
  
    return UPDATE_CONTINUE;
}


update_status ModuleInput::PostUpdate()
{
	last_mouse_position.x = mouse.x;
	last_mouse_position.y = mouse.y;

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

KeyState ModuleInput::GetKey(int id) const
{
	//if (!App->GetEditor()->GetIO()->WantCaptureKeyboard)
		return keyboard[id];
	//else
		//return KEY_IDLE;
}

KeyState ModuleInput::GetMouseButtonDown(int id) const
{
	if (!App->GetEditor()->GetIO()->WantCaptureMouse)
		return mouse_buttons[id - 1];
	else
		return KEY_IDLE;
}

const float2& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const float2& ModuleInput::GetLastMousePosition() const
{
	return last_mouse_position;
}

const float2& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}

const float2& ModuleInput::GetMouseWheel() const
{
	if (!App->GetEditor()->GetIO()->WantCaptureMouse)
		return mouse_wheel;
	else
		return float2(0.0f, 0.0f);
}