#include "LayerStack.h"

#include <algorithm>

namespace Melone
{
	LayerStack::~LayerStack()
	{
		for (auto layer : mLayers)
			delete layer;
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		mLayers.insert(mLayers.begin() + mLayerInsertIndex, layer);
		mLayerInsertIndex++;
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), layer);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
			mLayerInsertIndex--;
		}
	}

	void LayerStack::pushOverlay(Layer* layer)
	{
		mLayers.push_back(layer);
	}

	void LayerStack::popOverlay(Layer* layer)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), layer);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
		}
	}
}