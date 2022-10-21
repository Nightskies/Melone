#pragma once

#include "Melone/Core/Window.h"

namespace Melone
{
	class GUI
	{
	private:
		SPtr<Window> mWindow;
	public:
		GUI(const SPtr<Window>& window);
		~GUI();
	public:	
		void Begin();
		void End();

		static SPtr<GUI> Create(const SPtr<Window>& window) { return std::make_shared<GUI>(window); }
	};
}
