#pragma once

#include "Melone/Renderer/Camera.h"

namespace Melone 
{
	class SceneCamera : public Camera
	{
	private:
		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f;
		float mOrthographicFar = 1.0f;

		float mAspectRatio = 0.0f;
	public:
		SceneCamera(void);
		~SceneCamera(void) = default;

		void setOrthographic(float size, float nearClip, float farClip);

		void setViewportSize(unsigned int width, unsigned int height);

		float getOrthographicSize(void) const { return mOrthographicSize; }
		void setOrthographicSize(float size) { mOrthographicSize = size; recalculateProjection(); }
	private:
		void recalculateProjection(void);
	};

}

