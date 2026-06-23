#include "gepch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
namespace GodEngine {
	Application::Application()
	{

	}
	Application	::~Application() {

	}
	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			GE_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput)) {
			GE_TRACE(e.ToString());
		}

		
		while (true) {

	}
	
	}
}
