#pragma once

#include "SObj.h"

#include <UECS/World.h>

namespace Ubpa {
	class Scene : public World {
	public:
		Scene(const std::string& name);
		~Scene();

		SObj* Root() { return root; }
		const SObj* Root() const { return root; }

		template<typename... Cmpts>
		std::tuple<SObj*, Cmpts *...> CreateSObj(const std::string& name, SObj* parent = nullptr);

	private:
		SObj* const root;

		using World::CreateEntity;
	};
}

#include "detail/Scene.inl"
