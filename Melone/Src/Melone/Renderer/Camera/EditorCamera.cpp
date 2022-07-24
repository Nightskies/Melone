#include "mlpch.h"
#include "EditorCamera.h"

#include "Melone/Input/Input.h"
#include "Melone/Input/KeyCodes.h"
#include "Melone/Input/MouseButtonCodes.h"

namespace Melone
{
	EditorCamera::EditorCamera(const std::pair<float, float>& dimensions)
		: 
		PerspectiveCamera(dimensions)
	{}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		
		if (Input::IsKeyPressed(Key::LeftAlt))
		{	
			auto [xPos, yPos] = Input::GetMousePosition();
			const glm::vec2& mouse = { xPos, yPos };
			glm::vec2 mousePositionOffset = (mouse - mMousePosition) * 0.003f;
			mMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
			{
				MouseTranslation(mousePositionOffset);
			}
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			{		
				MouseRotate(mousePositionOffset);
			}
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
			{
				MouseZoom(mousePositionOffset.y);
			}

		}

		// Moving camera by using the keyboard
		if (Input::IsKeyPressed(Key::Space))
		{
			PerspectiveCamera::OnUpdate(ts);
		}
	}

	void EditorCamera::MouseRotate(const glm::vec2& mousePositionOffset)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		Rotate(PerspectiveCameraRotation::YAW, yawSign * mousePositionOffset.x * mRotationSpeed, false);
		Rotate(PerspectiveCameraRotation::PITCH, mousePositionOffset.y * mRotationSpeed);
	}

	void EditorCamera::MouseTranslation(const glm::vec2& mousePositionOffset)
	{
		Translation(PerspectiveCameraMovement::RIGHT, mousePositionOffset.x * mTranslationSpeed.x * mDistance, false);
		Translation(PerspectiveCameraMovement::DOWN, mousePositionOffset.y * mTranslationSpeed.y * mDistance);
	}

	void EditorCamera::SetViewportDimensions(const std::pair<float, float>& dimensions)
	{
		PerspectiveCamera::SetViewportDimensions(dimensions);
	}

	void EditorCamera::MouseZoom(float yOffset)
	{
		Zoom(yOffset);
	}
}