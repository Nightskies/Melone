#pragma once
#include "Melone/Renderer/BufferObj.h"

namespace Melone
{
	class OpenGLVBO : public VBO
	{
	private:
		unsigned int mRendererID;
		VBOLayout mLayout;
	public:
		OpenGLVBO(float* vertices, unsigned int size);
		OpenGLVBO(unsigned int size);
		~OpenGLVBO();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, unsigned int size) override;

		const VBOLayout& GetLayout() const override { return mLayout; }
		void SetLayout(VBOLayout&& layout) override { mLayout = layout; }

	};

	class OpenGLIBO : public IBO
	{
	private:
		unsigned int mRendererID;
	public:
		OpenGLIBO(unsigned int* indices, unsigned int count);
		OpenGLIBO(unsigned int count);
		~OpenGLIBO();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, unsigned int count) override;
	};
}