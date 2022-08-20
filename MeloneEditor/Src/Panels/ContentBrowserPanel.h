#pragma once
#include "Melone/Renderer/Texture.h"

#include <filesystem>

namespace Melone
{
	class ContentBrowserPanel
	{
	private:
		std::filesystem::path mCurrentDirectory;

		SPtr<Texture2D> mDirectoryIcon;
		SPtr<Texture2D> mFileIcon;
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	};
}


