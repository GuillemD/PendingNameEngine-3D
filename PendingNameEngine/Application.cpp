#include "Application.h"
#include "Globals.h"
#include "Configuration.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	ui = new ModuleUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(ui);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++) {
		delete(*item);
	}
}

bool Application::Init()
{
	bool ret = true;

	SetAppName("PendingNameEngine");
	SetVersion("v0.1.1");
	SetOrgName("UPC CITM");

	// Call Init() in all modules

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++) {
		ret = (*item)->Init();
	}

	//After we call all Init() we call all Starts
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++) {
		ret = (*item)->Start();
	}
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}


// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret==UPDATE_CONTINUE; item++) {
		ret = (*item)->PreUpdate(dt);
	}
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->Update(dt);
	}
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++) {
		ret = (*item)->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::OpenBrowser(const char * url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
void Application::OpenFile(const char * path)
{
	ShellExecute(NULL, NULL, path, NULL, NULL, SW_SHOW);
}

const char * Application::GetVersion() const
{
	return app_version.c_str();
}

void Application::SetVersion(const char * version)
{
	if (version != nullptr)
	{
		app_version = version;
	}
}

const char * Application::GetAppName() const
{
	return app_name.c_str();
}

void Application::SetAppName(const char * app)
{
	if (app != nullptr)
	{
		app_name = app;
	}
}

const char * Application::GetOrgName() const
{
	return org_name.c_str();
}

void Application::SetOrgName(const char * org)
{
	if (org != nullptr)
	{
		org_name = org;
	}
}
