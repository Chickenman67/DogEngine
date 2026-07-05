#pragma once

#include <cstdint>
#include <random>
#include <functional>

namespace DogEngine {

	class UUID {
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

}

namespace std {
	template<>
	struct hash<DogEngine::UUID> {
		std::size_t operator()(const DogEngine::UUID& uuid) const {
			return (uint64_t)uuid;
		}
	};
}
