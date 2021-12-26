#pragma once

#include <glm/glm.hpp>

namespace Melone
{
	class OrthographicCamera
	{
	private:
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		glm::mat4 mViewProjectionMatrix;

		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
		float mRotation = 0.0f;
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera(void) = default;
	public:
		const glm::vec3& getPos(void) const { return mPosition; }
		float getRotation(void) const { return mRotation; }

		void setPosition(const glm::vec3& pos) { mPosition = pos; updateViewMatrix(); }
		void setRotation(float rotation) { mRotation = rotation; updateViewMatrix(); }
		void setProjection(float left, float right, float bottom, float top);

		const glm::mat4& getProjectionMatrix(void) const { return mProjectionMatrix; }
		const glm::mat4& getViewMatrix(void) const { return mViewMatrix; }
		const glm::mat4& getViewProjectionMatrix(void) const { return mViewProjectionMatrix; }
	private:
		// Recalculate view matrix
		void updateViewMatrix(void);
	};
}
