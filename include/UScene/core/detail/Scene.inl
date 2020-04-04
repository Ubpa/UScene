#pragma once

#include "../Component.h"

namespace Ubpa {
	template<typename... Cmpts>
	std::tuple<SObj*, Cmpt::Transform*, Cmpts *...> Scene::CreateSObj(const std::string& name, SObj* parent) {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		static_assert(((!std::is_same_v<Cmpt::Transform, Cmpts>) && ...));
		assert(!parent || parent->IsDescendantOf(root));

		auto rst = World::CreateEntity<Cmpt::Transform, Cmpts ...>();
		auto entity = std::get<Entity*>(rst);
		auto sobj = new SObj(this, entity, name);
		std::get<Cmpt::Transform*>(rst)->sobj = sobj;
		((std::get<Cmpts*>(rst)->sobj = sobj), ...);

		(parent ? parent : root.get())->AddChild(sobj);

		return { sobj, std::get<Cmpt::Transform*>(rst), std::get<Cmpts*>(rst)... };
	}
}
