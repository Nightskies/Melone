#include "mlpch.h"
#include "OrthographicCameraController.h"

#include "Melone/Core/Input.h"
#include "Melone/Core/KeyCodes.h"

namespace Melone
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:
		mAspectRatio(aspectRatio),
		mCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel),
		mRotation(rotation)
	{}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		if (Input::isKeyPressed(MELONE_KEY_A))
			m_CameraPosition.x -= mCameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(MELONE_KEY_D))
			m_CameraPosition.x += mCameraTranslationSpeed * ts;

		if (Input::isKeyPressed(MELONE_KEY_W))
			m_CameraPosition.y += mCameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(MELONE_KEY_S))
			m_CameraPosition.y -= mCameraTranslationSpeed * ts;

		if (mRotation)
		{
			if (Input::isKeyPressed(MELONE_KEY_Q))
				mCameraRotation += mCameraRotationSpeed * ts;
			if (Input::isKeyPressed(MELONE_KEY_E))
				mCameraRotation -= mCameraRotationSpeed * ts;

			mCamera.setRotation(mCameraRotation);
		}

		mCamera.setPosition(m_CameraPosition);

		mCameraTranslationSpeed = mZoomLevel;
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(MELONE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(MELONE_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		mZoomLevel -= e.getYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mCamera.setProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		auto dimensions = e.getWinDimensions();
		mAspectRatio = (float)dimensions.first / (float)dimensions.second; // width / height
		mCamera.setProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
		return false;
	}
}