#include "gepch.h"
#include "GodEngine.h"



class ExampleLayer : public GodEngine::Layer {
public:
	ExampleLayer()
	: Layer("Example")
	{}

	void OnUpdate() override {
		//GE_INFO("ExampleLayer::Update");

		if (GodEngine::Input::IsKeyPressed(GE_KEY_TAB))
			GE_INFO("Tab key is pressed");
}
	void OnEvent(GodEngine::Event& event) override
	{
		if (event.GetEventType() == GodEngine::EventType::KeyPressed) {
			GodEngine::KeyPressedEvent& e = (GodEngine::KeyPressedEvent&)event;
			GE_TRACE("{0}", (char)e.GetKeyCode());
		}
		
	}
};

class Sandbox : public GodEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverLay(new GodEngine::ImGuiLayer());
	}
	~Sandbox() {

	}
};


GodEngine::Application* GodEngine::CreateApplication() {
	return new Sandbox();
}