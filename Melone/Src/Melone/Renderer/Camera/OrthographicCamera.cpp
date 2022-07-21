#include "mlpch.h"
#include "OrthographicCamera.h"

#include "Melone/Input/Input.h"
#include "Melone/Input/KeyCodes.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Melone
{
	OrthographicCamera::OrthographicCamera(const std::pair<float, float>& dimensions)
		:
		mViewportDimensions(dimensions)
	{
		const auto [width, height] = mViewportDimensions;
		mAspectRatio = width / height;

		RecalculateProjection();
		RecalculateView();
		RecalculateViewProjection();

		EventSystem::Subscribe(this, &OrthographicCamera::OnMouseScrolled);
		EventSystem::Subscribe(this, &OrthographicCamera::OnWindowResized);
		EventSystem::Subscribe(this, &OrthographicCamera::OnMouseMoved);
	}
	
	void OrthographicCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::A))
		{
			Translation(OrthographicCameraMovement::LEFT, mCameraTranslationSpeed * ts);
		}

		if (Input::IsKeyPressed(Key::D))
		{
			Translation(OrthographicCameraMovement::RIGHT, mCameraTranslationSpeed * ts);
		}

		if (Input::IsKeyPressed(Key::W))
		{
			Translation(OrthographicCameraMovement::UP, mCameraTranslationSpeed * ts);
		}

		if (Input::IsKeyPressed(Key::S))
		{
			Translation(OrthographicCameraMovement::DOWN, mCameraTranslationSpeed * ts);
		}

		if (Input::IsKeyPressed(Key::Q))
		{
			Rotate(OrthographicCameraRotation::COUNTERCLOCKWISE, mCameraRotationSpeed * ts);
		}

		if (Input::IsKeyPressed(Key::E))
		{
			Rotate(OrthographicCameraRotation::CLOCKWISE, mCameraRotationSpeed * ts);
		}
	}

	void OrthographicCamera::OnMouseScrolled(const MouseScrolledEvent& e)
	{
		mSize -= e.GetYOffset() * 0.25f;
		mSize = std::max(mSize, 0.25f);

		RecalculateProjection();
		RecalculateViewProjection();

		mCameraTranslationSpeed = mSize;
	}

	void OrthographicCamera::OnWindowResized(const WindowResizeEvent& e)
	{
		SetViewportDimensions(e.GetWinDimensions());
	}

	void OrthographicCamera::OnMouseMoved(const MouseMovedEvent& e)
	{
	}

	void OrthographicCamera::RecalculateProjection()
	{
		mProjectionMatrix = glm::ortho(-mAspectRatio * mSize, mAspectRatio * mSize, -mSize, mSize, mNear, mFar);
	}

	void OrthographicCamera::RecalculateView()
	{
		mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) *
			glm::rotate(glm::mat4(1.0f), glm::radians(mRotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

		mViewMatrix = glm::inverse(mViewMatrix);
	}

	void OrthographicCamera::RecalculateViewProjection()
	{
		mVPMatrix = mProjectionMatrix * mViewMatrix;
	}

	void OrthographicCamera::Translation(OrthographicCameraMovement direction, float velocity, bool recalculateMatrices)
	{
		switch (direction)
		{
			case Melone::OrthographicCameraMovement::UP:
				mPosition.y += velocity;
				break;
			case Melone::OrthographicCameraMovement::DOWN:
				mPosition.y -= velocity;
				break;
			case Melone::OrthographicCameraMovement::LEFT:
				mPosition.x -= velocity;
				break;
			case Melone::OrthographicCameraMovement::RIGHT:
				mPosition.x += velocity;
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

	void OrthographicCamera::Rotate(OrthographicCameraRotation direction, float velocity, bool recalculateMatrices)
	{
		switch (direction)
		{
			case Melone::OrthographicCameraRotation::CLOCKWISE:
				mRotationAngle -= velocity;
				break;
			case Melone::OrthographicCameraRotation::COUNTERCLOCKWISE:
				mRotationAngle += velocity;
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

	void OrthographicCamera::SetSize(float size)
	{
		mSize = size;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void OrthographicCamera::SetNearClip(float nearClip)
	{
		mNear = nearClip;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void OrthographicCamera::SetFarClip(float farClip)
	{
		mFar = farClip;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void OrthographicCamera::SetViewportDimensions(const std::pair<float, float>& dimensions)
	{
		mViewportDimensions = dimensions;

		const auto [width, height] = mViewportDimensions;
		mAspectRatio = width / height;

		RecalculateProjection();
		RecalculateViewProjection();
	}
}