#pragma once
#include "DogEngine/Layer.h"
#include "DogEngine/Events/KeyEvent.h"
#include "DogEngine/Events/MouseEvent.h"
#include "DogEngine/Events/ApplicationEvent.h"
namespace DogEngine {
	class DOGENGINE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();	

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		
	private:
		float m_Time{ 0.0f };

	};
}
