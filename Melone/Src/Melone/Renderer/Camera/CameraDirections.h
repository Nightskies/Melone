#pragma once

namespace Melone
{
	enum class OrthographicCameraMovement
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum class PerspectiveCameraMovement
	{
		FORWARD,
		BACKWARD,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum class OrthographicCameraRotation
	{
		CLOCKWISE,
		COUNTERCLOCKWISE
	};

	enum class PerspectiveCameraRotation
	{
		YAW,
		PITCH,
		ROLL
	};
}