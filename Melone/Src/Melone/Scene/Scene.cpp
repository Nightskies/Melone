#include "mlpch.h"
#include "Scene.h"

#include "Components.h"
#include "Melone/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Melone 
{
	Scene::Scene(void)
	{
	}

	Scene::~Scene(void)
	{
	}

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = { mRegistry.create(), this };
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::onUpdate(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		{
			auto group = mRegistry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::beginScene(mainCamera->getProjection(), *cameraTransform);

			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::drawQuad(transform, sprite.Color);
			}

			Renderer2D::endScene();
		}
	}

}