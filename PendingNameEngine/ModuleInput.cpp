#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"


#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(rapidjson::Document& document)
{
	
	LOG("Cleaning camera");
	CONSOLELOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		CONSOLELOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_DROPFILE:
			{
				std::string file_path = e.drop.file;

				std::string::size_type idx;
				
				idx = file_path.rfind('.');
				if (idx != std::string::npos)
				{
					std::string extension = file_path.substr(idx + 1);

					if (extension == "fbx" || extension == "FBX") {
						std::string mesh_file_path = file_path;
						App->scene_loader->scene_objects.clear();
						App->importer->Import(mesh_file_path.c_str());
						SDL_free(e.drop.file);
					}
					else if (extension == "png" || extension == "PNG" || extension == "dds" || extension == "DDS") {
						std::string tex_file_path = file_path;
						
						App->texture->LoadTexFromPath(tex_file_path.c_str());
						App->texture->current = tex_file_path.c_str();
							
						SDL_free(e.drop.file);
					}
					else {
						CONSOLELOG("Unrecognised file format.")
					}
				}
			}
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	CONSOLELOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::ShowInputConfiguration()
{
	ImGui::TextColored(ImVec4(1, 1, 1, 1), "Mouse X: %d", App->input->mouse_x);
	ImGui::TextColored(ImVec4(1, 1, 1, 1), "Mouse Y: %d", App->input->mouse_y);

	ImGui::TextColored(ImVec4(1, 1, 1, 1), "Mouse motion on X axis: %d", App->input->GetMouseXMotion());
	ImGui::TextColored(ImVec4(1, 1, 1, 1), "Mouse motion on Y axis: %d", App->input->GetMouseYMotion());

}