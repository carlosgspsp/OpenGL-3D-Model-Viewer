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

		void AddLog(char str[]);


		bool fullScreen, resizable;
		int width, height;
		const ImGuiIO* GetIO() const { return io; };
		

	private:
		ImGuiIO *io;
		void* context;
		ImGuiTextBuffer logs;
	

};

