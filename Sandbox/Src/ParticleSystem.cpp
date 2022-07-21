#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

std::mt19937 Random::sRandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::sDistribution;

ParticleSystem::ParticleSystem(unsigned int maxParticles)
	:
	mPoolIndex(maxParticles - 1)
{
	mParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Melone::Timestep ts)
{
	for (auto& particle : mParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender(Melone::Camera& camera)
{	
	//Melone::Renderer2D::BeginScene(camera);

	//for (auto& particle : mParticlePool)
	//{
	//	if (!particle.Active)
	//		continue;

	//	// Fade away particles
	//	float life = particle.LifeRemaining / particle.LifeTime;
	//	glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
	//	//color.a = color.a * life;

	//	float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
	//	glm::vec3 position = { particle.Position.x, particle.Position.y, 0.2f };
	//	Melone::Renderer2D::DrawRotatedQuad(position, { size, size }, particle.Rotation, color);
	//}

	//Melone::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = mParticlePool[mPoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Rand() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Rand() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Rand() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Rand() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	mPoolIndex = --mPoolIndex % mParticlePool.size();
}