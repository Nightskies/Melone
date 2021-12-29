#include "mlpch.h"
#include "OrthographicCameraController.h"

#include "Melone/Core/Input.h"
#include "Melone/Core/KeyCodes.h"

namespace Melone
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:
		mAspectRatio(aspectRatio),
		mBounds({ -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel }),
		mCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel),
		mRotation(rotation)
	{}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		if (Input::isKeyPressed(MELONE_KEY_A))
			mCameraPosition.x -= mCameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(MELONE_KEY_D))
			mCameraPosition.x += mCameraTranslationSpeed * ts;

		if (Input::isKeyPressed(MELONE_KEY_W))
			mCameraPosition.y += mCameraTranslationSpeed * ts;
		else if (Input::isKeyPressed(MELONE_KEY_S))
			mCameraPosition.y -= mCameraTranslationSpeed * ts;

		if (mRotation)
		{
			if (Input::isKeyPressed(MELONE_KEY_Q))
				mCameraRotation += mCameraRotationSpeed * ts;
			if (Input::isKeyPressed(MELONE_KEY_E))
				mCameraRotation -= mCameraRotationSpeed * ts;

			mCamera.setRotation(mCameraRotation);
		}

		mCamera.setPosition(mCameraPosition);

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
		mBounds = { -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel };
		mCamera.setProjection(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		auto [w, h] = e.getWinDimensions();
		mAspectRatio = (float)w / (float)h;
		mCamera.setProjection(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top);
		return false;
	}
}