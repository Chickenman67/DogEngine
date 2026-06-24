#include "gepch.h"
#include "GodEngine.h"

class ExampleLayer : public GodEngine::Layer {
public:
	ExampleLayer()
	: Layer("Example")
	{}

	void OnUpdate() override {
		GE_INFO("ExampleLayer::Update");
}
	void OnEvent(GodEngine::Event& event) override
	{
		GE_TRACE("{0}", event);
	}
};

class Sandbox : public GodEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};


GodEngine::Application* GodEngine::CreateApplication() {
	return new Sandbox();
}