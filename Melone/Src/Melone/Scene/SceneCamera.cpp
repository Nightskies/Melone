#include "mlpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Melone 
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float FOV, float nearClip, float farClip)
	{
		mProjectionType = ProjectionType::Perspective;
		mPerspectiveFOV = FOV;
		mPerspectiveNear = nearClip;
		mPerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		mProjectionType = ProjectionType::Orthographic;
		mOrthographicSize = size;
		mOrthographicNear = nearClip;
		mOrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportDimensions(const std::pair<float, float>& dimensions)
	{
		auto [width, height] = dimensions;
		mAspectRatio = width / height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (mProjectionType == ProjectionType::Perspective)
		{
			mProjectionMatrix = glm::perspective(glm::radians(mPerspectiveFOV), mAspectRatio, mPerspectiveNear, mPerspectiveFar);
		}
		else
		{
			mProjectionMatrix = glm::ortho(-mAspectRatio * mOrthographicSize, 
				mAspectRatio * mOrthographicSize,
				-mOrthographicSize, 
				mOrthographicSize, 
				mOrthographicNear, 
				mOrthographicFar);
		}

	}
}