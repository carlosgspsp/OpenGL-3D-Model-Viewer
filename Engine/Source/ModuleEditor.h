#pragma once
#include "Module.h"
#include "Globals.h"


struct ImGuiIO;
class ImGuiTextBuffer;

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


		bool fullScreen = false, resizable = true;
		int width = 0, height = 0;
		const ImGuiIO* GetIO() const { return io; };
		

	private:
		ImGuiIO *io = nullptr;
		void* context = nullptr;
		ImGuiTextBuffer* logs = nullptr;
	

};

