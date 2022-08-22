#pragma once

#include "entt.hpp"

#include "Melone/Core/Timestep.h"
#include "Melone/Renderer/Camera/EditorCamera.h"

class b2World;

namespace Melone 
{
	class Scene
	{
	private:
		entt::registry mRegistry;
		
		unsigned int mViewportWidth = 0;
		unsigned int mViewportHeight = 0;

		b2World* mPhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(std::string&& name);
		void DestroyEntity(Entity entity);

		void OnUpdateEditorCamera(Timestep ts, const EditorCamera& camera);
		void OnViewportResize(unsigned int width, unsigned int height);
		void OnUpdate(Timestep ts);

		void OnRuntimeStart();
		void OnRuntimeStop();

		Entity GetPrimaryCameraEntity();

		static SPtr<Scene> Create();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	};
}

