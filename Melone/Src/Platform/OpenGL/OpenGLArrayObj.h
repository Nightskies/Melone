#pragma once

#include "Melone/Renderer/ArrayObj.h"

namespace Melone
{
	class OpenGLVAO : public VAO
	{
	private:
		std::vector<std::shared_ptr<VBO>> mVBOArr;
		std::shared_ptr<IBO> mIBO;
		unsigned int mRendererID;
	public:
		OpenGLVAO(void);
		~OpenGLVAO(void);

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		virtual void addVBO(const std::shared_ptr<VBO>& VBO) override;
		virtual void setIBO(const std::shared_ptr<IBO>& IBO) override;
	};
}

