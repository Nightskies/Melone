#pragma once

namespace Melone
{
	class Timestep
	{
	private:
		float mTimestep;
	public:
		Timestep(float timestep = 0.0f);
		~Timestep() = default;
	public:
		float GetSeconds() const { return mTimestep; }
		float GetMilliseconds() const { return mTimestep * 1000.0f; }

		float GetTimestep();

		operator float() const { return mTimestep; }
	};
}