#pragma once
#include "Melone/Renderer/Texture.h"

#include <filesystem>

namespace Melone
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path mCurrentDirectory;

		SPtr<Texture2D> mDirectoryIcon;
		SPtr<Texture2D> mFileIcon;
	};
}