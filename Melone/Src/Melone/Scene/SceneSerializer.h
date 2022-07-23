#pragma once

#include "Scene.h"

namespace Melone
{
	class SceneSerializer
	{
	private:
		SPtr<Scene> mScene;
	public:
		SceneSerializer(const SPtr<Scene>& scene);

		void Serialize(std::string&& filePath);
		void SerializeRuntime(const std::string& filePath);

		bool Deserialize(std::string&& filePath);
		bool DeserializeRuntime(const std::string& filePath);
	};
}

