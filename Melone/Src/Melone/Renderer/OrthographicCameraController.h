#pragma once
#include "OrthographicCamera.h"

#include "Melone/Core/Timestep.h"

#include "Melone/Events/WindowEvent.h"
#include "Melone/Events/MouseEvent.h"

namespace Melone
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController(void) = default;

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		OrthographicCamera& getCamera(void) { return mCamera; }
		const OrthographicCamera& getCamera(void) const { return mCamera; }

		float getZoomLevel(void) const { return mZoomLevel; }
		void setZoomLevel(float level) { mZoomLevel = level; }
	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);
	private:
		float mAspectRatio;
		float mZoomLevel = 1.0f;
		OrthographicCamera mCamera;

		bool mRotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float mCameraRotation = 0.0f;
		float mCameraTranslationSpeed = 5.0f;
		float mCameraRotationSpeed = 180.0f;
	};
}
