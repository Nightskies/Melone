#pragma once

#include "Melone/Core/Window.h"

namespace Melone
{
	class GUI
	{
	private:
		const Window& mWindow;
	public:
		GUI(const Window& window);
		~GUI();
	public:	
		void Begin();
		void End();

		static SPtr<GUI> Create(const Window& window) { return std::make_shared<GUI>(window); }
	};
}
