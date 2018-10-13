#include "Application.h"
#include "ModuleUI.h"
#include "Globals.h"

ModuleUI::ModuleUI( bool start_enabled) : Module(start_enabled)
{
	
}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Start()
{
	LOG("Setting up UI");
	bool ret = true;
	ImGui_ImplSdl_Init(App->window->window);

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);
	
	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	
	/*if (ShowSphereCreatorPanel)ShowSphereCreator();
	if (ShowCubeCreatorPanel)ShowCubeCreator();
	if (ShowCapsuleCreatorPanel)ShowCapsuleCreator();*/
	CreateMainMenu();
	about.ShowElement();
	confg.ShowElement();
	insp.ShowElement();
	if (ShowTestWindow) ShowDemoWindow();
	if (ShowRNG) ShowRNGenerator();
	if (closeApp) return UPDATE_STOP;
	console.CreateConsole();

	return UPDATE_CONTINUE;

}
update_status ModuleUI::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}
void ModuleUI::DrawImGui() {
	
	
	
	App->renderer3D->DisableLight();
	bool auxwireframe = App->renderer3D->wireframe;
	bool auxcolor = App->renderer3D->color_material;
	
	if (App->renderer3D->wireframe || !auxcolor) {
		if (App->renderer3D->wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (!App->renderer3D->color_material) {
			glEnable(GL_COLOR_MATERIAL);
		}
		ImGui::Render();    

		if (auxwireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (!auxcolor) {
			glDisable(GL_COLOR_MATERIAL);
		}
	}
	else ImGui::Render();
}

bool ModuleUI::CleanUp()
{
	LOG("Cleaning UI");
	ImGui_ImplSdl_Shutdown();
	return true;
}


/*void ModuleUI::ShowCubeCreator() {
	if (ImGui::Begin("Cube Creator Panel")) {
		ImGui::Text("AABB Min Positions");
		ImGui::InputFloat("MinX Positon", &tmpvec.x, 0.05f, 0, 3);
		ImGui::InputFloat("MinY Position", &tmpvec.y, 0.05f, 0, 3);
		ImGui::InputFloat("MinZ Position", &tmpvec.z, 0.05f, 0, 3);

		ImGui::Text("AABB Max Positions");
		ImGui::InputFloat("MaxX Positon", &tmpvec2.x, 0.05f, 0, 3);
		ImGui::InputFloat("MaxY Position", &tmpvec2.y, 0.05f, 0, 3);
		ImGui::InputFloat("MaxZ Position", &tmpvec2.z, 0.05f, 0, 3);

		if (ImGui::Button("Create Cube")) {
			App->physics->CreateCube(tmpvec, tmpvec2);
		}
	}
	ImGui::End();
}

void ModuleUI::ShowSphereCreator()
{
	if (ImGui::Begin("Sphere Creator Panel")) {

		ImGui::InputFloat("Radius", &tmpfloat, 0.05f, 0, 3);
		ImGui::InputFloat("X Positon", &tmpvec.x, 0.05f, 0, 3);
		ImGui::InputFloat("Y Position", &tmpvec.y, 0.05f, 0, 3);
		ImGui::InputFloat("Z Position", &tmpvec.z, 0.05f, 0, 3);

		if (ImGui::Button("Create Sphere")) {
			App->physics->CreateSphere(tmpvec, tmpfloat);
		}
	}
	ImGui::End();
}

void ModuleUI::ShowCapsuleCreator()
{
	if (ImGui::Begin("Capsule Creator Panel")) {

		ImGui::Text("Radius");
		ImGui::InputFloat("Radius", &tmpfloat, 0.05f, 0, 3);

		ImGui::Text("LineSegment Begin Positions");
		ImGui::InputFloat("BeginX Positon", &tmpvec.x, 0.05f, 0, 3);
		ImGui::InputFloat("BeginY Position", &tmpvec.y, 0.05f, 0, 3);
		ImGui::InputFloat("BeginZ Position", &tmpvec.z, 0.05f, 0, 3);

		ImGui::Text("LineSegment End Positions");
		ImGui::InputFloat("EndX Positon", &tmpvec2.x, 0.05f, 0, 3);
		ImGui::InputFloat("EndY Position", &tmpvec2.y, 0.05f, 0, 3);
		ImGui::InputFloat("EndZ Position", &tmpvec2.z, 0.05f, 0, 3);


		if (ImGui::Button("Create Capsule")) {
			App->physics->CreateCapsule(tmpfloat, LineSegment(tmpvec,tmpvec2));
		}

	}
	ImGui::End();
}*/

void ModuleUI::ShowDemoWindow()
{
	ImGui::ShowTestWindow();
}
void ModuleUI::ShowRNGenerator()
{
	if (ImGui::Begin("Random Number Generator using PCG"))
	{
		ImGui::InputInt("Insert first int", &input_min);
		ImGui::InputInt("Insert second int", &input_max);

		if (input_min == input_max)
		{
			rand_bounded_int = input_max = input_min;
		}
		else
		{
			if (input_min > input_max)
			{
							
				if (ImGui::Button("Generate Random Between given ints"))
				{
					Swap<int>(input_min, input_max);
					rand_bounded_int = (" %i", (int)pcg32_boundedrand_r(&rng1, input_max) + input_min);
				}
				ImGui::Text("%i", rand_bounded_int);
			}
			else
			{
				if (ImGui::Button("Generate Random Between given ints"))
				{
					rand_bounded_int = (" %i", (int)pcg32_boundedrand_r(&rng1, input_max) + input_min);
				}
				ImGui::Text("%i", rand_bounded_int);
			}
		}

		if (ImGui::Button("Generate Float between 0.0 and 1.0"))
		{
			rand_float = ldexp(pcg32_random_r(&rng1), -32);
		}
		ImGui::Text("%f", rand_float);
		ImGui::End();
	}
}
void ModuleUI::CreateMainMenu()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {

			if (ImGui::MenuItem("Show Random Number Generator")) {
				ShowRNG = !ShowRNG;
			}
			if (ImGui::MenuItem("Save config"))
				App->SaveConfig();
			if (ImGui::MenuItem("Delete Meshes"))
				App->scene_loader->scene_objects.clear();
			if (ImGui::MenuItem("Close Application")) {
				closeApp = !closeApp;
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Options")) {
			if (ImGui::MenuItem("Show Configuration"))
			{
				if (confg.active) {}
				else
				{
					confg.active = true;
				}
				
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("ImGui Demo Window")) {
				ShowTestWindow = !ShowTestWindow;
			}
			if (ImGui::MenuItem("Engine documentation")) {
				App->OpenBrowser("https://github.com/GuillemD/PendingNameEngine-3D/wiki");
			}
			if (ImGui::MenuItem("Latest Release")) {
				App->OpenBrowser("https://github.com/GuillemD/PendingNameEngine-3D/releases");
			}
			if (ImGui::MenuItem("Report Bugs")) {
				App->OpenBrowser("https://github.com/GuillemD/PendingNameEngine-3D/issues");
			}
			if (ImGui::MenuItem("About")) {
				about.SwitchActive();
				
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

