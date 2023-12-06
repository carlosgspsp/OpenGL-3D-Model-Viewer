#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math/float2.h"

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
		float cameraSpeed = 0;
		float2 panSensitivity = float2::zero;
		float2 mouseSensitivity = float2::zero;
		float zoomSensitivity = 0;

	private:
		ImGuiIO *io = nullptr;
		void* context = nullptr;
		ImGuiTextBuffer* logs = nullptr;
	

};

