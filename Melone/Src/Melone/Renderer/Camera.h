#pragma once

#include <glm/glm.hpp>

namespace Melone 
{
	class Camera
	{
	protected:
		glm::mat4 mProjection = glm::mat4(1.0f);;
	public:
		Camera(void) = default;
		Camera(const glm::mat4& projection)
			: 
			mProjection(projection) 
		{}

		virtual ~Camera(void) = default;

		const glm::mat4& getProjection(void) const { return mProjection; }
	};

}