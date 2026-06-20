#pragma once


#ifdef GE_PLATFORM_WINDOWS

extern GodEngine::Application* GodEngine::CreateApplication();
int main(int argc, char** argv) {

	GodEngine::Log::Init();
	GE_CORE_WARN("initialized Log!");
	int a{ 5 };
	GE_INFO("HELLOW var = {0}!", a);
	
	auto app = GodEngine::CreateApplication();
	app->Run();

	delete app;
}

#endif