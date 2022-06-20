#include "LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (auto& layer : m_Layers) {
		layer->OnDetach();
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayersInstertIndex, layer);
	m_LayersInstertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
	const auto& it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayersInstertIndex, layer);
	if (it != m_Layers.begin() + m_LayersInstertIndex) {
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayersInstertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay)
{
	const auto& it = std::find(m_Layers.begin() + m_LayersInstertIndex, m_Layers.end(), overlay);
	if (it != m_Layers.end()) {
		overlay->OnDetach();
		m_Layers.erase(it);
	}
}
