#pragma once

#include "PerspectiveCamera.h"

namespace Melone
{
	class EditorCamera : public PerspectiveCamera
	{
	private:
		glm::vec2 mMousePosition = { 0.0f, 0.0f };
	public:
		EditorCamera(const std::pair<float, float>& dimensions);

		~EditorCamera() = default;

		void OnUpdate(Timestep ts);

		void SetViewportDimensions(const std::pair<float, float>& dimensions);

		const glm::mat4& GetViewProjectionMatrix() const { return mVPMatrix; }
		const glm::mat4 GetViewMatrix() const { return mViewMatrix; }
		const glm::mat4 GetProjectionMatrix() const { return mProjectionMatrix; }
	private:
		void MouseRotate(const glm::vec2& mousePositionOffset);
		void MouseTranslation(const glm::vec2& mousePositionOffset);
		void MouseZoom(float yOffset);
	};
}

