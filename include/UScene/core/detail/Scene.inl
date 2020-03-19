#pragma once

namespace Ubpa {
	template<typename... Cmpts>
	std::tuple<Cmpt::SObj*, Cmpts *...> Scene::CreateSObj(Cmpt::SObj* parent) {
		static_assert(!(std::is_same_v<Cmpts, Cmpt::SObj> || ...));
		static_assert((std::is_base_of_v<Cmpt::ECmpt, Cmpts> && ...));

		auto rst = World::CreateEntity<Cmpt::SObj, Cmpts ...>();
		auto sobj = std::get<Cmpt::SObj*>(rst);

		(parent ? parent : root)->AddChild(sobj);

		return { sobj, std::get<Cmpts*>(rst)... };
	}
}
