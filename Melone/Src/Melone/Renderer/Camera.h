#pragma once

#include <glm/glm.hpp>

namespace Melone 
{
	class Camera
	{
	private:
		glm::mat4 mProjection;
	public:
		Camera(const glm::mat4& projection)
			: 
			mProjection(projection) 
		{}

		const glm::mat4& getProjection(void) const { return mProjection; }
	};

}