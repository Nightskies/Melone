#pragma once
#include "Melone/Core/Window.h"

namespace Melone
{
	class GUI
	{
	public:
		GUI(const SPtr<Window>& window);
		~GUI();
	
		void Begin();
		void End();

		static SPtr<GUI> Create(const SPtr<Window>& window) { return std::make_shared<GUI>(window); }
	private:
		SPtr<Window> mWindow;
	};
}
