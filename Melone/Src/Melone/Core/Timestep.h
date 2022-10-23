#pragma once

namespace Melone
{
	class Timestep
	{
	public:
		Timestep(float timestep = 0.0f);
		~Timestep() = default;

		float GetSeconds() const { return mTimestep; }
		float GetMilliseconds() const { return mTimestep * 1000.0f; }

		float GetTimestep();

		operator float() const { return mTimestep; }
	private:
		float mTimestep;
	};
}