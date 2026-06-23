#include "gepch.h"
#include "GodEngine.h"

class Sandbox : public GodEngine::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}
};


GodEngine::Application* GodEngine::CreateApplication() {
	return new Sandbox();
}