#pragma once

#include "Core.h"

namespace Melone
{
	class App
	{
	public:
		App(void) = default;
		virtual ~App(void) = default;
	
		void run(void);
	};

	App* createApp(void);
}
