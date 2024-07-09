#pragma once

#include <memory>

namespace Omotura 
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t _uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Omotura::UUID>
	{
		std::size_t operator()(const Omotura::UUID& _uuid) const
		{
			return hash<uint64_t>()((uint64_t)_uuid);
		}
	};
}
