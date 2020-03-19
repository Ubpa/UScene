#pragma once

#include "SObj.h"

#include <UECS/core/World.h>

namespace Ubpa {
	class Scene : public World {
	public:
		Scene(const std::string& name);

		template<typename... Cmpts>
		std::tuple<Cmpt::SObj*, Cmpts *...> CreateSObj(Cmpt::SObj* parent = nullptr);

		Cmpt::SObj* Root() { return root; }
		const Cmpt::SObj* Root() const { return root; }

	private:
		Cmpt::SObj* root;

		using World::CreateEntity;
	};
}

#include "detail/Scene.inl"
