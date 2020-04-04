#pragma once

#include "../Component.h"

namespace Ubpa {
	template<typename Cmpt>
	Cmpt* SObj::Get() {
		static_assert(std::is_base_of_v<Component, Cmpt>);
		return entity->Get<Cmpt>();
	}

	template<typename Cmpt>
	const Cmpt* SObj::Get() const {
		static_assert(std::is_base_of_v<Component, Cmpt>);
		return const_cast<SObj*>(this)->Get<Cmpt>();
	}

	template<typename... Cmpts>
	std::tuple<Cmpts *...> SObj::Attach() {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		static_assert(((!std::is_same_v<Cmpt::Transform, Cmpts>) &&...),
			"Cmpt::Transform is already attached");
		auto cmpts = entity->Attach<Cmpts...>();
		((std::get<Cmpts*>(cmpts)->sobj = this), ...);
		return cmpts;
	}

	template<typename Cmpt>
	Cmpt* SObj::GetOrAttach() {
		auto cmpt = Get<Cmpt>();
		if (!cmpt) {
			std::tie(cmpt) = Attach<Cmpt>();
			cmpt->sobj = this;
		}
		return cmpt;
	}

	template<typename... Cmpts>
	void SObj::Detach() {
		static_assert((std::is_base_of_v<Component, Cmpts> && ...));
		static_assert(((!std::is_same_v<Cmpt::Transform, Cmpts>) &&...),
			"Cmpt::Transform can be dettached");
		entity->Detach<Cmpts...>();
	}
}
