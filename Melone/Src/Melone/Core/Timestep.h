#pragma once

namespace Melone
{
	class Timestep
	{
	private:
		float mTime;
	public:
		Timestep(float time = 0.0f)
			:
			mTime(time)
		{}

		~Timestep() = default;
	public:
		float GetSeconds() const { return mTime; }
		float GetMilliseconds() const { return mTime * 1000.0f; }

		operator float() const { return mTime; }
	};
}
