#pragma once

#include "Melone/Renderer/Camera.h"

namespace Melone 
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	private:
		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f;
		float mOrthographicFar = 1.0f;

		float mAspectRatio = 0.0f;

		ProjectionType mProjectionType = ProjectionType::Orthographic;

		float mPerspectiveFOV = glm::radians(45.0f);
		float mPerspectiveNear = 0.01f, mPerspectiveFar = 1000.0f;
	public:
		SceneCamera(void);
		~SceneCamera(void) = default;

		void setOrthographic(float size, float nearClip, float farClip);
		void setPerspective(float verticalFOV, float nearClip, float farClip);

		float getPerspectiveVerticalFOV(void) const { return mPerspectiveFOV; }
		void setPerspectiveVerticalFOV(float verticalFov) { mPerspectiveFOV = verticalFov; recalculateProjection(); }
		float getPerspectiveNearClip(void) const { return mPerspectiveNear; }
		void setPerspectiveNearClip(float nearClip) { mPerspectiveNear = nearClip; recalculateProjection(); }
		float getPerspectiveFarClip(void) const { return mPerspectiveFar; }
		void setPerspectiveFarClip(float farClip) { mPerspectiveFar = farClip; recalculateProjection(); }

		void setViewportSize(unsigned int width, unsigned int height);

		float getOrthographicSize(void) const { return mOrthographicSize; }
		void setOrthographicSize(float size) { mOrthographicSize = size; recalculateProjection(); }

		float getOrthographicNearClip(void) const { return mOrthographicNear; }
		void setOrthographicNearClip(float nearClip) { mOrthographicNear = nearClip; recalculateProjection(); }
		float getOrthographicFarClip(void) const { return mOrthographicFar; }
		void setOrthographicFarClip(float farClip) { mOrthographicFar = farClip; recalculateProjection(); }

		ProjectionType getProjectionType(void) const { return mProjectionType; }
		void setProjectionType(ProjectionType type) { mProjectionType = type; recalculateProjection(); }
	private:
		void recalculateProjection(void);
	};

}

