#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h";

typedef unsigned __int8 Uint8;

#define NUM_MOUSE_BUTTONS 5

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};


class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	KeyState GetKey(int id) const;
	KeyState GetMouseButtonDown(int id) const;
	const float2& GetMouseMotion() const;
	const float2& GetMousePosition() const;
	const float2& GetLastMousePosition() const;
	const float2& GetMouseWheel() const;

private:
	//const Uint8 *keyboard = NULL;
	KeyState* keyboard;
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	float2 mouse_motion, mouse_wheel;
	float2 mouse, last_mouse_position;

};