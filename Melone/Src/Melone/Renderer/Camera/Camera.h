#pragma once
#include <glm/glm.hpp>

namespace Melone
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& proj)
			:
			mProjectionMatrix(proj)
		{}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return mProjectionMatrix; }
	protected:
		glm::mat4 mProjectionMatrix;
	};
}