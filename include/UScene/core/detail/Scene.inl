#pragma once

namespace Ubpa {
	template<typename... Cmpts>
	std::tuple<SObj*, Cmpts *...> Scene::CreateSObj(const std::string& name, SObj* parent) {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		static_assert(((!detail::SObj_::IsNecessaryCmpt<Cmpts>) &&...),
			"<Cmpts> is necessary component");
		assert(!parent || parent->IsDescendantOf(root));

		auto rst = World::CreateEntity<Cmpt::SObjPtr,
			Cmpt::Position, Cmpt::Rotation, Cmpt::Scale,
			Cmpt::Transform, Cmpt::L2W,
			Cmpts ...>();
		auto entity = std::get<Entity*>(rst);
		auto sobj = new SObj(this, entity, name);
		std::get<Cmpt::SObjPtr*>(rst)->sobj = sobj;

		(parent ? parent : root.get())->AddChild(sobj);

		return { sobj, std::get<Cmpts*>(rst)... };
	}
}
