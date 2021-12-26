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
		~Timestep(void) = default;
	public:
		float getSeconds(void) const { return mTime; }
		float getMilliseconds(void) const { return mTime * 1000.0f; }

		operator float(void) const { return mTime; }
	};
}
