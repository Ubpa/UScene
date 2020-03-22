#pragma once

#include "SObj.h"

#include <UECS/World.h>

#include <UDP/Basic/Read.h>

namespace Ubpa {
	class Scene : public World {
	public:
		Read<Scene, SObj*> root;

		Scene(const std::string& name = "");
		virtual ~Scene();

		template<typename... Cmpts>
		std::tuple<SObj*, Cmpts *...> CreateSObj(const std::string& name, SObj* parent = nullptr);

	private:
		using World::CreateEntity;
	};
}

#include "detail/Scene.inl"
