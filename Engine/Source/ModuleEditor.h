#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
class ModuleEditor : public Module
{

	
	public:
		ModuleEditor();
		~ModuleEditor();

		bool Init();
		//update_status PreUpdate();
		update_status Update();
		//update_status PostUpdate();
		bool CleanUp();

		bool fullScreen, resizable;
		int width, height;
		const ImGuiIO* GetIO() { return io; };
		ImGuiTextBuffer logs;

	private:
		ImGuiIO *io;
		void* context;
	

};

