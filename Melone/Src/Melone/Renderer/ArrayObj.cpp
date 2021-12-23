#include "mlpch.h"
#include "ArrayObj.h"

#include "Platform/OpenGL/OpenGLArrayObj.h"

namespace Melone
{
	std::shared_ptr<VAO> VAO::create(void)
	{
		return std::make_shared<OpenGLVAO>();
	}
}