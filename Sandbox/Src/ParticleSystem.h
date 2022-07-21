#pragma once

#include <Melone.h>

#include <random>

class Random
{
public:
	static void Init()
	{
		sRandomEngine.seed(std::random_device()());
	}

	static float Rand()
	{
		return (float)sDistribution(sRandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 sRandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> sDistribution;
};

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec2 VelocityVariation;
	glm::vec4 ColorBegin;
	glm::vec4 ColorEnd;
	float SizeBegin;
	float SizeEnd;
	float SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem(unsigned int maxParticles = 10000);

	void OnUpdate(Melone::Timestep ts);
	void OnRender(Melone::Camera& camera);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin;
		glm::vec4 ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin;
		float SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> mParticlePool;
	unsigned int mPoolIndex;

	unsigned int mQuadVA = 0;
	std::unique_ptr<Melone::Shader> mParticleShader;
	int mParticleShaderViewProj;
	int mParticleShaderTransform;
	int mParticleShaderColor;
};