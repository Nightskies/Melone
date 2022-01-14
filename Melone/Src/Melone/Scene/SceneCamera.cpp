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
		mProjectionType = ProjectionType::Orthographic;
		mOrthographicSize = size;
		mOrthographicNear = nearClip;
		mOrthographicFar = farClip;
		recalculateProjection();
	}

	void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip)
	{
		mProjectionType = ProjectionType::Perspective;
		mPerspectiveFOV = verticalFOV;
		mPerspectiveNear = nearClip;
		mPerspectiveFar = farClip;
		recalculateProjection();
	}

	void SceneCamera::setViewportSize(unsigned int width, unsigned int height)
	{
		mAspectRatio = (float)width / (float)height;
		recalculateProjection();
	}

	void SceneCamera::recalculateProjection(void)
	{
		if (mProjectionType == ProjectionType::Perspective)
		{
			mProjection = glm::perspective(mPerspectiveFOV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);
		}
		else
		{
			float orthoLeft = -mOrthographicSize * mAspectRatio * 0.5f;
			float orthoRight = mOrthographicSize * mAspectRatio * 0.5f;
			float orthoBottom = -mOrthographicSize * 0.5f;
			float orthoTop = mOrthographicSize * 0.5f;

			mProjection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
		}
	}

}