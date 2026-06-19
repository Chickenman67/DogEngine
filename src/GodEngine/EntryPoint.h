#pragma once


#ifdef GE_PLATFORM_WINDOWS

extern GodEngine::Application* GodEngine::CreateApplication();
int main(int argc, char** argv) {
	printf("GOD ENGINE\n");
	auto app = GodEngine::CreateApplication();
	app->Run();

	delete app;
}

#endif