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
	bool CleanUp();
	KeyState GetKey(int id);

private:
	//const Uint8 *keyboard = NULL;
	KeyState* keyboard;
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	float2 mouse_motion;
	float2 mouse;

};