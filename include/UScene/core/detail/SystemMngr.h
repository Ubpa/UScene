#pragma once

#include <UECS/World.h>

namespace Ubpa {
	class SystemMngr {
	public:
		static SystemMngr& Instance() noexcept {
			static SystemMngr instance;
			return instance;
		}

	private:
		friend class SObj;
		friend class Scene;

		template<typename Cmpt>
		void Regist();

		void Update(World* world);

		std::map<size_t, std::function<void(World*)>> OnUpdates;
	};
}

#include "SystemMngr.inl"
