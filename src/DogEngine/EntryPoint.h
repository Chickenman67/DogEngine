#pragma once


#ifdef GE_PLATFORM_WINDOWS

extern DogEngine::Application* DogEngine::CreateApplication();
int main(int argc, char** argv) {

	DogEngine::Log::Init();
	GE_CORE_WARN("initialized Log!");
	int a{ 5 };
	GE_INFO("HELLOW var = {0}!", a);
	
	auto app = DogEngine::CreateApplication();
	app->Run();

	delete app;
}

#endif