#pragma once

#include "Melone/Renderer/ArrayObj.h"

namespace Melone
{
	class OpenGLVAO : public VAO
	{
	private:
		std::vector<SPtr<VBO>> mVBOArr;
		SPtr<IBO> mIBO;
		unsigned int mRendererID;
		unsigned int mVertexBufferIndex = 0;
	public:
		OpenGLVAO();
		~OpenGLVAO();

		void Bind() const override;
		void Unbind() const override;

		void AddVBO(const SPtr<VBO>& VBO) override;
		void SetIBO(const SPtr<IBO>& IBO) override;

		const SPtr<IBO>& GetIBO() const override { return mIBO; }
	};
}

