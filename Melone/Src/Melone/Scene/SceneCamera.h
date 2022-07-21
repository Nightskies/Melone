#pragma once

#include "Melone/Renderer/Camera/Camera.h"

namespace Melone 
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};
	private:
		ProjectionType mProjectionType = ProjectionType::Orthographic;

		float mPerspectiveFOV = 45.0f;
		float mPerspectiveNear = 0.01f;
		float mPerspectiveFar = 1000.0f;

		float mOrthographicSize = 1.0f;
		float mOrthographicNear = -1.0f;
		float mOrthographicFar = 1.0f;

		float mAspectRatio = 0.0f;
	public:
		SceneCamera();
		~SceneCamera() = default;

		void SetPerspective(float FOV, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportDimensions(const std::pair<float, float>& dimensions);

		float GetPerspectiveFOV() const { return mPerspectiveFOV; }
		void SetPerspectiveFOV(float verticalFov) { mPerspectiveFOV = verticalFov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return mPerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { mPerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return mPerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { mPerspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return mOrthographicSize; }
		void SetOrthographicSize(float size) { mOrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return mOrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { mOrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return mOrthographicFar; }
		void SetOrthographicFarClip(float farClip) { mOrthographicFar = farClip; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return mProjectionType; }
		void SetProjectionType(ProjectionType type) { mProjectionType = type; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	};
}

