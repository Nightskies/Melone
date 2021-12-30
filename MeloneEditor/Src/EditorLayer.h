#pragma once

#include <Melone.h>

namespace Melone 
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(void);
		~EditorLayer(void) = default;

		virtual void onAttach(void) override;
		virtual void onDetach(void) override;

		void onUpdate(Timestep ts) override;
		virtual void onImGuiRender(void) override;
		void onEvent(Event& e) override;
	private:
		OrthographicCameraController mCameraController;

		std::shared_ptr<VAO> mSquareVAO;
		std::shared_ptr<Shader> mFlatColorShader;
		std::shared_ptr<Framebuffer> mFramebuffer;

		std::shared_ptr<Texture2D> mCheckerboardTexture;

		glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};
}