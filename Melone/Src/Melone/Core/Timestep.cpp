#include "mlpch.h"
#include "Timestep.h"

#include <glfw/glfw3.h>

namespace Melone
{
	Timestep::Timestep(float timestep)
		:
		mTimestep(timestep)
	{}

	float Timestep::GetTimestep()
	{
		static float lastFrameTime = 0.0f;

		float time = static_cast<float>(glfwGetTime());
		mTimestep = time - lastFrameTime;
		lastFrameTime = time;

		return mTimestep;
	}
}