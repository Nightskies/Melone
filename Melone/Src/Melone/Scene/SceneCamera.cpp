#include "mlpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Melone 
{
	SceneCamera::SceneCamera(void)
	{
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		mOrthographicSize = size;
		mOrthographicNear = nearClip;
		mOrthographicFar = farClip;
		recalculateProjection();
	}

	void SceneCamera::setViewportSize(unsigned int width, unsigned int height)
	{
		mAspectRatio = (float)width / (float)height;
		recalculateProjection();
	}

	void SceneCamera::recalculateProjection(void)
	{
		float orthoLeft = -mOrthographicSize * mAspectRatio * 0.5f;
		float orthoRight = mOrthographicSize * mAspectRatio * 0.5f;
		float orthoBottom = -mOrthographicSize * 0.5f;
		float orthoTop = mOrthographicSize * 0.5f;

		mProjection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
	}

}