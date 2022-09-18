#include "mlpch.h"
#include "PerspectiveCamera.h"

#include "Melone/Input/Input.h"
#include "Melone/Input/KeyCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Melone
{
	PerspectiveCamera::PerspectiveCamera(const std::pair<float, float>& dimensions)
		:
		mViewportDimensions(dimensions)
	{
		const auto [width, height] = mViewportDimensions;
		mAspectRatio = width / height;

		RecalculateTranslationSpeed();
		RecalculateZoomSpeed();

		RecalculateProjection();
		RecalculateView();
		RecalculateViewProjection();

		EventSystem::Subscribe(this, &PerspectiveCamera::OnMouseScrolled);
		EventSystem::Subscribe(this, &PerspectiveCamera::OnWindowResized);
		EventSystem::Subscribe(this, &PerspectiveCamera::OnMouseMoved);
	}

	void PerspectiveCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::A))
		{
			Translation(PerspectiveCameraMovement::LEFT, mTranslationSpeed.x * mDistance * ts);
		}

		if (Input::IsKeyPressed(Key::D))
		{
			Translation(PerspectiveCameraMovement::RIGHT, mTranslationSpeed.x * mDistance * ts);
		}

		if (Input::IsKeyPressed(Key::W))
		{
			Translation(PerspectiveCameraMovement::UP, mTranslationSpeed.y * mDistance * ts);
		}

		if (Input::IsKeyPressed(Key::S))
		{
			Translation(PerspectiveCameraMovement::DOWN, mTranslationSpeed.y * mDistance * ts);
		}

		if (Input::IsKeyPressed(Key::Q))
		{
			Rotate(PerspectiveCameraRotation::YAW, mRotationSpeed * ts);
		}

		if (Input::IsKeyPressed(Key::E))
		{
			Rotate(PerspectiveCameraRotation::PITCH, mRotationSpeed * ts);
		}
	}

	void PerspectiveCamera::OnMouseScrolled(const MouseScrolledEvent& e)
	{
		float yOffset = e.GetYOffset() * 0.1f;
		Zoom(yOffset);

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void PerspectiveCamera::OnWindowResized(const WindowResizeEvent& e)
	{
		SetViewportDimensions(e.GetWinDimensions());
	}

	void PerspectiveCamera::OnMouseMoved(const MouseMovedEvent& e)
	{
	}

	void PerspectiveCamera::RecalculateProjection()
	{
		mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspectRatio, mNear, mFar);
	}

	void PerspectiveCamera::RecalculateView()
	{	
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		mPosition = CalculatePosition();

		glm::quat orientation = GetOrientation();
		mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
		mViewMatrix = glm::inverse(mViewMatrix);
	}

	void PerspectiveCamera::RecalculateViewProjection()
	{
		mVPMatrix = mProjectionMatrix * mViewMatrix;
	}

	void PerspectiveCamera::Translation(PerspectiveCameraMovement direction, float velocity, bool recalculateMatrices)
	{
		switch (direction)
		{
			case PerspectiveCameraMovement::FORWARD:
				break;
			case PerspectiveCameraMovement::BACKWARD:
				break;
			case PerspectiveCameraMovement::LEFT:
				mFocalPoint += GetRightDirection() * velocity;
				break;
			case PerspectiveCameraMovement::RIGHT:
				mFocalPoint += -GetRightDirection() * velocity;
				break;
			case PerspectiveCameraMovement::UP: // Forward
				mFocalPoint += -GetUpDirection() * velocity;
				break;
			case PerspectiveCameraMovement::DOWN:  // Backward
				mFocalPoint += GetUpDirection() * velocity;
				break;
			default:
				return;
		}

		if (recalculateMatrices)
		{
			RecalculateView();
			RecalculateViewProjection();
		}
	}

	void PerspectiveCamera::Rotate(PerspectiveCameraRotation direction, float velocity, bool recalculateMatrices)
	{
		switch (direction)
		{
			case PerspectiveCameraRotation::YAW:
				mYaw += velocity;
				break;
			case PerspectiveCameraRotation::PITCH:
				mPitch += velocity;
				break;
			default:
				return;
		}

		if (recalculateMatrices)
		{
			RecalculateView();
			RecalculateViewProjection();
		}
	}

	void PerspectiveCamera::Zoom(float yOffset)
	{
		if (mDistance -= yOffset * mZoomSpeed; mDistance < 1.0f)
		{
			mFocalPoint += GetForwardDirection();
			mDistance = 1.0f;
		}

		RecalculateZoomSpeed();
		
		RecalculateView();
		RecalculateViewProjection();
	}

	void PerspectiveCamera::SetFOV(float fov)
	{
		mFOV = fov;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void PerspectiveCamera::SetNearClip(float nearClip)
	{
		mNear = nearClip;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void PerspectiveCamera::SetFarClip(float farClip)
	{
		mFar = farClip;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void PerspectiveCamera::SetViewportDimensions(const std::pair<float, float>& dimensions)
	{
		mViewportDimensions = dimensions;

		const auto [width, height] = mViewportDimensions;
		mAspectRatio = width / height;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	glm::quat PerspectiveCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
	}

	glm::vec3 PerspectiveCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 PerspectiveCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 PerspectiveCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 PerspectiveCamera::CalculatePosition() const
	{
		return mFocalPoint - GetForwardDirection() * mDistance;
	}

	void PerspectiveCamera::RecalculateZoomSpeed()
	{
		float distance = mDistance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		mZoomSpeed = speed;
	}

	void PerspectiveCamera::RecalculateTranslationSpeed()
	{
		const auto [width, height] = mViewportDimensions;

		float x = std::min(width / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(height / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		mTranslationSpeed.x = xFactor;
		mTranslationSpeed.y = yFactor;
	}
}