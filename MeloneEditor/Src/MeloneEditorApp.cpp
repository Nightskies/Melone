#include "EditorLayer.h"

#include <Melone/Core/EntryPoint.h>

namespace Melone
{
	class MeloneEditor : public Application
	{
	public:
		MeloneEditor()
			:
			Application("Melone Editor")
		{
			AddLayer(std::make_unique<EditorLayer>());
		}

		~MeloneEditor() = default;
	};

	Application* CreateApplication()
	{
		return new MeloneEditor();
	}
}