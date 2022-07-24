#pragma once
#include "CameraDirections.h"

#include "Melone/Core/Timestep.h"

#include "Melone/Events/EventSystem.h"

#include <glm/glm.hpp>

namespace Melone
{
	class OrthographicCamera
	{
	private:
		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };

		float mSize = 1.0f;
		float mNear = -1.0f;
		float mFar = 1.0f;

		std::pair<float, float> mViewportDimensions;
		float mAspectRatio;
	protected:
		glm::mat4 mVPMatrix;
		glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 mViewMatrix = glm::mat4(1.0f);

		float mCameraTranslationSpeed = 5.0f;
		float mCameraRotationSpeed = 90.0f;

		float mRotationAngle = 0.0f;
	public:
		OrthographicCamera(const std::pair<float, float>& dimensions);

		virtual ~OrthographicCamera() = default;
	protected:

		float GetSize() const { return mSize; }
		float GetNearClip() const { return mNear; }
		float GetFarClip() const { return mFar; }

		void OnUpdate(Timestep ts);

		void Translation(OrthographicCameraMovement direction, float velocity, bool recalculateMatrices = true);
		void Rotate(OrthographicCameraRotation direction, float velocity, bool recalculateMatrices = true);

		void SetSize(float size);
		void SetNearClip(float nearClip);
		void SetFarClip(float farClip);
		void SetViewportDimensions(const std::pair<float, float>& dimensions);
	private:
		void RecalculateViewProjection();
		void RecalculateProjection();
		void RecalculateView();

		void OnMouseScrolled(const MouseScrolledEvent& e);
		void OnWindowResized(const WindowResizeEvent& e);
		void OnMouseMoved(const MouseMovedEvent& e);
	};
}


