#pragma once
#include "CameraDirections.h"

#include "Melone/Core/Timestep.h"

#include "Melone/Events/EventSystem.h"

#include <glm/glm.hpp>

namespace Melone
{
	class PerspectiveCamera
	{
	private:
		glm::vec3 mPosition;

		glm::vec3 mFocalPoint = { 0.0f, 0.0f, 0.0f };

		float mYaw = 0.0f;
		float mPitch = 0.0f;

		float mFOV = 45.0f;
		float mNear = 0.01f;
		float mFar = 1000.0f;

		std::pair<float, float> mViewportDimensions;
		float mAspectRatio;
	protected:
		glm::mat4 mVPMatrix;
		glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 mViewMatrix = glm::mat4(1.0f);

		float mDistance = 10.0f;

		glm::vec2 mTranslationSpeed;
		float mRotationSpeed = 0.8f;
		float mZoomSpeed;
	public:
		PerspectiveCamera(const std::pair<float, float>& dimensions);

		virtual ~PerspectiveCamera() = default;
	protected:
		void OnUpdate(Timestep ts);

		float GetFOV() const { return mFOV; }
		float GetNearClip() const { return mNear; }
		float GetFarClip() const { return mFar; }

		void Translation(PerspectiveCameraMovement direction, float velocity, bool recalculateMatrices = true);
		void Rotate(PerspectiveCameraRotation direction, float velocity, bool recalculateMatrices = true);
		void Zoom(float yOffset);

		void SetFOV(float fov);
		void SetNearClip(float nearClip);
		void SetFarClip(float farClip);
		void SetViewportDimensions(const std::pair<float, float>& dimensions);

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
	private:
		glm::quat GetOrientation() const;

		void RecalculateViewProjection();
		void RecalculateProjection();
		void RecalculateView();

		void RecalculateZoomSpeed();
		void RecalculateTranslationSpeed();

		glm::vec3 CalculatePosition() const;

		void OnMouseScrolled(const MouseScrolledEvent& e);
		void OnWindowResized(const WindowResizeEvent& e);
		void OnMouseMoved(const MouseMovedEvent& e);
	};

}

