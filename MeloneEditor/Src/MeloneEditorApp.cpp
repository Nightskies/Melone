#include "EditorLayer.h"

#include <Melone/Core/EntryPoint.h>

namespace Melone
{
	class MeloneEditor : public App
	{
	public:
		MeloneEditor(void)
			:
			App("Melone Editor")
		{
			pushLayer(new EditorLayer());
		}

		~MeloneEditor(void) = default;
	};

	App* createApp(void)
	{
		return new MeloneEditor;
	}
}