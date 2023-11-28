#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleEditor.h"
#include ".\backends\imgui_impl_sdl2.h"
#include ".\backends\imgui_impl_opengl3.h"

ModuleEditor::ModuleEditor() {
	context = nullptr;
	fullScreen = false;
	resizable = true;
	
}
ModuleEditor::~ModuleEditor() {

}

bool ModuleEditor::Init()
{
	
	ImGui::CreateContext();
	
	io = &(ImGui::GetIO()); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	//io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	   // Enable ImGui outside Viewports
	

	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->context);
	ImGui_ImplOpenGL3_Init("#version 460");


	width = App->GetWindow()->GetScreenSize().x;
	height = App->GetWindow()->GetScreenSize().y;


	return true;
}

update_status ModuleEditor::Update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
	ImGui::NewFrame();

	
	
	static bool demo = true;
	if (demo) {
		ImGui::ShowDemoWindow(&demo);
	}


	//ImGui::Begin("DockSpace");

	/*ImGuiID dockspace_id = ImGui::GetID("")
	ImGui::DockSpace();
	ImGui::End();
	*/
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);


	bool about_popup = false;
	static bool mainMenu_window = true; 
	if (mainMenu_window) {
		ImGui::Begin("Main Menu", &mainMenu_window, ImGuiWindowFlags_MenuBar);
		
		if (ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("General"))
			{
				if (ImGui::MenuItem("GitHub"))
				{
					App->RequestBrowser("https://github.com/carlosgspsp/GameEngine");
				}
				if (ImGui::MenuItem("About"))
				{
		
					about_popup = true;
					
					
					
				}

				

				if (ImGui::MenuItem("Quit"))
				{
					return UPDATE_STOP;
				}
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		if (about_popup) {
			ImGui::OpenPopup("About");
		}
		if (ImGui::BeginPopup("About")) {
			
			ImGui::Text("Noob Engine");
			ImGui::Text("Description: Game Engine created for UPC project");
			ImGui::Text("Author: Carlos Garcia Segura");
			ImGui::Text("Libraries: ");
			ImGui::Text("License: MIT");
			
			if (ImGui::Button("CLOSE", ImVec2(200, 0))) {
				ImGui::CloseCurrentPopup();
				about_popup = false;
			}

			ImGui::EndPopup();
		}

		if (ImGui::CollapsingHeader("Configuration"))
		{
			if (ImGui::TreeNode("Options"))
			{
				if (ImGui::CollapsingHeader("Application"))
				{
					char title[25];
					sprintf_s(title, 25, "Framerate %.1f", App->GetFrameRate()->at(App->GetFrameRate()->size() - 1));
					ImGui::PlotHistogram("##FrameRate", &(App->GetFrameRate()->at(0)), App->GetFrameRate()->size(), 0, title, 0.0f, 100.0f, ImVec2(310.0f, 100.0f));

					sprintf_s(title, 25, "Milliseconds %.1f", App->GetMilliseconds()->at(App->GetMilliseconds()->size() - 1));
					ImGui::PlotHistogram("##Milliseconds", &(App->GetMilliseconds()->at(0)), App->GetMilliseconds()->size(), 0, title, 0.0f, 40.0f, ImVec2(310.0f, 100.0f));
				}

				if (ImGui::CollapsingHeader("Window"))
				{
					
					if (ImGui::SliderInt("Width", &width, 1, 3840 )) {
						SDL_SetWindowSize(App->GetWindow()->window, width, height);
					}
					if (ImGui::SliderInt("Height", &height, 1, 2160)) {
						SDL_SetWindowSize(App->GetWindow()->window, width, height);
					}


					if (ImGui::Checkbox("Fullscreen", &fullScreen)) {
						App->GetWindow()->SetFullScreen(fullScreen);
						
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Resizable", &resizable)) {
						App->GetWindow()->SetResizable(resizable);
						
					}
				}

				ImGui::TreePop();
			}	

		}


		if (ImGui::CollapsingHeader("Properties"))
		{
			if (ImGui::TreeNode("Geometry"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Texture"))
			{
				ImGui::Text("File: %s", App->GetSDLVersion());
				ImGui::Text("Width: %s", App->GetSDLVersion());
				ImGui::SameLine();
				ImGui::Text("Height: %s", App->GetSDLVersion());
				ImGui::Text("Format: %s", App->GetSDLVersion());
				ImGui::TreePop();
			}
		}


		if (ImGui::CollapsingHeader("Hardware"))
		{

			SDL_version version; 
			SDL_VERSION(&version);

			GLfloat total_vram, available_vram, usage;
			glGetFloatv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_vram);
			glGetFloatv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_vram);
			
			
			usage = (total_vram - available_vram) / total_vram;

			ImGui::Text("SDL Version: %u.%u.%u", version.major, version.minor, version.patch);
			ImGui::Separator();
			ImGui::Text("CPUs: %i (Cache: %.1fkb)" ,App->GetCPUsCount(), App->GetChacheSize());
			ImGui::Text("System RAM: %.1fGB ", App->GetSystemRAM());
			ImGui::Separator();
			ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
			ImGui::Text("GPU Brand: %s" , glGetString(GL_RENDERER));
			ImGui::Text("Driver Version: %s", glGetString(GL_VERSION));
			ImGui::Text("VRAM Budget: %.1fMB", (total_vram/1024.0f));
			ImGui::Text("VRAM Usage: %.2f%%", usage*100.0f);
			ImGui::Text("VRAM Available: %.2fMB" ,available_vram/1024);
		}


		ImGui::End();

	}
	
	static bool console_window = true;
	if (console_window) {
		static bool autoscroll = true;
		ImGui::Begin("Console", &console_window);

		ImGui::Checkbox("AutoScroll", &autoscroll);
		ImGui::Separator();
		ImGui::BeginChild("Console2");

		ImGui::TextUnformatted(logs.begin(), logs.end());
		
		if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		
		
		
		ImGui::End();
	}



	
	

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }


	return UPDATE_CONTINUE;

}


bool ModuleEditor::CleanUp() {
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}