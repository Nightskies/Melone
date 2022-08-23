#pragma once

#include <xhash>

namespace Melone
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return mUUID; }
	private:
		uint64_t mUUID;
	};
}

namespace std 
{

	template<>
	struct hash<Melone::UUID>
	{
		std::size_t operator()(const Melone::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}
