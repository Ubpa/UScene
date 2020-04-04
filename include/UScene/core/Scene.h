#pragma once

#include "SObj.h"

#include <UECS/World.h>

#include <UDP/Basic/Read.h>

namespace Ubpa {
	class Scene : public World {
	public:
		Read<Scene, SObj*> root{ nullptr };

		Scene(const std::string& name = "");
		virtual ~Scene();

		// auto add Cmpt::Transform
		// if parent == nullptr, it will be added to root
		template<typename... Cmpts>
		std::tuple<SObj*, Cmpt::Transform*, Cmpts *...> CreateSObj(const std::string& name, SObj* parent = nullptr);

		void Update();

	private:
		using World::CreateEntity;
	};
}

#include "detail/Scene.inl"
