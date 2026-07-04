#include "gepch.h"
#include "LayerStack.h"

namespace DogEngine {
	LayerStack::LayerStack() {

	}
	LayerStack::~LayerStack() {
	}

	void LayerStack::PushLayer(Scope<Layer> layer) {
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
		m_LayerInsertIndex++;
	}
	void LayerStack::PushOverlay(Scope<Layer> overlay) {
		m_Layers.emplace_back(std::move(overlay));
	}
	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find_if(m_Layers.begin(), m_Layers.end(), 
			[layer](const Scope<Layer>& owned) { return owned.get() == layer; });
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}
	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find_if(m_Layers.begin(), m_Layers.end(),
			[overlay](const Scope<Layer>& owned) { return owned.get() == overlay; });
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}
