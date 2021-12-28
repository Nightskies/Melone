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
		~OpenGLVBO(void);

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		virtual void setData(const void* data, unsigned int size) override;

		virtual const VBOLayout& getLayout(void) const override { return mLayout; }
		virtual void setLayout(const VBOLayout& layout) override { mLayout = layout; }

	};

	class OpenGLIBO : public IBO
	{
	private:
		unsigned int mRendererID;
		unsigned int mCount;
	public:
		OpenGLIBO(unsigned int* indices, unsigned int count);
		~OpenGLIBO(void);

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		virtual unsigned int getCount(void) const override { return mCount; }
	};
}