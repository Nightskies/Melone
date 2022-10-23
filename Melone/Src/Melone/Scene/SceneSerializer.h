#pragma once
#include "Scene.h"

namespace Melone
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const SPtr<Scene>& scene);
		~SceneSerializer() = default;

		void Serialize(std::string&& filePath);
		void SerializeRuntime(const std::string& filePath);

		bool Deserialize(std::string&& filePath);
		bool DeserializeRuntime(const std::string& filePath);
	private:
		SPtr<Scene> mScene;
	};
}